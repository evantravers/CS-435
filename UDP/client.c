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
#include "Setsockopt.h"
#include "Sendto.h"
// TODO: fix the wrapper for recvfrom
// #include "Recvfrom.h"

main(int argc, char *argv[]) {
	
	// initialize
	int my_socket, on=1, off=0;
	struct sockaddr_in from, server;
	char buf[512];
	int bytes;
	unsigned int fromlen;
	
	my_socket=Socket(AF_INET, SOCK_DGRAM,0);
	
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, 4);
	inet_pton(AF_INET, "192.168.1.111", &server.sin_addr);
	bytes = Sendto(my_socket, argv[2], strlen(argv[2])+1, 0, (struct sockaddr *) &server, sizeof(server));
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &off,4);
	
	fromlen=sizeof(from);
	bytes=recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &from, &fromlen);
	printf("%s\n", buf	);
	
	close(my_socket);
	
	// send broadcast to potential servers
	
	// establish connection using the packet you receive back
	
	// close connection when finished.
}