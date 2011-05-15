/*============================================================================
 Name        : command.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 10, 2011 12:30:08 AM
 Project     : source
 =============================================================================
*/

void *command_thread(void *data)
{
	MSG msg;
		uint16_t len;

		sem_P(keymng_local(KEY_GCS_SEM));
		send_login(GCS.s, GCS.name);
		sem_V(keymng_local(KEY_GCS_SEM));

		while(1) {
			msg = message_queue_receive_command();
			memcpy(&len, msg.text+1, sizeof(uint16_t));
			if(ntohs(len) > 1277) continue; /* sorry, but the message was lost in the mq -> don't send */
			sem_P(keymng_local(KEY_GCSSOCK_SEM));
			send(GCS.s, msg.text, ntohs(len) + sizeof(t_msg_header), 0);
			sem_V(keymng_local(KEY_GCSSOCK_SEM));
		}

		return NULL;
}

void send_login(int sock, char* name)
{
	char msg[MAX_MSG];
	t_msg_header hdr;

	bzero(msg, sizeof(msg));
	
	hdr.type = RFC_LOGIN;
	hdr.length = htons(strlen(name) + 1);
	
	memcpy(msg,
	       &hdr, sizeof(hdr));
	memcpy(msg + sizeof(hdr),
	       &role, sizeof(uint8_t));
	memcpy(msg + sizeof(hdr) + sizeof(uint8_t),
	       name, strlen(name));

	message_queue_send_command(msg);

}

void choose_questions()
{
  //do something great
  return 0;
}

void start_game()
{
  //do something great
  return 0;
  
}

void send_answer()
{
  //do something great
  return 0;
}

void end_game()
{
  //do something great
  return 0;
}

void shutdown()
{
  //do something great
  return 0;
}
  
