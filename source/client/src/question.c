/*
 ============================================================================
 Name        : question.c
 Author      : Kathrin Holzmann
 Version     : May 25, 2011 
 Project     : client
 ============================================================================
 */

#include <stdio.h>
#include "gui_interface.h"
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
#include "gui_interface.h"
#include "client.h"
#include "command.h"
#include "listener.h"
#include <getopt.h>

void *question_thread(void *data)
{
    int receive =-1;
    
    while(receive!=0)
    {
       send_QR();
       receive=getQuestion();
      //waitAnswer
       //semaphor 
    }
}

/*
 ========================================================================
 Funktion:
        send_QR
 Kurzbeschreibung:
        Fordert die nächste Frage an.
 ========================================================================
 */

void send_QR()
{
  t_msg_header hdr;
  hdr.type = RFC_QUESTIONREQUEST;
  printf("cr: %i \n", hdr.type);
    //umdrehen <=16 Bit werte
  hdr.length = 0;       
  send(GCI.sock, &hdr, sizeof(hdr), 0);
  printf("QR was send \n"); 
}

int getQuestion()
{
  int receiver, ret, i;
  t_msg_header hdr;
  char* frage = malloc(256);
  char* antwort;
  receiver = recv(GCI.sock, &hdr, sizeof(hdr), MSG_WAITALL);
  // timer fuer timeout hier setzen
  
  
  if(receiver == 0 || receiver < sizeof(hdr)) 
  {
    printf("message wrong size \n");
    return -1;
  }
  
   printf("hdr length erstes: %d \n",hdr.length);
  
  hdr.length = ntohs(hdr.length);
  
  //is it what whe want?
  if(hdr.type != RFC_QUESTION)
  {
    printf("message wrong type \n");
    return -1;
  }
//receive rest of package and save the getted client ID
  ret = recv(GCI.sock, frage, hdr.length, MSG_WAITALL);
  printf("Frage: %s",frage);
 //did we receive great things?
  
  for(i=0; i<4; i++)
  {
      antwort=malloc(128);
            ret= recv(GCI.sock, antwort, 128, MSG_WAITALL);
   }
  if(ret == 0 || ret < hdr.length)
  {
    printf("rest vom paket falsch \n");
      return -1;
  } 
 return 0;
    
}
