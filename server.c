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
	int bytes, i, j, counter=0;
	
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
		bzero(buf,sizeof(buf));

		// update readfds based on open_sockets
		for (i = 0; i < 16; ++i) {
			if (open_sockets[i]!=0&&open_sockets[i]!=-1) {
				FD_SET(open_sockets[i], &readfds);
			}
		}

		// start up the select
		Select(16, &readfds, 0, 0, 0);

		if (FD_ISSET(passive_socket, &readfds)) {			
			// bind that connection to another socket
			socket_to_client=Accept(passive_socket, 0, 0);	
			
			// put that socket in the array
			for (counter = 0; counter < 16; ++counter) {
				if (open_sockets[counter]==0) {
					open_sockets[counter]=socket_to_client;
					// when the passive socket gets a buzz
					printf("A client %d has connected.\n", counter);
					break;
				}
			}
		}
		
		// for each descriptor in the array
		for (i = 0; i < 16; i++) {
			if (open_sockets[i]!=0&&open_sockets[i]!=-1) {
				if (FD_ISSET(open_sockets[i], &readfds)) {
					// read what the client has to say
					bytes=Recv(open_sockets[i],buf,512,0);
					if (bytes<=0) {
						break;
					}
					else {
						printf("Client %d sent %s\n", i, buf);
						// check for killswitch
							if (!strcmp(buf, "quit\n")) {
								// if that data is quit, close all connections and exit itself
								for (j = 0; j < 16; ++j) {
									Write(open_sockets[j], "Goodbye!\n", 512);
									Close(open_sockets[j]);
									printf("Closing all connections and exiting.\n");
									exit(0);
								}
							}
							if (!strcmp(buf, "end\n")) {
								// if that data is end, then close the connection cleanly
								Write(open_sockets[i], "Goodbye!\n", 512);
								Close(open_sockets[i]);
								open_sockets[i]=0;
							}
							
						// if there is data, read it and write it back
						Write(open_sockets[i], buf, 512);
					}
				}
			}
		}
	}
	exit(0);
}