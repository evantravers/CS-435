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
#include <unistd.h>

#include "Socket.h"
#include "Bind.h"
#include "Listen.h"
#include "Accept.h"
#include "Read.h"
#include "Write.h"
#include "Close.h"
#include "Recv.h"
#include "Select.h"
#include "Execvp.h"
#include "Dup2.h"

void SignalInterrupt(int sigtype) {
	//printf("caught a signal <%d>\n", sigtype);
	while (1) {
		int status, r;
		r=waitpid(-1, &status, WNOHANG);
		if (r<=0) {
			return;
		}
		if (WEXITSTATUS(status) == 1) {
			kill(0, SIGKILL);
			exit(0);
		}
	}
}

main(int argc, char *argv[]) {
	struct sockaddr_in Server_Address_Passive;
	int passive_socket, socket_to_client;
	
	signal(SIGCHLD,SignalInterrupt);
	
	char buf[512];
	int forknum;

	int bytes;
	
	// set up the passive socket
	passive_socket=Socket(AF_INET,SOCK_STREAM,0);
	Server_Address_Passive.sin_family=AF_INET;
	Server_Address_Passive.sin_addr.s_addr=htonl(INADDR_ANY);
	Server_Address_Passive.sin_port=htons(atoi(argv[1]));
	
	Bind(passive_socket, (struct sockaddr *) &Server_Address_Passive, sizeof(struct sockaddr));
	Listen(passive_socket,5);
	
	while(1) {
		socket_to_client=Accept(passive_socket, 0, 0);
		printf("A client has connected\n");
		int forknum = fork();
		while (forknum==0) {			
			// do client handling in here
			bytes=Recv(socket_to_client,buf,512,0);
			if (bytes<=0) {
				break;
			}
			else {
				printf("Client: %s\n", buf);
				// check for killswitch
					if (!strcmp(buf, "quit\n")) {
						// if that data is quit, close all connections and exit itself
						Write(socket_to_client, "Goodbye!\n", 512);
						Write(socket_to_client, "quit", 512);
						Close(socket_to_client);
						printf("Closing all connections and exiting.\n");
						exit(1);
					}
					if (!strcmp(buf, "end\n")) {
						// if that data is end, then close the connection cleanly
						printf("Client has exited\n");
						Write(socket_to_client, "Goodbye!\n", 512);
						Write(socket_to_client, "quit", 512);
						Close(socket_to_client);
						exit(0);
					}
					
					int forknum2=fork();
					while (forknum2==0) {
						// tokenize the buffer string into an array
						// of strings
						char *x;
						char *args[15];
						x=strtok(buf, " ,\n");
						args[0]=x;
						
						int k=0;
						while(1) {
							k++;
							x=strtok(NULL," ,\n");
							if (!x||k==14) {
								break;
							}
							args[k]=x;
						}
						
						dup2(socket_to_client,1);
						dup2(socket_to_client,2);
						
						execvp(args[0],args);
						exit(0);
					}
					
				// received a message. fork off a grandchild, just to handle the thing.
				Write(socket_to_client, buf, 512);
			}
		}
	}
}