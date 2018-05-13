/*-------------------------------------------------------------------------
TCP wrapper
(c) 2008 Alexander Chukov <sash@pdaXrom.org>

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
#ifndef TCP_H
#define TCP_H

#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#else
#include <windows.h>
#endif
#ifndef NOSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

enum {
    TCP_SERVER = 0,
    TCP_SSL_SERVER,
    TCP_CLIENT,
    TCP_SSL_CLIENT
};

typedef struct _tcp_channel {
    int s;
    struct sockaddr_in my_addr;
    int mode;
#ifndef NOSSL
    SSL *ssl;
    SSL_CTX *ctx;
#endif
} tcp_channel;

#define tcp_fd(u) (u->s)

tcp_channel *tcp_open(int mode, const char *addr, int port);
tcp_channel *tcp_accept(tcp_channel *u);
int tcp_read(tcp_channel *u, char *buf, size_t len);
int tcp_write(tcp_channel *u, char *buf, size_t len);
int tcp_close(tcp_channel *u);

#endif
