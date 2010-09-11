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
	int server_socket, socket_to_client;
	int procid, tmp;
	int open_sockets[16];
	
	char buf[512];
	int msglength;
	
	// start up the passive socket
	
	// start listening
	
	// when the passive socket gets a buzz
	
	// bind that connection to another socket
	
	// put that socket in the array
	
	// if data comes from that socket
	
	// read and print it back
	
	// if the data is quit then close that socket/connection
	
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
		exit(0);
	}
}