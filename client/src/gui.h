#ifndef GUI_H_
#define GUI_H_
/*============================================================================
 Name        : gui.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : Apr 25, 2011 8:55:11 AM
 Project     : client
 =============================================================================
*/

typedef struct
{
	int ID;
	char* name;
	int sock;
}global_client_info;

global_client_info GCI;
