/*
 ============================================================================
 Name        : YDKR-Server.h
 Author      : Rainer Hihn, Kathrin Holzmann, Florian Rosenkranz
 Version     : 12.04.2011 - 17:01:43
 Project     : YDKR-Server
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
 * struct to pass custom data to echo_thread
 **/
struct client_data {
	int sock;
	struct sockaddr_storage addr;
	socklen_t addrlen;
};


/**
 * Copy all data from fd -> fd
 * using a buffer of 512 Byte
 **/
static void echo_loop(int fd)
{
	int ret;
	static char buf[512];

	while (1) {
		ret = read(fd, buf, sizeof(buf));
		if (ret == 0) {
			break;
		}
		if (ret < 0) {
			perror("read");
			break;
		}
		if (write(fd, buf, ret) < ret) {
			perror("write");
			break;
		}
	}
}

/**
 * Thread to handle connection in background and run echo_loop
 * param: struct client_data*
 **/
void* echo_thread(void* param)
{
	char dst[INET6_ADDRSTRLEN];
	struct client_data * data;
	data = (struct client_data*)param;

	/* RTFM: getnameinfo */
	getnameinfo((struct sockaddr*)&data->addr,
			data->addrlen,
			dst,
			sizeof(dst),
			NULL,
			0,
			NI_NUMERICHOST);

	printf("Connection opened from %s\n",dst);
	echo_loop(data->sock);
	close(data->sock);
	printf("Connection closed from %s\n", dst);

	free(data);

	pthread_exit(0);
	return NULL;
}

/**
 * Wait for connection on all available sockets
 * fd: array of sockets in listen state
 * numfd: number of valid sockets in fd
 **/
void accept_loop(int fd[], int numfd)
{
	fd_set set;
	int max, i, ret;

	if (numfd < 1) {
		printf("No sockets available!\n");
		return;
	}
	while (1) {
		max = 0;
		FD_ZERO(&set);
		for (i=0; i<numfd; i++) {
			if (fd[i] > max)
				max = fd[i];
			FD_SET(fd[i], &set);
		}

		/* wait for first fd that has data */
		ret = select(max+1, &set, NULL, NULL, NULL);
		if (ret <= 0) {
			perror("select");
			return;
		}
		for (i=0; i<numfd; i++)
			if (FD_ISSET(fd[i], &set)) {
				struct client_data *data;

				data = (struct client_data*)malloc(sizeof(struct client_data));

				data->addrlen = sizeof(data->addr);
				data->sock = accept(fd[i], (struct sockaddr*) &data->addr, &data->addrlen);

				if (data->sock < 0) {
					perror("accept");
					free(data);
				} else {
					pthread_t thread_id;
					/* Background new connection */
					pthread_create(&thread_id, NULL, echo_thread, data);
				}
			}
	}
}

#endif /* YDKRSERVER_H_ */
