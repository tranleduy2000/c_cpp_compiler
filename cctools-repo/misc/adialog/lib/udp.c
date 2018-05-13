/*-------------------------------------------------------------------------
UDP wrapper
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define closesocket close
#else
#include <windows.h>
#endif

#define PORT 9930

#include "udp.h"

#ifdef _WIN32
typedef int socklen_t;

static int winsock_inited = 0;
static int winsock_init(void)
{
    WSADATA w;

    if (winsock_inited)
	return 0;

    /* Open windows connection */
    if (WSAStartup(0x0101, &w) != 0) {
	fprintf(stderr, "Could not open Windows connection.\n");
	return -1;
    }
    
    winsock_inited = 1;
    return 0;
}
#endif

udp_channel *udp_open(int mode, char *addr, int port)
{
#ifdef _WIN32
    if (winsock_init())
	return NULL;
#endif

    udp_channel *u = (udp_channel *)malloc(sizeof(udp_channel));

    u->mode = mode;

    if ((u->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
	free(u);
	return NULL;
    }

    memset(&u->my_addr, 0, sizeof(u->my_addr));
    u->my_addr.sin_family = AF_INET;
    u->my_addr.sin_port = htons(port);

    if (mode == UDP_SERVER) {
	u->my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(u->s, (struct sockaddr* ) &u->my_addr, sizeof(u->my_addr)) == -1) {
    	    fprintf(stderr, "bind() failed\n");
	    closesocket(u->s);
	    free(u);
	    return NULL;
	}

	u->inp_addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
	u->out_addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    } else {
	u->inp_addr = NULL;
	u->out_addr = NULL;

#ifdef _WIN32
	if ((u->my_addr.sin_addr.s_addr = inet_addr(addr)) == INADDR_NONE) {
	    fprintf(stderr, "inet_addr() failed\n");
#else
	if (inet_aton(addr, &u->my_addr.sin_addr) == 0) {
    	    fprintf(stderr, "inet_aton() failed\n");
#endif
    	    closesocket(u->s);
    	    free(u);
    	    return NULL;
	}
    }

    u->forward = NULL;

    return u;
}

int udp_close(udp_channel *u)
{
    if (u->forward) {
	udp_forward *tmp;
	while (u->forward) {
	    tmp = u->forward->next;
	    free(u->forward->label);
	    free(u->forward);
	    u->forward = tmp;
	}
    }
    if (u->inp_addr)
	free(u->inp_addr);
    if (u->out_addr)
	free(u->out_addr);
    if (u->s >=0)
	closesocket(u->s);
    free(u);
/*
#ifdef _WIN32
    if (winsock_inited) {
	WSACleanup();
	winsock_inited = 0;
    }
#endif
 */
    return 0;
}

int udp_read(udp_channel *u, uint8_t *buf, size_t len)
{
    int r;
    socklen_t slen = sizeof(u->my_addr);

    if (u->mode == UDP_SERVER) {
        if ((r = recvfrom(u->s, buf, len, 0, (struct sockaddr*)u->inp_addr, &slen)) == -1)
    	    fprintf(stderr, "recvfrom()\n");
#ifdef DEBUG
    	else
    	    fprintf(stderr, "Received packet from %s:%d size %d\n", inet_ntoa(u->inp_addr->sin_addr), ntohs(u->inp_addr->sin_port), r);
#endif
	*u->out_addr = *u->inp_addr;
    } else {
        if ((r = recvfrom(u->s, buf, len, 0, (struct sockaddr*)&u->my_addr, &slen))==-1)
	    fprintf(stderr, "recvfrom()\n");
#ifdef DEBUG
        else
	    fprintf(stderr, "Received packet from %s:%d size %d\n", inet_ntoa(u->my_addr.sin_addr), ntohs(u->my_addr.sin_port), r);
#endif
    }

    return r;
}

int udp_write(udp_channel *u, uint8_t *buf, size_t len)
{
    int r;
    socklen_t slen = sizeof(u->my_addr);

    if (u->mode == UDP_SERVER) {
	if ((r = sendto(u->s, buf, len, 0, (struct sockaddr*)u->out_addr, slen)) < 0)
	    fprintf(stderr, "sendto()\n");
    } else {
	if ((r = sendto(u->s, buf, len, 0, (struct sockaddr*)&u->my_addr, slen)) == -1)
	    fprintf(stderr, "sendto()\n");
    }

    return r;
}

/*
 *
 */

int udp_read_src(udp_channel *u, uint8_t *buf, size_t len)
{
    int r;
    socklen_t slen = sizeof(u->my_addr);

    if (u->mode == UDP_SERVER) {
        if ((r = recvfrom(u->s, buf, len, 0, (struct sockaddr*)u->inp_addr, &slen)) == -1)
    	    fprintf(stderr, "recvfrom()\n");
#ifdef DEBUG
    	else
    	    fprintf(stderr, "Received packet from %s:%d size %d\n", inet_ntoa(u->inp_addr->sin_addr), ntohs(u->inp_addr->sin_port), r);
#endif
    } else {
        if ((r = recvfrom(u->s, buf, len, 0, (struct sockaddr*)&u->my_addr, &slen))==-1)
	    fprintf(stderr, "recvfrom()\n");
#ifdef DEBUG
        else
	    fprintf(stderr, "Received packet from %s:%d size %d\n", inet_ntoa(u->my_addr.sin_addr), ntohs(u->my_addr.sin_port), r);
#endif
    }

    return r;
}

void udp_commit_dst(udp_channel *u)
{
    if (u->mode == UDP_SERVER)
	*u->out_addr = *u->inp_addr;
}

int udp_forward_add(udp_channel *u, char *label)
{
    if (u->mode != UDP_SERVER)
	return 0;

    udp_forward *fwd = u->forward;
    udp_forward *prev = NULL;
    while (fwd) {
	if (!strncmp(fwd->label, label, 13)) {
	    fwd->used++;
	    return 0;
	}
	prev = fwd;
	fwd = fwd->next;
    }

    fwd = (udp_forward *) malloc(sizeof(udp_forward));
    if (!fwd)
	return -1;
    fwd->addr = *u->inp_addr;
    fwd->label = strdup(label);
    fwd->used = 1;
    fwd->total = 0;
    fwd->next = NULL;

    if (prev)
	prev->next = fwd;
    else
	u->forward = fwd;

#ifdef DEBUG
    fprintf(stderr, "Added forward for %s\n", fwd->label);
#endif

    return 0;
}

int udp_forward_write(udp_channel *u, char *label, uint8_t *buf, size_t len)
{
    if (u->mode != UDP_SERVER)
	return 0;

    udp_forward *fwd = u->forward;
    while(fwd) {
	if (!strncmp(fwd->label, label, 13)) {
	    socklen_t slen = sizeof(fwd->addr);
	    int r;
#ifdef DEBUG
	    fprintf(stderr, "Forward to %s\n", fwd->label);
#endif
	    if ((r = sendto(u->s, buf, len, 0, (struct sockaddr*)&fwd->addr, slen)) < 0)
		fprintf(stderr, "sendto()\n");

	    fwd->total += len;

	    return r;
	}
	fwd = fwd->next;
    }
#ifdef DEBUG
    fprintf(stderr, "No forward destination!\n");
#endif
    return -1;
}

void udp_forward_show(udp_channel *u)
{
    udp_forward *fwd = u->forward;
    if (!fwd)
	return;
    fprintf(stderr, "-- UDP forward table --\n");
    while (fwd) {
	fprintf(stderr, "%s %s:%d %d\n", fwd->label, inet_ntoa(fwd->addr.sin_addr), ntohs(fwd->addr.sin_port), fwd->total);
	fwd = fwd->next;
    }
    fprintf(stderr, "-----------------------\n");
}

void udp_forward_remove_inactive(udp_channel *u)
{
    udp_forward *fwd = u->forward;
    udp_forward *prev = NULL;
    if (!fwd)
	return;
    while (fwd) {
	if (!fwd->used) {
	    if (prev)
		prev->next = fwd->next;
	    else
		u->forward = fwd->next;
	    free(fwd->label);
	    free(fwd);
	    if (prev)
		fwd = prev->next;
	    else
		fwd = u->forward;
	    continue;
	}
	fwd->used = 0;
	prev = fwd;
	fwd = fwd->next;
    }
}
