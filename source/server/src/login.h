/*
 ============================================================================
 Name        : YDKR-Server.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Project     : YDKR-Server
 Description : Header-Datei fuer den Server von YDKR
 ============================================================================
 */

#ifndef LOGIN_H_
#define LOGIN_H_

// Systemdateien
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// lokale Dateien
#include "../../common/src/network.h"

// Functions
void *loginThread(int service);

#endif /* LOGIN_H_ */
