// CS-435
// Evan Travers

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
#include <sys/types.h>
#include <unistd.h>

#include "Socket.h"
#include "Bind.h"
#include "Recvfrom.h"
#include "Close.h"
#include "Sendto.h"

// basic UDP client, returns everything it is sent until is sent a 'quit' message
main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./client <port> \n");
		exit(0);
	}
	
	struct sockaddr_in client;
	struct hostent *hp;
	int my_socket, bytes;
	char buf[512];
	unsigned int fromlen;

	my_socket=Socket(AF_INET, SOCK_DGRAM,0);
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=htonl(INADDR_ANY);
	client.sin_port=htons(atoi(argv[1]));
	
	Bind(my_socket, (struct sockaddr *) &client, sizeof(client));
	
	while(1) {
		bzero(buf, sizeof(buf));
		fromlen=sizeof(client);
		bytes=Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &client, &fromlen);
		printf("SERVER: read %d bytes from client: (%s)\n", bytes, buf);
		printf("Sending the data back...\n");
		bytes=Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *) &client, sizeof(client));
		if (!strcmp(buf, "quit")) {
			printf("Quitting!\n");
			exit(0);
		}
	}
	Close(my_socket);
}
