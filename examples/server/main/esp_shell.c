/*
 * esp_shell.c - Minimal "shell" session channel for Dropbear on ESP-IDF.
 *
 * Instead of fork()+exec() a real shell, this provides a simple interactive
 * command loop over the SSH channel (similar to the libssh example).
 * Runs in the main task context (no separate FreeRTOS task).
 *
 * Supported commands: help, hello, uptime, heap, reset, exit
 *
 * Overrides the weak stubs in idf_stubs.c for:
 *   svrchansess, svr_chansessinitialise, svr_chansess_checksignal
 */

#include "includes.h"
#include "session.h"
#include "channel.h"
#include "chansession.h"
#include "dbutil.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "mem_stats.h"

#if ENABLE_MEMORY_STATS
#include "esp_heap_caps.h"
#include <inttypes.h>
#include <stdlib.h>
#endif

#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

#include "socketpair.h"

/* ------------------------------------------------------------------ */
/*  Per-session state (stored in channel->typedata)                   */
/* ------------------------------------------------------------------ */
struct EspShellSess {
	int  chan_fd;           /* Dropbear side of the socket pair    */
	int  shell_fd;          /* shell side (read client input here) */
	int  done;              /* set to 1 when shell should close    */
	int  banner_sent;       /* 1 after welcome message              */
	char cmd[128];
	int  cmd_len;
};

/* ------------------------------------------------------------------ */
/*  Socket pair (from espressif/sock_utils)                            */
/* ------------------------------------------------------------------ */
static int make_socket_pair(int sv[2])
{
	return socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
}

/* ------------------------------------------------------------------ */
/*  Helpers                                                           */
/* ------------------------------------------------------------------ */
static void shell_write(int fd, const char *s)
{
	int len = (int)strlen(s);
	while (len > 0) {
		int n = write(fd, s, len);
		if (n <= 0) break;
		s   += n;
		len -= n;
	}
}

#if ENABLE_MEMORY_STATS
static const char *TAG = "esp_shell";

/**
 * Print stack high-water marks for all running tasks and heap summary
 * (same as libssh example). Writes to both ESP log and the shell fd.
 */
