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

main(int argc, char *argv[]) {
	struct sockaddr_in from, server;
	struct hostent *hp;
	int my_socket, bytes;
	char buf[512];
	unsigned int fromlen;

	my_socket=Socket(AF_INET, SOCK_DGRAM,0);
	bzero((char*) &server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(atoi(argv[1]));
	
	Bind(my_socket, (struct sockaddr *) &server, sizeof(server));
	
	while(1) {
		fromlen=sizeof(from);
		bytes=recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &from, &fromlen);
		printf("SERVER: read %d bytes from IP %d (%s)\n", bytes, inet_ntoa(from.sin_addr), buf);
		printf("Sending the data back...\n");
		bytes=sendto(my_socket, buf, strlen(buf)+1, 0, (struct sockaddr *) &from, sizeof(from));
		if (!strcmp(buf, "quit")) {
			// Should send quit to the client
			break;
		}
	}
	close(my_socket);
}
