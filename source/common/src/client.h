#ifndef COMMON_CLINET_H
#define COMMON_CLIENT_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>

/**
 * struct das die spielerinfo beinhaltet
 */
struct client_info {
	int sock; // socket ueber den der spieler sich verbindet
	pthread_t thread; // der client thread auf dem server
	char *name; // eindeutiger spielername
	uint8_t client_id; // eindeutige client id
};

typedef struct client_info t_client_info;
typedef t_client_info* tp_client_info;

#endif
