/*
 ============================================================================
 Name        : YDKR-Server.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 Description : login ( thread )
 ============================================================================
 */

void *loginThread(void *data) {
	int login_sock;
	int client_sock;
	int port = *((int *)data);
	int ret;
	tp_client_info ci;


	login_sock = sock_open(port);
	if (login_sock == 0) {
		LOG("Socket open failed \n\n");
		perror("Socket open failed \n\n");
		raise(SIGINT);
	}


	while( 1 ) {
		if ((client_sock = sock_acct(login_sock)) < 0) {
			LOG("Login thread: Connection error... \n");
			raise(SIGINT);
			return NULL;
		}

		ci = (tp_client_info) malloc( sizeof( t_client_info ) );
		if(!ci) {
			sock_close(&client_sock);
			continue;
		}

		LOG("Login thread: Connection successfull... \n");

		ci->sock      = client_sock;
		ci->name      = NULL;
		ci->client_id = -1;
		ci->role      = RFC_ROLE_STUD;
		ci->first_send = 1;

		ret = pthread_create(&ci->thread, NULL, clientThread, ci);
		if (ret != 0) {
			sock_close(&client_sock);
			free(ci);
			continue;
		}
		pthread_detach(ci->thread);
	}

	return NULL;
}
