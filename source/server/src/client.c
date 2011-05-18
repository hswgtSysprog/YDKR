/*
 ============================================================================
 Name        : client.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 ============================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "../../common/src/network.h"
// #include "../common/mq.h"
// #include "../common/sem.h"
// #include "shm.h"
// #include "clientlist.mod.h"
// #include "client.mod.h"

void client_cleanup(tp_client_info ci) {
	close(ci->sock);
	if(ci->client_id > -1)
		clientlist_remove(ci->client_id);
	free(ci->name);
	free(ci);
	pthread_exit(0);
}

void *clientThread(void *data) {
	t_msg_header hdr;
	tp_client_info ci = (tp_client_info) data;
	size_t ret = 0;
	int parse_ok = 0;

	while( 1 ) {
		ret = recv(ci->sock, &hdr, sizeof(hdr), MSG_WAITALL);
		if(ret == 0 || ret < sizeof(hdr)) {
			client_cleanup(ci);
		}

		hdr.length = ntohs(hdr.length);
		parse_ok = parse_msg(ci, &hdr);
		if(parse_ok == ERR_KILL_CLIENT) {
			client_cleanup(ci);
		}
	}
}
