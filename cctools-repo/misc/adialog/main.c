/*-------------------------------------------------------------------------

CCTools console dialog utility

(c) 2013 Alexander Chukov <sash@pdaXrom.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "tcp.h"

enum {
    FUNC_NONE = 0,
    FUNC_YESNO,
    FUNC_MSGBOX,
    FUNC_EDITBOX,
    FUNC_INSTALL
};

int send_command(tcp_channel *channel, char *cmd, char *title, char *message)
{
    char buf[1024];
    int r;
    snprintf(buf, sizeof(buf) - 1, "%s\n%s\n%s\n", cmd, title, message);
    if ((r = tcp_write(channel, buf, strlen(buf))) <= 0) {
	fprintf(stderr, "tcp_write()\n");
	return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int r;
    int func = FUNC_NONE;
    char *title = NULL;
    char *message = NULL;
    char *text = NULL;
    char buf[1024];

    while (--argc > 0) {
	if (!strcmp(argv[1], "--yesno"))
	    func = FUNC_YESNO;
	else if (!strcmp(argv[1], "--msgbox"))
	    func = FUNC_MSGBOX;
	else if (!strcmp(argv[1], "--editbox"))
	    func = FUNC_EDITBOX;
	else if (!strcmp(argv[1], "--title")) {
	    title = argv[2];
	    argc--;
	    argv++;
	} else if (!strcmp(argv[1], "--message")) {
	    message = argv[2];
	    argc--;
	    argv++;
	} else if (!strcmp(argv[1], "--text")) {
	    text = argv[2];
	    argc--;
	    argv++;
	} else if (!strcmp(argv[1], "--install")) {
	    func = FUNC_INSTALL;
	    title = argv[2];
	    argc--;
	    argv++;
	} else {
	    fprintf(stderr, "Unknown arg %s", argv[1]);
	    return -1;
	}
	argv++;
    }

    int port = 0;

    if (getenv("ADIALOG_PORT")) {
	port = atoi(getenv("ADIALOG_PORT"));
    }

    if (!port) {
	port = 13527;
    }

    tcp_channel *server = tcp_open(TCP_CLIENT, "127.0.0.1", port);
    if (!server) {
	fprintf(stderr, "tcp_open()\n");
	return -1;
    }


    if (func == FUNC_YESNO) {
	send_command(server, "yesno", title, message);
	if ((r = tcp_read(server, buf, sizeof(buf))) > 0) {
	    buf[r] = 0;
	    printf("%s\n", buf);
	}
    }

    if (func == FUNC_MSGBOX) {
	send_command(server, "textview", title, message);
	snprintf(buf, sizeof(buf), "%s\n", text);
	if ((r = tcp_write(server, buf, strlen(buf))) <= 0) {
	    fprintf(stderr, "tcp_write()\n");
	    return -1;
	}
	if ((r = tcp_read(server, buf, sizeof(buf))) > 0) {
	    buf[r] = 0;
	    printf("%s\n", buf);
	}
    }

    if (func == FUNC_EDITBOX) {
	send_command(server, "editbox", title, message);
	if ((r = tcp_read(server, buf, sizeof(buf))) > 0) {
	    buf[r] = 0;
	    printf("%s\n", buf);
	}
    }

    if (func == FUNC_INSTALL) {
	send_command(server, "install", title, message);
    }

    tcp_close(server);

    return 0;
}

