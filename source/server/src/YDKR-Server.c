/*
 ============================================================================
 Name        : YDKR-Server.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : 12.04.2011 - 16:46:20
 Project     : YDKR-Server
 Description : Server fuer das Spiel "You dont know Rainer"
 ============================================================================
 */


#include "YDKR-Server.h"

/**
 * struct to pass custom data to echo_thread
 **/
struct client_data {
	int sock;
	struct sockaddr_storage addr;
	socklen_t addrlen;
};

/**
 * Copy all data from fd -> fd
 * using a buffer of 512 Byte
 **/
static void echo_loop(int fd)
{
	int ret;
	static char buf[512];

	while (1) {
		ret = read(fd, buf, sizeof(buf));
		if (ret == 0) {
			break;
		}
		if (ret < 0) {
			perror("read");
			break;
		}
		if (write(fd, buf, ret) < ret) {
			perror("write");
			break;
		}
	}
};

/**
 * Thread to handle connection in background and run echo_loop
 * param: struct client_data*
 **/
void* echo_thread(void* param)
{
	/**
	 * IPV6-faehig
	 */
	char dst[INET6_ADDRSTRLEN];
	struct client_data * data;
	data = (struct client_data*)param;

	/* RTFM: getnameinfo */
	getnameinfo((struct sockaddr*)&data->addr,
			data->addrlen,
			dst,
			sizeof(dst),
			NULL,
			0,
			NI_NUMERICHOST);

	printf("Connection opened from %s\n",dst);
	echo_loop(data->sock);
	close(data->sock);
	printf("Connection closed from %s\n", dst);

	free(data);

	pthread_exit(0);
	return NULL;
};

/**
 * Wait for connection on all available sockets
 * fd: array of sockets in listen state
 * numfd: number of valid sockets in fd
 **/
void accept_loop(int fd[], int numfd)
{
	fd_set set;
	int max, i, ret;

	if (numfd < 1) {
		printf("No sockets available!\n");
		return;
	}
	while (1) {
		max = 0;
		FD_ZERO(&set);
		for (i=0; i<numfd; i++) {
			if (fd[i] > max)
				max = fd[i];
			FD_SET(fd[i], &set);
		}

		/* wait for first fd that has data */
		ret = select(max+1, &set, NULL, NULL, NULL);
		if (ret <= 0) {
			perror("select");
			return;
		}

		for (i=0; i<numfd; i++)
			if (FD_ISSET(fd[i], &set)) {

				/**
				 * listen - receive
				 */
				static char buf[512];
				recv(fd[i], &buf, sizeof(buf), MSG_WAITALL);

				struct client_data *data;

				data = (struct client_data*)malloc(sizeof(struct client_data));

				data->addrlen = sizeof(data->addr);
				data->sock = accept(fd[i], (struct sockaddr*) &data->addr, &data->addrlen);

				if (data->sock < 0) {
					perror("accept");
					free(data);
				} else {
					pthread_t thread_id;
					/* Background new connection */
					pthread_create(&thread_id, NULL, echo_thread, data);
				}
			}
	}
};

int main(int argc, char ** argv)
{
	/**
	 * service = Port | "http"
	 */
	char *server = "0.0.0.0";
	char *service;

	/**
	 * Anzahl der moeglichen Socketverbindungen
	 */
	int sockets[128];

	/**
	 * Struct fuer hints, addrinfo und (running) process (?)
	 */
	struct addrinfo hints, *addr_info, *p;

	/**
	 * ret: return-Wert von getaddrinfo
	 */
	int ret;

	/**
	 * Anzahl der offenen Sockets
	 */
	int numsockets = 0;
	int i;

	/**
	 * @todo default port?
	 */
	if (argc == 3)
	{
		/**
		 * Port = argv[2]
		 */
		service = argv[2];
	}
	else
	{
		/**
		 * Bei Aufruf ohne Argumente
		 */
		printf("Simple Echo-Server\nUsage: %s [BINDADDR] [SERVICE]\n", argv[0]);
		printf("\nExample: %s localhost 8080\n", argv[0]);
		exit(1);
	}

	/**
	 * Mit 0 initialisieren
	 */
	memset(&hints, 0, sizeof(hints));

	/**
	 * Kriterien fuer getaddrinfo()
	 */
	hints.ai_family = AF_UNSPEC; /* Protokollfamilie: IPV4 oder IPV6 oder unspecified */
	hints.ai_socktype = SOCK_STREAM; /* Socket-Typ: verbindungsorientiert */
	hints.ai_protocol = IPPROTO_TCP; /* Protokoll: TCP */
	hints.ai_flags = AI_PASSIVE | AI_V4MAPPED; /* IPV4 Adressen werden angenommen */

	ret = getaddrinfo(server, service, &hints, &addr_info);

	/**
	 * alles ausser 0 ist ein Fehler
	 */
	if (ret)
	{
		printf("===>>> ERROR: \n");
		printf("getaddrinfo: %s\n", gai_strerror(ret));
		exit(1);
	}

	/**
	 * @todo ab hier kommentieren
	 */
	p = addr_info;
	while(p)
	{
		int s; // socket
		int on = 1;
		char dst[INET6_ADDRSTRLEN];
		char service[INET6_ADDRSTRLEN];

		/**
		 * Socket erstellen
		 */
		s = socket(p->ai_family, p->ai_socktype, 0);

		/**
		 * Socketoptionen setzen
		 */
		if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 )
		{
			perror("setsockopt"); /* maybe not so fatal, continue */
		}
		/**
		 * IPV6-spezifisch
		 */
		if(p->ai_family == AF_INET6)
		{
			if(setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)) < 0)
			{
				perror("setsockopt");
			}
		}

		/**
		 * Konvertiert Socketadresse zu Name und Service
		 */
		getnameinfo(p->ai_addr,
					p->ai_addrlen,
					dst,
					sizeof(dst),
					service,
					sizeof(service),
					NI_NUMERICHOST | NI_NUMERICSERV);

		printf("Trying %s:%s ... ",dst, service);
		fflush(stdout);

		/**
		 * "assigning a name to a socket"
		 */
		if(bind(s, p->ai_addr, p->ai_addrlen) == 0)
		{
			if(listen(s, 1) < 0)
			{
				perror("listen");
				close(s);
			}
			else
			{
				/**
				 * Anzahl der Nummer der Sockets erhoehen
				 */
				printf("bind successful\n");
				sockets[numsockets++] = s;
			}
		}
		else
		{
			/**
			 * binding fehlgeschlagen
			 */
			perror("bind");
			close(s);
		}

		p = p->ai_next;
	}

	freeaddrinfo(addr_info);
	printf("Waiting for connections...\n\n");
	/**
	 * Eigentliche Schleife die auf Verbindungen wartet
	 */
	accept_loop(sockets, numsockets);

	for(i = 0; i < numsockets; i++)
	{
		close(sockets[i]);
	}
	return 0;
}
