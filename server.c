#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include "Socket.h"

// TODO handle the system call error handling
// don't forget to use perror()

main() {
	struct sockaddr_in Server_Address_Passive;
	int server_socket, socket_to_client;
	int procid, tmp;
	char buf[512];
	int msglength;
	// FIXED take care of Socket
	server_socket=Socket();
	Server_Address_Passive.sin_family=AF_INET;
	Server_Address_Passive.sin_addr.s_addr=htonl(INADDR_ANY);
	Server_Address_Passive.sin_port=htons(10234);
	// FIXED take care of bind
	int e_bin = bind(server_socket, (struct sockaddr *) &Server_Address_Passive, sizeof(struct sockaddr));
	if (e_bin==-1) {
		perror("the bind has failed");
		exit(-1);
	}
	// FIXED take care of listen
	int e_lis = listen(server_socket,5);
	if (e_lis==-1) {
		perror("the listen has failed");
		exit(-1);
	}
	while (1) {
		// FIXED take care of accept
		socket_to_client=accept(server_socket, 0 ,0);
		if (socket_to_client==-1) {
			perror("socket accept has failed");
			exit(-1);
		}
				
		// FIXED take care of read
		msglength=read(socket_to_client,buf,512);
		if (msglength==-1) {
			perror("the read has failed");
			exit(-1);
		}
		
		printf("SERVER: msg from client: %s\n", buf);
		
		// FIXED take care of write
		int e_wri = write(socket_to_client, "pong", 5);
		if (e_wri==-1) {
			perror("write has failed");
			exit(-1);
		}
		
		
		// FIXED take care of close
		int e_clo = close(socket_to_client);
		if (e_clo==-1) {
			perror("the close has failed");
			exit(-1);
		}
		exit(0);
	}
}