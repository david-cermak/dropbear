#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>

#include "channel.h"

// Minimal stubs for missing libc/Dropbear symbols on ESP-IDF.

uid_t getuid(void)
{
	return 0;
}

__attribute__((weak)) struct passwd *getpwuid(uid_t uid)
{
	static struct passwd pw;
	static char pw_name[] = "root";
	static char pw_dir[] = "/";
	static char pw_shell[] = "/bin/sh";

	memset(&pw, 0, sizeof(pw));
	pw.pw_uid = uid;
	pw.pw_gid = 0;
	pw.pw_name = pw_name;
	pw.pw_dir = pw_dir;
	pw.pw_shell = pw_shell;
	return &pw;
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

__attribute__((weak)) void svr_chansess_checksignal(void)
{
}

__attribute__((weak)) void svr_authinitialise(void)
{
}

__attribute__((weak)) void svr_chansessinitialise(void)
{
}

__attribute__((weak)) void recv_msg_userauth_request(void)
{
}

__attribute__((weak)) const struct ChanType svrchansess = {
	"session",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};


int getrlimit(int resource, struct rlimit *rlim)
{
    return 0;
}
int setrlimit(int resource, const struct rlimit *rlim)
{
    return 0;
}