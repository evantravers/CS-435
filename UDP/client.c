// CS-435
// Evan Travers

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Socket.h"
#include "Setsockopt.h"
#include "Sendto.h"
#include "Close.h"
#include "Recvfrom.h"
#include "Bind.h"

main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: ./client <subnet mask (i.e. 255.255.255.0)> <port> <message> \n");
		exit(0);
	}
	
	// initialize
	int my_socket, on=1, off=0;
	struct hostent *hp;
	struct sockaddr_in server;
	char buf[512];
	int bytes;
	unsigned int fromlen;
	struct sockaddr_in listofservers[30];
	int server_count=0, more_servers=1;

	// initialize sockets	
	hp = gethostbyname(argv[1]);
	memcpy((unsigned char *) & server.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	
	my_socket = Socket(AF_INET, SOCK_DGRAM, 0);
	
	// enable broadcast
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr = *((struct in_addr *)hp->h_addr);
	server.sin_addr.s_addr|=htonl(0xff);
	
	// send the greeting
	bytes = Sendto(my_socket, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&server, sizeof(server));	
	printf("sent %d bytes to %s\n", bytes, inet_ntoa(server.sin_addr));
	
	// TODO detect multiple servers, add them to array
	while(more_servers==1) {
		
		fromlen = sizeof(server);
		bytes = Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &server, &fromlen);
		printf("Client: Found host at IP\t%s\nServer: %s\n", inet_ntoa(server.sin_addr), buf);
		// add to the array
		int i;
		for (i=0; i < server_count; ++i) {
			if (server.sin_addr.s_addr==listofservers[i].sin_addr.s_addr) {
				break;
			}
		}
		listofservers[server_count++]=server;
	}	
	
	// turn off broadcast
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(off));
	
	Close(my_socket);
}
