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
#include "Recv.h"
#include "Select.h"

main(int argc, char *argv[]) {
	struct sockaddr_in Server_Address_Passive;
	int passive_socket, socket_to_client;
	int procid, tmp;
	int open_sockets[16];
	
	char buf[512];
	int msglength;
	
	fd_set readfds;
	int bytes, i, des_pnt, counter=0;
	
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
		i=Select(16, &readfds, 0, 0, 0);

		if (FD_ISSET(passive_socket, &readfds)) {
			// when the passive socket gets a buzz
			printf("A client has connected.\n");
			
			// bind that connection to another socket
			socket_to_client=Accept(passive_socket, 0, 0);
			// CHANGED make the array add wherever there isn't a null or -1
			
			// put that socket in the array
			for (counter = 0; counter < 16; ++counter) {
				if (open_sockets[counter]==0) {
					open_sockets[counter]=socket_to_client;
					break;
				}
			}
		}
		
		// for each descriptor in the array
		for (des_pnt = 0; des_pnt < 16; des_pnt++) {
			if (open_sockets[des_pnt]!=0&&open_sockets[des_pnt]!=-1) {
				if (FD_ISSET(open_sockets[des_pnt], &readfds)) {
					// read what the client has to say
					bytes=Recv(des_pnt,buf,512,0);
					if (bytes<=0) {
						break;
					}
					printf("Message received from client: %s\n",buf);
					
					// check that for data
					// if there is data, read it and write it back

					// if that data is end, then close the connection cleanly
					
					// if that data is quit, close all connections and exit itself
				}
			}
		}
	}
	exit(0);
}