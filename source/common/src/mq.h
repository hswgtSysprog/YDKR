/*
 * keymanager.h
 *
 *  Created on: May 10, 2011
 *      Author: kathrin
 */

#ifndef MQ_H
#define MQ_H

#include "keymanager.h"

#define MAX_MSG 1280
#define MAX_LOGGER_MSG 1280

#define LOG(x) message_queue_send_logger(x)

typedef struct {
	long mtype;
	char text[MAX_MSG];
} MSG;

typedef MSG MSG2LOGGER;

int message_queue_open(key_t key);
void message_queue_remove(key_t key);


int message_queue_send(key_t key, char *msg);
MSG2LOGGER message_queue_receive(key_t key);

int message_queue_send_logger(char *msg);
MSG2LOGGER message_queue_receive_logger();

int message_queue_send_command(char *msg);
MSG2LOGGER message_queue_receive_command();



#endif /* MQ_H */
