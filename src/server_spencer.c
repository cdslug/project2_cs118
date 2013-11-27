/*
 * server_spencer.c
 *
 *  Created on: Nov 26, 2013
 *      Author: spencer
 */


#include <stdio.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>	/* for the waitpid() system call */
#include <signal.h>	/* signal name macros, and the kill() prototype */
#include <string.h>
#include <assert.h>
#include <time.h>

#define MAX_FILE_SIZE 1024
#define BUFSIZE 1024


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char * argv[]){
	int sockfd, portno; // sockfd:portno -> server socket
	int bytesrecv;  /* # bytes received */
	int msgcnt = 0; /* number of messages received */
	struct sockaddr_in serv_addr; // server address
	struct sockaddr_in cli_addr; // client address
	socklen_t clilen = sizeof(cli_addr);

	char buf[BUFSIZE]; // receive buffer

	if(argc < 2){
		error("ERROR, no port provided\n");
	}

	/* create UDP socket */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		error("ERROR, opening socket");
	}

	/* bind UDP socket */
	memset((char *) &serv_addr, 0, sizeof(serv_addr)); //clear memory for socket
	//fill in address info
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
	if((bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
		error("ERROR, binding socket");
	}

	while(1) {
		printf("waiting on port %d\n", portno);
		bytesrecv = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&cli_addr, &clilen);
		if(bytesrecv > 0) {
			buf[bytesrecv] = 0;
			printf("received message: %s\n", buf);
		}
		else
			printf("Error receiving message!\n");
		// echo file request
		printf("sending echo response \"%s\"\n", buf);
		if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)& cli_addr, clilen) < 0) {
			error("sendto");
		}
	}
	// never exits

}