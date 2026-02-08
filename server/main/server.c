#include <stdio.h>

#include "includes.h"
#include "dbutil.h"
#include "runopts.h"
#include "session.h"
#include "netio.h"
#include "crypto_desc.h"
#include "dbrandom.h"


#define DEFAULT_PORT "2222"

static void dropbear_setup(const char *port)
{
	char *argv[] = {
		"dropbear",
		"-F",
		"-p",
		(char *)port,
		NULL
	};
	int argc = 4;

	_dropbear_exit = svr_dropbear_exit;
	_dropbear_log = svr_dropbear_log;

	disallow_core();
	svr_getopts(argc, argv);
	seedrandom();
	crypto_init();
	load_all_hostkeys();
}

static size_t listen_sockets(int *socks, size_t sockcount, int *maxfd)
{
	unsigned int i, n;
	char *errstring = NULL;
	size_t sockpos = 0;
	int nsock;

	for (i = 0; i < svr_opts.portcount; i++) {
		nsock = dropbear_listen(
			svr_opts.addresses[i],
			svr_opts.ports[i],
			&socks[sockpos],
			sockcount - sockpos,
			&errstring,
			maxfd,
			svr_opts.interface
		);

		if (nsock < 0) {
			dropbear_log(
				LOG_WARNING,
				"Failed listening on '%s': %s",
				svr_opts.ports[i],
				errstring ? errstring : "unknown error"
			);
			m_free(errstring);
			continue;
		}

		for (n = 0; n < (unsigned int)nsock; n++) {
			int sock = socks[sockpos + n];
			set_sock_priority(sock, DROPBEAR_PRIO_LOWDELAY);
		}

		sockpos += (size_t)nsock;
	}

	return sockpos;
}

void app_main(void)
{
	int listensocks[MAX_LISTEN_ADDR];
	int maxfd = -1;
	size_t listensockcount;

	dropbear_setup(DEFAULT_PORT);

	listensockcount = listen_sockets(listensocks, MAX_LISTEN_ADDR, &maxfd);
	if (listensockcount == 0) {
		dropbear_exit("No listening ports available.");
	}

	printf("Dropbear SSH server listening on port %s\n", DEFAULT_PORT);

	for (;;) {
		struct sockaddr_storage remoteaddr;
		socklen_t remoteaddrlen = sizeof(remoteaddr);
		int childsock;
		char *remote_host = NULL;
		char *remote_port = NULL;

		/* Simple example: block on the first listening socket. */
		childsock = accept(listensocks[0], (struct sockaddr *)&remoteaddr, &remoteaddrlen);
		if (childsock < 0) {
			continue;
		}

		getaddrstring(&remoteaddr, &remote_host, &remote_port, 0);
		dropbear_log(LOG_INFO, "Connection from %s:%s",
			remote_host ? remote_host : "?",
			remote_port ? remote_port : "?");
		m_free(remote_host);
		m_free(remote_port);

		seedrandom();
		/* svr_session never returns. */
		svr_session(childsock, -1);
	}
}
