/*
 ============================================================================
 Name        : YDKR-Server.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : 12.04.2011 - 17:01:43
 Project     : YDKR-Server
 Description : Header-Datei fuer den Server von YDKR
 ============================================================================
 */

#ifndef YDKRSERVER_H_
#define YDKRSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>


/**
 * structs
 */
struct client_data;

/**
 * functions
 */
static void echo_loop(int fd);
void* echo_thread(void* param);
void accept_loop(int fd[], int numfd);

#endif /* YDKRSERVER_H_ */
