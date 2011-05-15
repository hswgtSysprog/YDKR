/*============================================================================
 Name        : mq.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 10, 2011 12:25:25 AM
 Project     : source
 =============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "keymanager.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "mq.h"

int message_queue_open(key_t key) {
	int id;

	id = msgget(key, 0);    /* öffnet message queue */
	if(id == -1)                    /* message queue existiert noch nicht */
		id = msgget(key, IPC_CREAT | 0666 ); /* message queue anlegen */

	if(id == -1) {
		fprintf(stderr, "Fehler %d: konnte Messagequeue nicht anlegen!\n", errno);
		exit(-42);
	}

	return id;
}

void message_queue_remove(key_t key) {
	int id = message_queue_open(key);
	struct msqid_ds dummy;
	if(msgctl(id, IPC_RMID, &dummy) < 0)
		fprintf(stderr, "Error removing the mq!");
}


int message_queue_send(key_t key, char *text) {
	int queue;
	MSG2LOGGER msg;

	if(text == NULL) return 0;

	msg.mtype = 1;
	memcpy(msg.text, text, MAX_MSG); /* this does not work with the server ! */

	queue = message_queue_open(key);
	return msgsnd(queue, &msg, sizeof(msg), 0);
}


MSG2LOGGER message_queue_receive(key_t key) {
	MSG2LOGGER msg;
	int queue, length;

	queue = message_queue_open(key);
	length = msgrcv(queue, &msg, sizeof(msg), 1, 0);
	if(length == -1) {
		fprintf(stderr, "MQ Read Abort\n");
		raise(SIGINT);
	}
	return msg;
}

int message_queue_send_command(char *text) {
	return message_queue_send(keymng_local(KEY_COMMAND_MQ), text);
}

MSG2LOGGER message_queue_receive_command() {
	return message_queue_receive(keymng_local(KEY_COMMAND_MQ));
}