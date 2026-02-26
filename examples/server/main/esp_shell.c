/*
 * esp_shell.c - Minimal "shell" session channel for Dropbear on ESP-IDF.
 *
 * Instead of fork()+exec() a real shell, this provides a simple interactive
 * command loop over the SSH channel (similar to the libssh example).
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
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

/* ------------------------------------------------------------------ */
/*  Per-session state (stored in channel->typedata)                   */
/* ------------------------------------------------------------------ */
struct EspShellSess {
	int  chan_fd;           /* Dropbear side of the socket pair    */
	int  shell_fd;          /* shell-task side of the socket pair  */
	TaskHandle_t task;
	volatile int done;      /* set to 1 when the shell task exits  */
};

/* ------------------------------------------------------------------ */
/*  TCP-loopback socket pair (works on any lwIP build)                */
/* ------------------------------------------------------------------ */
static int make_socket_pair(int sv[2])
{
	int lfd = -1, cfd = -1, afd = -1;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd < 0) goto fail;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port        = 0;          /* kernel picks a free port */

	if (bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) goto fail;
	if (listen(lfd, 1) < 0) goto fail;
	if (getsockname(lfd, (struct sockaddr *)&addr, &len) < 0) goto fail;

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cfd < 0) goto fail;
	if (connect(cfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) goto fail;

	afd = accept(lfd, NULL, NULL);
	if (afd < 0) goto fail;

	close(lfd);
	sv[0] = cfd;   /* one end  */
	sv[1] = afd;   /* other end */
	return 0;

fail:
	if (lfd >= 0) close(lfd);
	if (cfd >= 0) close(cfd);
	if (afd >= 0) close(afd);
	return -1;
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
	int len;

	len = snprintf(line, sizeof(line),
		"\r\n%-20s %5s %10s %5s\r\n", "Task", "State", "Stack HWM", "Prio");
	ESP_LOGI(TAG, "%s", line);
	shell_write(fd, line);

	len = snprintf(line, sizeof(line),
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
		len = snprintf(line, sizeof(line),
			"%-20s %5s %10u %5u\r\n",
			task_array[i].pcTaskName,
			state,
			(unsigned)task_array[i].usStackHighWaterMark,
			(unsigned)task_array[i].uxCurrentPriority);
		ESP_LOGI(TAG, "%s", line);
		shell_write(fd, line);
	}

	len = snprintf(line, sizeof(line),
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
/*  Shell task – runs on its own FreeRTOS thread                      */
/* ------------------------------------------------------------------ */
static void shell_task(void *arg)
{
	struct EspShellSess *sess = (struct EspShellSess *)arg;
	int fd = sess->shell_fd;
	char buf[256];
	char cmd[128];
	int  cmd_len = 0;

	shell_write(fd, "\r\n=== ESP32 Dropbear Shell ===\r\n");
	shell_write(fd, "Type 'help' for available commands.\r\n");
	shell_write(fd, "esp32> ");

	while (!sess->done) {
		int n = read(fd, buf, sizeof(buf) - 1);
		if (n <= 0) {
			break;
		}

		for (int i = 0; i < n; i++) {
			unsigned char c = (unsigned char)buf[i];

			if (c == '\r' || c == '\n') {
				shell_write(fd, "\r\n");
				cmd[cmd_len] = '\0';

				if (cmd_len > 0) {
					if (strcmp(cmd, "exit") == 0) {
						shell_write(fd, "Goodbye!\r\n");
						goto done;
					} else if (strcmp(cmd, "reset") == 0) {
						shell_write(fd, "Resetting ESP32...\r\n");
						vTaskDelay(pdMS_TO_TICKS(100));
						esp_restart();
					} else if (strcmp(cmd, "hello") == 0) {
						shell_write(fd, "Hello, world!\r\n");
					} else if (strcmp(cmd, "uptime") == 0) {
						char tmp[64];
						snprintf(tmp, sizeof(tmp), "Uptime: %lu ms\r\n",
							(unsigned long)(xTaskGetTickCount()
								* portTICK_PERIOD_MS));
						shell_write(fd, tmp);
					} else if (strcmp(cmd, "heap") == 0) {
						char tmp[64];
						snprintf(tmp, sizeof(tmp),
							"Free heap: %lu bytes\r\n",
							(unsigned long)esp_get_free_heap_size());
						shell_write(fd, tmp);
#if ENABLE_MEMORY_STATS
					} else if (strcmp(cmd, "stats") == 0) {
						print_all_task_stats(fd);
#endif
					} else if (strcmp(cmd, "help") == 0) {
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
						shell_write(fd, cmd);
						shell_write(fd, "\r\nType 'help' for available commands.\r\n");
					}
				}

				cmd_len = 0;
				shell_write(fd, "esp32> ");
			} else if (c == 0x7f || c == '\b') {
				if (cmd_len > 0) {
					cmd_len--;
					shell_write(fd, "\b \b");
				}
			} else if (c == 0x03) {
				/* Ctrl-C */
				shell_write(fd, "^C\r\n");
				cmd_len = 0;
				shell_write(fd, "esp32> ");
			} else if (c >= 0x20 && cmd_len < (int)sizeof(cmd) - 1) {
				cmd[cmd_len++] = (char)c;
				write(fd, &c, 1);
			}
		}
	}

done:
	sess->done = 1;
	close(fd);
	sess->shell_fd = -1;
	vTaskDelete(NULL);
}

/* ------------------------------------------------------------------ */
/*  ChanType callbacks                                                */
/* ------------------------------------------------------------------ */

static int esp_newchansess(struct Channel *channel)
{
	struct EspShellSess *sess;

	sess = (struct EspShellSess *)m_malloc(sizeof(*sess));
	sess->chan_fd  = -1;
	sess->shell_fd = -1;
	sess->task     = NULL;
	sess->done     = 0;

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

		if (sess->task != NULL) {
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

		channel->readfd  = sv[0];
		channel->writefd = sv[0];
		channel->bidir_fd = 1;

		setnonblocking(sv[0]);

		ses.maxfd = MAX(ses.maxfd, sv[0]);

		if (xTaskCreate(shell_task, "esp_shell", 4096, sess,
				5, &sess->task) != pdPASS) {
			close(sv[0]);
			close(sv[1]);
			sess->chan_fd  = -1;
			sess->shell_fd = -1;
			dropbear_log(LOG_WARNING, "Failed to create shell task");
			goto out;
		}

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

	/* Give the task a moment to notice and exit */
	if (sess->task) {
		vTaskDelay(pdMS_TO_TICKS(50));
		sess->task = NULL;
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
	esp_cleanupchansess
};

