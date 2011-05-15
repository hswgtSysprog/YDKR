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
		receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);		
		if (receiver ==0 || receiver < sizeof(hdr))
		{
		 printf("Fehler");
		}	
		
		hdr.length = ntohs(hdr.length);
		parser = parse_msg(&hdr);
	}
}

int parse_msg(t_msg_header *hdr)
{    
    if(hdr->type == RFC_PLAYERLIST)
    {
	//Rangliste aktualisieren
	// void refresh_playerlist(datenrahmen);
    }
    else if(hdr->type == RFC_QUESTION)
    {
      //Frage anzeigen
      // 
    }
    else if (hdr->type == RFC_ERRORWARNING)
    {
      //Warnung ausgeben
    }
    
    return 0;
}
