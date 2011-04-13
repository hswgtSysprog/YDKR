/*
 ============================================================================
 Name        : main.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : Apr 12, 2011 - 10:44:39 AM
 Project     : YDKR-Client
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// hauptfunktion erwartet server und portnummer
int main(int argc, char ** argv)
{
	struct addrinfo *addr_info, *p, hints;
	int ret;
	//zu wenig parameter
	if(argc<=2)
	{

		printf("Simple Echo-Client\nUsage: %s [SERVER] [PORT]\n", argv[0]);
		printf("\nExample: %s www.sixxs.net 80\n", argv[0]);
		exit(1);
	}

	//hinweise zur verbindung setzen
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0 /* | AI_ADDRCONFIG */;

	/* RTFM: getaddrinfo */
        ret = getaddrinfo(argv[1], argv[2], &hints, &addr_info);
        if (ret)
	{
		printf("getaddrinfo: %s\n", gai_strerror(ret));
		exit(1);
	}

	printf("\n");
        p = addr_info;

    while (p)
    {
       int s;
       char dst[INET6_ADDRSTRLEN];

       	// neuen socket erstellen
        s = socket(p->ai_family, p->ai_socktype, 0);

       /* RTFM: getnameinfo */
       getnameinfo(p->ai_addr,
       p->ai_addrlen,
		   dst,
		   sizeof(dst),
		   NULL,
		   0,
		   NI_NUMERICHOST);

       printf("Trying %s ... ",dst);

       fflush(stdout);

       		//verbindungsversuch
           if (connect(s, p->ai_addr, p->ai_addrlen) == 0)
           {
        	   printf("Connected\n");

        	   send(s,"hello world\n", sizeof("hello world\n"),0);

       			//socket verbindung schließen
       			close(s);
       			printf("Closed\n");
       			break;
            } else
            {
            // fehler
       			perror("Verbindung konnte nicht aufgebaut werden");
       		}
       		close(s);

       p = p->ai_next;
    }
    freeaddrinfo(addr_info);

   	return 0;
}