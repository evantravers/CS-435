#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>

// TODO handle the system call error handling
// don't forget to use perror()

main() {
	struct sockaddr_in Server_Address_Passive;
	int server_socket, socket_to_client;
	int procid, tmp;
	char buf[512];
	int msglength;
	// TODO take care of Socket
	server_socket=socket(AF_INET,SOCK_STREAM,0);
	Server_Address_Passive.sin_family=AF_INET;
	Server_Address_Passive.sin_addr.s_addr=htonl(INADDR_ANY);
	Server_Address_Passive.sin_port=htons(10214);
	// TODO take care of bind
	bind(server_socket, (struct sockaddr *) &Server_Address_Passive, sizeof(struct sockaddr));
	// TODO take care of listen
	listen(server_socket,5);
	while (1) {
		// TODO take care of accept
		socket_to_client=accept(server_socket, 0 ,0);
		// TODO take care of read
		msglength=read(socket_to_client,buf,512);
		printf("SERVER: msg from client: %s\n", buf);
		// TODO take care of write
		write(socket_to_client, "pong", 5);
		// TODO take care of close
		close(socket_to_client);
		exit(0);
	}
}