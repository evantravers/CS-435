// CS 435 Evan Travers

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include "Socket.h"
#include "Bind.h"
#include "Listen.h"
#include "Accept.h"
#include "Read.h"
#include "Write.h"
#include "Close.h"

main(int argc, char *argv[]) {
	struct sockaddr_in Server_Address_Passive;
	int passive_socket;
	int procid, tmp;
	int open_sockets[16];
	
	char buf[512];
	int msglength;
	
	fd_set readfds;
	int bytes, i;
	
	// set up the passive socket
	passive_socket=Socket(AF_INET,SOCK_STREAM,0);
	Server_Address_Passive.sin_family=AF_INET;
	Server_Address_Passive.sin_addr.s_addr=htonl(INADDR_ANY);
	Server_Address_Passive.sin_port=htons(atoi(argv[1]));
	
	int e_bin = Bind(passive_socket, (struct sockaddr *) &Server_Address_Passive, sizeof(struct sockaddr));
	
	// start listening
	int e_lis = Listen(passive_socket,5);
	
	while(1) {
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		FD_SET(passive_socket, &readfds);

		// start up the select
		i=select(16, &readfds, 0, 0, 0);

		if (FD_ISSET(passive_socket, &readfds)) {
			// when the passive socket gets a buzz
			
			// bind that connection to another socket

			// put that socket in the array
		}

		// for each descriptor in the array
		// check that for data
		// if there is data, read it and write it back
		
		// if that data is quit, then close the connection cleanly
/*
	server_socket=Socket(AF_INET,SOCK_STREAM,0);
	Server_Address_Passive.sin_family=AF_INET;
	Server_Address_Passive.sin_addr.s_addr=htonl(INADDR_ANY);
	Server_Address_Passive.sin_port=htons(atoi(argv[1]));

	int e_bin = Bind(server_socket, (struct sockaddr *) &Server_Address_Passive, sizeof(struct sockaddr));

	int e_lis = Listen(server_socket,5);
	
	while (1) {
		socket_to_client=Accept(server_socket, 0, 0);
	
		msglength=Read(socket_to_client,buf,512);
		
		if (strcmp(buf, "ping")) {
			printf("string does not match ping\n");
		}
		else {
			printf("SERVER: msg from client: %s\n", buf);
		}
		
		int e_wri = Write(socket_to_client, "pong", 5);		
		
		int e_clo = Close(socket_to_client);
*/
	}
	exit(0);
}