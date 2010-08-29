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
	hp = gethostbyname(argv[1]);
	if (hp==NULL) {
		perror("gethostbyname has failed");
		exit(-1);
	}
	// CHANGED memcpy thing
	memcpy((unsigned char *) &Remote_Address.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	Remote_Address.sin_port=htons(10234);

	int e_con = connect(client_socket, (struct sockaddr *)&Remote_Address, sizeof(Remote_Address));
	if (e_con==-1) {
		perror("connect has failed");
		exit(-1);
	}
	
	// FIXED handle the write
	int e_wri = write(client_socket, "ping", 5);
	if (e_wri==-1) {
		perror("write has failed");
		exit(-1);
	}
	
	// FIXED handle the read
	int e_rea = read(client_socket, buf, 512);
	if (e_rea==-1) {
		perror("the read has failed");
		exit(-1);
	}
	
	printf("Client: message from server: %s \n", buf);
	
	int e_clo = close(client_socket);
	if (e_clo==-1) {
		perror("The close failed");
		exit(-1);
	}
	printf("Client: exit \n");
	exit(0);
}