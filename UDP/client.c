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

main(int argc, char *argv[]) {
	
	// initialize
	int my_socket;
	char buf[512];
	int bytes;
	struct sockaddr_in server, from;
	unsigned int fromlen;
	struct hostent *hp;
	
	// send broadcast to potential servers
	// get connection back
	my_socket=Socket(AF_INET, SOCK_DGRAM,0);
	hp=gethostbyname(argv[1]);
	server.sin_family=AF_INET;
	memcpy((unsigned char *) &server.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	server.sin_port=htons(4096);
	
	// send/receive message
	// TODO write a sendto thingo
	// TODO write a recvfrom
	bytes=sendto(my_socket, argv[2], strlen(argv[2])+1, 0, (struct sockaddr *) &server, sizeof(server));
	
	fromlen=sizeof(from);
	bytes=recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &from, &fromlen);
	close(my_socket);
}