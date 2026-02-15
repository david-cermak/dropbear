#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
struct rlimit {
	unsigned long rlim_cur;
	unsigned long rlim_max;
};

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * Minimal stubs for missing libc/Dropbear symbols on ESP-IDF.
 *
 * Hardcoded credentials: any username / password "dropbear"
 */

#define HARDCODED_PASSWORD "dropbear"

uid_t getuid(void)
{
	return 0;
}

uid_t geteuid(void)
{
	return 0;
}

static struct passwd _esp_pw;
static char _esp_pw_name[32] = "root";
static char _esp_pw_dir[] = "/";
static char _esp_pw_shell[] = "/bin/sh";
static char _esp_pw_passwd[] = HARDCODED_PASSWORD;

static struct passwd *_fill_pw(const char *name, uid_t uid)
{
	memset(&_esp_pw, 0, sizeof(_esp_pw));
	if (name) {
		snprintf(_esp_pw_name, sizeof(_esp_pw_name), "%s", name);
	}
	_esp_pw.pw_uid = uid;
	_esp_pw.pw_gid = 0;
	_esp_pw.pw_name = _esp_pw_name;
	_esp_pw.pw_dir = _esp_pw_dir;
	_esp_pw.pw_shell = _esp_pw_shell;
	_esp_pw.pw_passwd = _esp_pw_passwd;
	return &_esp_pw;
}

__attribute__((weak)) struct passwd *getpwuid(uid_t uid)
{
	return _fill_pw(NULL, uid);
}

struct passwd *getpwnam(const char *name)
{
	return _fill_pw(name, 0);
}

/*
 * Trivial crypt() that returns the key as-is (plaintext comparison).
 * svr-authpasswd.c does: constant_time_strcmp(crypt(input, stored), stored)
 * so with this stub the user must type the plaintext from pw_passwd.
 */
char *crypt(const char *key, const char *salt)
{
	(void)salt;
	static char buf[128];
	strncpy(buf, key, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';
	return buf;
}

/*
 * Shell validation stubs -- always return "/bin/sh" so that
 * checkusername() in svr-auth.c accepts our hardcoded shell.
 */
static int _shell_returned = 0;

void setusershell(void)
{
	_shell_returned = 0;
}

char *getusershell(void)
{
	if (_shell_returned == 0) {
		_shell_returned = 1;
		return "/bin/sh";
	}
	return NULL;
}

void endusershell(void)
{
	_shell_returned = 0;
}

__attribute__((weak)) char *dirname(char *path)
{
	char *slash;

	if (path == NULL || *path == '\0') {
		return (char *)".";
	}

	slash = strrchr(path, '/');
	if (slash == NULL) {
		return (char *)".";
	}

	/* trim trailing slashes */
	while (slash > path && *slash == '/') {
		*slash = '\0';
		slash--;
	}

	if (*path == '\0') {
		return (char *)"/";
	}

	return path;
}

__attribute__((weak)) int getgroups(int size, gid_t list[])
{
	if (size > 0 && list != NULL) {
		list[0] = 0;
		return 1;
	}
	return 0;
}

__attribute__((weak)) void disallow_core(void)
{
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	if (req == NULL) {
		errno = EINVAL;
		return -1;
	}
	uint32_t ms = (uint32_t)(req->tv_sec * 1000) + (uint32_t)(req->tv_nsec / 1000000);
	if (ms > 0) {
		vTaskDelay(pdMS_TO_TICKS(ms));
	}
	if (rem) {
		rem->tv_sec = 0;
		rem->tv_nsec = 0;
	}
	return 0;
}

int getrlimit(int resource, struct rlimit *rlim)
{
	return 0;
}
int setrlimit(int resource, const struct rlimit *rlim)
{
	return 0;
}