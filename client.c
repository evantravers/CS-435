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

int main(int argc, char *argv[]) {
	char buf[512];
	int client_socket;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;
	client_socket = Socket(AF_INET, SOCK_STREAM, 0);
	Remote_Address.sin_family=AF_INET;
	hp = gethostbyname(argv[1]);
	
	memcpy((unsigned char *) &Remote_Address.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	Remote_Address.sin_port=htons(atoi(argv[2]));

	int e_con = Connect(client_socket, (struct sockaddr *)&Remote_Address, sizeof(Remote_Address));
	
	int e_wri = Write(client_socket, "ping", 5);
	
	int e_rea = Read(client_socket, buf, 512);
	
	// check to see if the strings match
	if (strcmp(buf, "pong")) {
		printf("server string does not match pong\n");
	}
	else {
		printf("Client: message from server: %s \n", buf);
	}
	
	int e_clo = Close(client_socket);

	printf("Client: exit \n");
	exit(0);
}