/*
 * ============================================================================
 * Name        : main.c
 * Author      :Kathrin Holzmann
 * Version     : Jun 20, 2011 - 08:14:21 AM
 * Project     : client
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include "gui_interface.h"
#include "client.h"
#include "listener.h"
#include "fragewechsel.h"

void *fragen_thread(void *data)
{
	while(1)
	{
		if( sem_P(keymng_local(KEY_QUESTION)) == -1 ) { 
			if( errno == EIDRM ) return 0;
		}
		printf("HALLO ICH BIN HIER");
		send_QR(3);
		// sem_P(keymng_local(KEY_QUESTION));
	}        
}
