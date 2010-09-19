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
#include <sys/types.h>
#include <unistd.h>

#include "Socket.h"
#include "Bind.h"
#include "Listen.h"
#include "Accept.h"
#include "Read.h"
#include "Write.h"
#include "Close.h"
#include "Connect.h"
#include "Select.h"
#include "Recv.h"

int main(int argc, char *argv[]) {
	char buf[512];
	int server_socket;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;
	server_socket = Socket(AF_INET, SOCK_STREAM, 0);
	Remote_Address.sin_family=AF_INET;
	hp = gethostbyname(argv[1]);
	fd_set readfds;
	int bytes;
	int i,j,k;
	
	memcpy((unsigned char *) &Remote_Address.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	Remote_Address.sin_port=htons(atoi(argv[2]));

	Connect(server_socket, (struct sockaddr *)&Remote_Address, sizeof(Remote_Address));
	//printf("You are now connected to the server. Type a line to see it echoed back.\n");
	while (1) {
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		FD_SET(server_socket, &readfds);
		bzero(buf,sizeof(buf));

		i=Select(16, &readfds, 0, 0, 0);
	
		// check to see if the server bit is being set, and read from it
		if (FD_ISSET(server_socket, &readfds)) {
			bytes=Recv(server_socket,buf,512,0);
			// echo this back
			if (!strcmp(buf, "quit")) {
				printf("Shutting down.\n");
				exit(0);
			}
			printf("Message from server: %s\n", buf);
			if (bytes<=0) {
				break;
			}
		}
	
		// check to see if the stdin is being set, and read from it
		if (FD_ISSET(0, &readfds)) {
			bytes=Read(fileno(stdin),buf,512);
			if (bytes<=0) {
				break;
			}
			else {
				//printf("You just typed: %s\n", buf);
				buf[bytes]=0;
				Write(server_socket, buf, 512);
			}
		}
	}
	
	Close(server_socket);

	printf("Client: exit \n");
	exit(0);
}