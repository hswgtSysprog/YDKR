#ifndef CLIENT_H_
#define CLIENT_H_

/*============================================================================
 Name        : client.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : May 10, 2011 10:18:15 AM
 Project     : client
 =============================================================================
*/


#include "../../common/src/messages.h"

#define ERR_OOM -1
#define ERR_KILL_CLIENT -2


typedef struct
{
	int ID;
	char* name;
	int sock;
	unsigned long score;
	char* question;
}global_client_info;

global_client_info GCI;

void send_login(char* name);
int wait_loginOK();
void setClientMode();


#endif /*CLIENT_H_*/