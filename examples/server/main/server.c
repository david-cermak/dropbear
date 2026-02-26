#include <stdio.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "protocol_examples_common.h"
#include "includes.h"
#include "dbutil.h"
#include "runopts.h"
#include "session.h"
#include "netio.h"
#include "crypto_desc.h"
#include "dbrandom.h"
#include "algo.h"


#define DEFAULT_PORT "2222"

/*
 * Hardcoded ed25519 host key in Dropbear's native binary format.
 * This is the same key used in the libssh ESP-IDF example
 * (ssh_host_ed25519_key), converted to Dropbear's wire format:
 *   [4-byte string length] "ssh-ed25519"
 *   [4-byte blob length=64] [32-byte private key] [32-byte public key]
 */
static const uint8_t hostkey_ed25519[] = {
	0x00, 0x00, 0x00, 0x0b, 0x73, 0x73, 0x68, 0x2d, 0x65, 0x64, 0x32, 0x35,
	0x35, 0x31, 0x39, 0x00, 0x00, 0x00, 0x40, 0xa1, 0xfa, 0xc0, 0x61, 0x4c,
	0x4f, 0xfc, 0xfe, 0xb4, 0x32, 0xdd, 0x57, 0x16, 0xe1, 0xb3, 0x16, 0x0d,
	0xef, 0x95, 0xe0, 0x64, 0x3e, 0x8e, 0xcc, 0xe9, 0xc2, 0xf4, 0x4b, 0x59,
	0xdb, 0xc0, 0x1f, 0x67, 0xf2, 0x99, 0x9e, 0x7c, 0x06, 0x39, 0x5a, 0xa5,
	0x3c, 0xfa, 0xbc, 0x6d, 0xb4, 0xf0, 0x07, 0xb2, 0x99, 0x64, 0x7a, 0xda,
	0xa6, 0xbf, 0x5d, 0x5e, 0x55, 0x4f, 0xc0, 0x4b, 0x9a, 0x39, 0xd2,
};

static void load_hardcoded_hostkeys(void)
{
	sign_key *key = new_sign_key();
	enum signkey_type type = DROPBEAR_SIGNKEY_ANY;

	buffer *buf = buf_new(sizeof(hostkey_ed25519));
	buf_putbytes(buf, hostkey_ed25519, sizeof(hostkey_ed25519));
	buf_setpos(buf, 0);

	if (buf_get_priv_key(buf, key, &type) == DROPBEAR_FAILURE) {
		dropbear_exit("Failed to parse hardcoded ed25519 host key");
	}
	buf_burn_free(buf);

	svr_opts.hostkey = new_sign_key();
	svr_opts.hostkey->ed25519key = key->ed25519key;
	key->ed25519key = NULL;
	sign_key_free(key);

	dropbear_log(LOG_INFO, "Loaded hardcoded ed25519 host key");
}

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
	load_hardcoded_hostkeys();
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

void init_idf(void)
{
	    // Initialize ESP-IDF components
		ESP_ERROR_CHECK(nvs_flash_init());
		ESP_ERROR_CHECK(esp_netif_init());
		ESP_ERROR_CHECK(esp_event_loop_create_default());
		ESP_ERROR_CHECK(example_connect());
}

void app_main(void)
{
	int listensocks[MAX_LISTEN_ADDR];
	int maxfd = -1;
	size_t listensockcount;

	init_idf();
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
