#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "listener.h"
#include "client.h"
#include "gui.h"
#include "command.h"


void *listener_thread(void *data)
{
	t_msg_header hdr;
	int receiver=0;
	int parser=0;
	
	
	while(1)
	{
		receiver = recv(GCI.s, &hdr, sizeof(hdr), MSG_WAITALL);		
		if (receiver ==0 || receiver < sizeof(hdr))
		{
				raise(SIGINT);
		}	
		
		hdr.length = ntohs(hdr.length);
		parser = parse_msg();
	}
}

void parse_msg(t_msg_header *hdr)
{
    size_t ret =0;
    
    if(hdr->type == FOOBAR)
    {
      
    }
}
