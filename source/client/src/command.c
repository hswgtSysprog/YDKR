/*============================================================================
 Name        : command.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 10, 2011 12:30:08 AM
 Project     : source
 =============================================================================
*/
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "../../common/src/mq.h"
#include "command.h"
#include "client.h"
void *command_thread(void *data)
{
	MSG msg;
	uint16_t len;
	send_login(GCI.name);

	while(1)
	{
		msg = message_queue_receive_command();
		memcpy(&len, msg.text+1, sizeof(uint16_t));
		if(ntohs(len) > 1277) continue; /* sorry, but the message was lost in the mq -> don't send */
		send(GCI.sock, msg.text, ntohs(len) + sizeof(t_msg_header), 0);
	}
	return NULL;
}

void send_login(char* name)
{
	char msg[MAX_MSG];
	t_msg_header hdr;

	bzero(msg, sizeof(msg));
	
	hdr.type = RFC_LOGINREQUEST;
	hdr.length = htons(strlen(name) + 1);
	
	memcpy(msg,
	       &hdr, sizeof(hdr));
	memcpy(msg + sizeof(hdr) + sizeof(uint8_t),
	       name, strlen(name));

	message_queue_send_command(msg);

}
	
void choose_questions()
{
  //do something great
}

void start_game()
{
  //do something great
}

void send_answer()
{
  //do something great
}

void end_game()
{
  //do something great
}

void shutdown_game()
{
  //do something great
}
  
