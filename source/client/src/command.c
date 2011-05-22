/*============================================================================
 Name        : command.c
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 10, 2011 12:30:08 AM
 Project     : source
 =============================================================================
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include "command.h"
#include "gui_interface.h"

void *command_thread(void *data)
{
	
	printf("huuhu \n");
	sendCR();
	return 0;
}

/*-----------void sendCR()---------------------------
@description: sends the Catalouge Request
-----------------------------------------------------------*/

void sendCR()
{
  
  t_msg_header hdr;
  hdr.type = RFC_CATALOGREQUEST;
  printf("cr: %i \n", hdr.type);
    //umdrehen <=16 Bit werte
  hdr.length = 0;	
  send(GCI.sock, &hdr, sizeof(hdr), 0);
  printf("CR was send \n"); 
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
  /*
   * variable anlgegen, header typ ist RFC_QUESTIONANSWERED
   * 
   *
  
  if (write(GCI.sock, &hdr, ret) < ret)
	{
		perror("write");
		break;
	}*/
} 

void end_game()
{
  //do something great
}

void shutdown_game()
{
  //do something great
}
  
