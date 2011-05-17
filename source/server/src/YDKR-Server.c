/*
 ============================================================================
 Name        : YDKR-Server.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 Description : Server fuer das Spiel "You dont know Rainer"
 ============================================================================
 */


#include "YDKR-Server.h"
#include "socket.h"

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
