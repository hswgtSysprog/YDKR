#ifndef COMMON_CLINET_H
#define COMMON_CLIENT_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include "common.h"

struct client_info {
	int sock;
	pthread_t thread;

	char     *name;
	uint16_t client_id;

	int first_send;
};


typedef struct client_info t_client_info;
typedef t_client_info* tp_client_info;

#endif
