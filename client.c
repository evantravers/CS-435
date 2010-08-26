#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char buf[512];
	int client_socket;
	struct sockaddr_in Remote_Address;
	struct hostent *hp;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	Remote_Address.sin_family=AF_INET;
	// TODO handle gethostbyname
	hp = gethostbyname(argv[1]);
	// TODO memcpy thing
	memcpy((unsigned char *) &Remote_Address.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	Remote_Address.sin_port=htons(10214);
	// TODO handle the connect
	connect(client_socket, (struct sockaddr *)&Remote_Address, sizeof(Remote_Address));
	// TODO handle the write
	write(client_socket, "ping", 5);
	// TODO handle the read
	read(client_socket, buf, 512);
	printf("Client: message from server: %s \n", buf);
	// TODO handle the close
	close(client_socket);
	printf("Client: exit \n");
	exit(0);
}