static void print_all_task_stats(int fd)
{
	UBaseType_t num_tasks = uxTaskGetNumberOfTasks();
	TaskStatus_t *task_array = malloc(num_tasks * sizeof(TaskStatus_t));
	if (task_array == NULL) {
		shell_write(fd, "Failed to allocate task status array\r\n");
		return;
	}

	UBaseType_t actual = uxTaskGetSystemState(task_array, num_tasks, NULL);
	char line[128];

	(void)snprintf(line, sizeof(line),
		"\r\n%-20s %5s %10s %5s\r\n", "Task", "State", "Stack HWM", "Prio");
	ESP_LOGI(TAG, "%s", line);
	shell_write(fd, line);

	(void)snprintf(line, sizeof(line),
		"%-20s %5s %10s %5s\r\n", "----", "-----", "---------", "----");
	ESP_LOGI(TAG, "%s", line);
	shell_write(fd, line);

	for (UBaseType_t i = 0; i < actual; i++) {
		const char *state;
		switch (task_array[i].eCurrentState) {
		case eRunning:   state = "RUN";   break;
		case eReady:     state = "RDY";   break;
		case eBlocked:   state = "BLK";   break;
		case eSuspended: state = "SUS";   break;
		case eDeleted:   state = "DEL";   break;
		default:         state = "???";   break;
		}
		(void)snprintf(line, sizeof(line),
			"%-20s %5s %10u %5u\r\n",
			task_array[i].pcTaskName,
			state,
			(unsigned)task_array[i].usStackHighWaterMark,
			(unsigned)task_array[i].uxCurrentPriority);
		ESP_LOGI(TAG, "%s", line);
		shell_write(fd, line);
	}

	(void)snprintf(line, sizeof(line),
		"\r\nFree heap: %" PRIu32 " | Min ever: %" PRIu32 " | Internal: %zu\r\n",
		esp_get_free_heap_size(),
		esp_get_minimum_free_heap_size(),
		heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
	ESP_LOGI(TAG, "%s", line);
	shell_write(fd, line);

	free(task_array);
}
#endif

/* ------------------------------------------------------------------ */
/*  Shell I/O – runs in main task when shell_fd is readable             */
/* ------------------------------------------------------------------ */
static void shell_process_input(struct EspShellSess *sess)
{
	int fd = sess->shell_fd;
	char buf[256];
	int n, i;

	if (fd < 0 || sess->done) return;

	/* Send banner once */
	if (!sess->banner_sent) {
		shell_write(fd, "\r\n=== ESP32 Dropbear Shell ===\r\n");
		shell_write(fd, "Type 'help' for available commands.\r\n");
		shell_write(fd, "esp32> ");
		sess->banner_sent = 1;
	}

	n = read(fd, buf, sizeof(buf) - 1);
	if (n <= 0) {
		if (n == 0 || (errno != EAGAIN && errno != EWOULDBLOCK))
			sess->done = 1;
		return;
	}

	for (i = 0; i < n; i++) {
		unsigned char c = (unsigned char)buf[i];

		if (c == '\r' || c == '\n') {
			shell_write(fd, "\r\n");
			sess->cmd[sess->cmd_len] = '\0';

			if (sess->cmd_len > 0) {
				if (strcmp(sess->cmd, "exit") == 0) {
					shell_write(fd, "Goodbye!\r\n");
					sess->done = 1;
					return;
				} else if (strcmp(sess->cmd, "reset") == 0) {
					shell_write(fd, "Resetting ESP32...\r\n");
					vTaskDelay(pdMS_TO_TICKS(100));
					esp_restart();
				} else if (strcmp(sess->cmd, "hello") == 0) {
					shell_write(fd, "Hello, world!\r\n");
				} else if (strcmp(sess->cmd, "uptime") == 0) {
					char tmp[64];
					snprintf(tmp, sizeof(tmp), "Uptime: %lu ms\r\n",
						(unsigned long)(xTaskGetTickCount()
							* portTICK_PERIOD_MS));
					shell_write(fd, tmp);
				} else if (strcmp(sess->cmd, "heap") == 0) {
					char tmp[64];
					snprintf(tmp, sizeof(tmp),
						"Free heap: %lu bytes\r\n",
						(unsigned long)esp_get_free_heap_size());
					shell_write(fd, tmp);
#if ENABLE_MEMORY_STATS
				} else if (strcmp(sess->cmd, "stats") == 0) {
					print_all_task_stats(fd);
#endif
				} else if (strcmp(sess->cmd, "help") == 0) {
					shell_write(fd,
						"Available commands:\r\n"
						"  hello   - print greeting\r\n"
						"  uptime  - show uptime in ms\r\n"
						"  heap    - show free heap\r\n"
#if ENABLE_MEMORY_STATS
						"  stats   - show task and heap stats\r\n"
#endif
						"  reset   - restart ESP32\r\n"
						"  exit    - close session\r\n"
						"  help    - this message\r\n");
				} else {
					shell_write(fd, "Unknown command: ");
					shell_write(fd, sess->cmd);
					shell_write(fd, "\r\nType 'help' for available commands.\r\n");
				}
			}

			sess->cmd_len = 0;
			shell_write(fd, "esp32> ");
		} else if (c == 0x7f || c == '\b') {
			if (sess->cmd_len > 0) {
				sess->cmd_len--;
				shell_write(fd, "\b \b");
			}
		} else if (c == 0x03) {
			shell_write(fd, "^C\r\n");
			sess->cmd_len = 0;
			shell_write(fd, "esp32> ");
		} else if (c >= 0x20 && sess->cmd_len < (int)sizeof(sess->cmd) - 1) {
			sess->cmd[sess->cmd_len++] = (char)c;
			write(fd, &c, 1);
		}
	}
}

static void esp_set_extra_fds(struct Channel *channel, fd_set *readfds, fd_set *writefds)
{
	struct EspShellSess *sess = (struct EspShellSess *)channel->typedata;
	(void)writefds;
	if (sess && sess->shell_fd >= 0 && !sess->done) {
		FD_SET(sess->shell_fd, readfds);
		ses.maxfd = MAX(ses.maxfd, sess->shell_fd);
	}
}

static void esp_handle_extra_io(struct Channel *channel,
	const fd_set *readfds, const fd_set *writefds)
{
	struct EspShellSess *sess = (struct EspShellSess *)channel->typedata;
	(void)writefds;
	if (sess && sess->shell_fd >= 0 && FD_ISSET(sess->shell_fd, readfds)) {
		shell_process_input(sess);
	}
}

/* ------------------------------------------------------------------ */
/*  ChanType callbacks                                                */
/* ------------------------------------------------------------------ */

static int esp_newchansess(struct Channel *channel)
{
	struct EspShellSess *sess;

	sess = (struct EspShellSess *)m_malloc(sizeof(*sess));
	sess->chan_fd     = -1;
	sess->shell_fd    = -1;
	sess->done        = 0;
	sess->banner_sent = 0;
	sess->cmd_len     = 0;

	channel->typedata = sess;
	channel->prio = DROPBEAR_PRIO_LOWDELAY;
	return 0;
}

static int esp_sesscheckclose(struct Channel *channel)
{
	struct EspShellSess *sess = (struct EspShellSess *)channel->typedata;
	return (sess != NULL && sess->done);
}

static void esp_chansessionrequest(struct Channel *channel)
{
	unsigned int typelen;
	char *type = buf_getstring(ses.payload, &typelen);
	unsigned char wantreply = buf_getbool(ses.payload);
	int ret = DROPBEAR_FAILURE;

	struct EspShellSess *sess = (struct EspShellSess *)channel->typedata;

	TRACE(("esp_chansessionrequest: type='%s'", type))

	if (strcmp(type, "pty-req") == 0) {
		/* Accept but ignore -- we don't allocate a real PTY */
		ret = DROPBEAR_SUCCESS;

	} else if (strcmp(type, "shell") == 0 || strcmp(type, "exec") == 0) {

		if (sess->shell_fd >= 0) {
			dropbear_log(LOG_WARNING, "Shell already running");
			goto out;
		}

		int sv[2];
		if (make_socket_pair(sv) < 0) {
			dropbear_log(LOG_WARNING, "make_socket_pair failed");
			goto out;
		}

		sess->chan_fd  = sv[0];
		sess->shell_fd = sv[1];

		channel->readfd   = sv[0];
		channel->writefd  = sv[0];
		channel->bidir_fd = 1;

		setnonblocking(sv[0]);
		setnonblocking(sv[1]);

		ses.maxfd = MAX(ses.maxfd, sv[0]);
		ses.maxfd = MAX(ses.maxfd, sv[1]);

		dropbear_log(LOG_INFO, "ESP32 shell session started");
#if ENABLE_MEMORY_STATS
		print_mem_stats("session ready (auth+channel)");
#endif
		ret = DROPBEAR_SUCCESS;

	} else if (strcmp(type, "window-change") == 0) {
		ret = DROPBEAR_SUCCESS;

	} else if (strcmp(type, "signal") == 0) {
		ret = DROPBEAR_SUCCESS;

	} else {
		TRACE(("esp_chansessionrequest: unhandled type '%s'", type))
	}

out:
	if (wantreply) {
		if (ret == DROPBEAR_SUCCESS) {
			send_msg_channel_success(channel);
		} else {
			send_msg_channel_failure(channel);
		}
	}

	m_free(type);
}

static void esp_closechansess(const struct Channel *channel)
{
	struct EspShellSess *sess = (struct EspShellSess *)channel->typedata;
	if (sess) {
		sess->done = 1;
	}
}

static void esp_cleanupchansess(const struct Channel *channel)
{
	struct EspShellSess *sess = (struct EspShellSess *)channel->typedata;
	if (!sess) return;

	sess->done = 1;

	if (sess->shell_fd >= 0) {
		close(sess->shell_fd);
		sess->shell_fd = -1;
	}

	m_free(sess);
}

/* ------------------------------------------------------------------ */
/*  Exported symbols (override weak stubs in idf_stubs.c)             */
/* ------------------------------------------------------------------ */

const struct ChanType svrchansess = {
	"session",
	esp_newchansess,
	esp_sesscheckclose,
	esp_chansessionrequest,
	esp_closechansess,
	esp_cleanupchansess,
	esp_set_extra_fds,
	esp_handle_extra_io
};

