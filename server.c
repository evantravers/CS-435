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

// TODO move all the system call error handlers to wrappers
// CHANGED handle the system call error handling
// don't forget to use perror()

main(int argc, char *argv[]) {
	struct sockaddr_in Server_Address_Passive;
	int server_socket, socket_to_client;
	int procid, tmp;
	
	// CHANGED get the port num
	
	char buf[512];
	int msglength;
	// CHANGED take care of Socket
	server_socket=Socket(AF_INET,SOCK_STREAM,0);
	Server_Address_Passive.sin_family=AF_INET;
	Server_Address_Passive.sin_addr.s_addr=htonl(INADDR_ANY);
	Server_Address_Passive.sin_port=htons(atoi(argv[1]));
	// CHANGED take care of bind
	int e_bin = Bind(server_socket, (struct sockaddr *) &Server_Address_Passive, sizeof(struct sockaddr));

	// CHANGED take care of listen
	int e_lis = Listen(server_socket,5);
	
	while (1) {
		
		// CHANGED take care of accept
		socket_to_client=Accept(server_socket, 0, 0);
		
		// CHANGED check to see that the client sends the correct message.		
		// CHANGED take care of read
		msglength=Read(socket_to_client,buf,512);
		
		if (strcmp(buf, "ping")) {
			printf("string does not match ping\n");
		}
		else {
			printf("SERVER: msg from client: %s\n", buf);
		}
		
		// CHANGED take care of write
		int e_wri = Write(socket_to_client, "pong", 5);		
		
		// CHANGED take care of close
		int e_clo = Close(socket_to_client);
		
		exit(0);
	}
}