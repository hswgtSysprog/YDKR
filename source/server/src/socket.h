/*
 * socket.h
 *
 *  Created on: 16.05.2011
 *      Author: rainer
 */

#ifndef SOCKET_H_
#define SOCKET_H_

/**
 * functions
 */
static void echo_loop(int fd);
void* echo_thread(void* param);
void accept_loop(int fd[], int numfd);

#endif /* SOCKET_H_ */
