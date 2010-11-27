// CS-435
// Evan Travers
// client

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Socket.h"
#include "Setsockopt.h"
#include "Sendto.h"
#include "Close.h"
#include "Recvfrom.h"
#include "Bind.h"

#define RECV_TIMEOUT 2

static sigjmp_buf recv_timed_out;

void timeout_handler (int signum) {
	signal(SIGALRM, SIG_DFL);
	siglongjmp(recv_timed_out, 1);
}

main(int argc, char *argv[]) {
	if (argc != 7) {
		printf("Usage: ./client <subnet IP (i.e. 192.168.1.205)> <port> <player card> <player card> <dealer card> <iterations> \n");
		exit(0);
	}
	
	// initialize
	int my_socket, on=1, off=0;
	struct hostent *hp;
	struct sockaddr_in server;
	struct sockaddr_in listofservers[30];
	char buf[512];
	int bytes;
	int server_count=0, server_count_b=99, fromlen;
	int stand_odds=0, double_odds=0, hit_odds=0;
	
	// initialize sockets	
	hp = gethostbyname(argv[1]);
	memcpy((unsigned char *) & server.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	
	my_socket = Socket(AF_INET, SOCK_DGRAM, 0);
	
	// enable broadcast
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr = *((struct in_addr *)hp->h_addr);
	server.sin_addr.s_addr|=htonl(0x1ff);
	
	// TODO account for possible packet loss somewhere in here.
	// this creates an array of all the servers on the subnet
	memset(&listofservers, 0, sizeof(listofservers));
	while (1) {
		printf("Scanning...\n");
		// begin the server scan
		server_count=0;
		// send the greeting
		sprintf(buf, "ping",0);
		bytes = Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));	
		// printf("sent %d bytes to %s\n", bytes, inet_ntoa(server.sin_addr));
		
		// generate an arrary of IPs.
		while(1) {
			fromlen = sizeof(server);
			// this is to handle blocking on the recvfrom
			signal(SIGALRM, timeout_handler);
			alarm(RECV_TIMEOUT);
			if (sigsetjmp(recv_timed_out, 1)) {
				break;
			}
			bytes = Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &server, &fromlen);
			alarm(0);
			signal(SIGALRM, SIG_DFL);

			// printf("Client: Found host at IP\t%s\nServer: %s\n", inet_ntoa(server.sin_addr), buf);
			// add to the array
			listofservers[server_count]=server;
			server_count++;
		}			
		break;
		// if the number of servers you found this time is same as last, then stop looking
		if (server_count==server_count_b) {
			break;
		}
		else {
			if (server_count>server_count_b) {
				server_count_b=server_count;	
			}
		}
	}
	
	printf("Servers found:\n");
	if (server_count>0) {
		int i;
		for (i = 0; i < server_count; ++i) {
			printf("Server %d: %s\n", i+1, inet_ntoa(listofservers[i].sin_addr));
		}
		
	}
	else {
		printf("None found!\n");
		exit(0);
	}
	
	// set up the string to be sent to each client
	int iterations = atoi(argv[6]);
	iterations=iterations/server_count;
	sprintf(buf, "%s %s %s %d", argv[3], argv[4], argv[5], iterations);
	// turn off broadcast
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(off));
	
	// talk to each server
	// send out an equal amount of work for each one.
	int i;
	for (i = 0; i < server_count; i++) {
		server = listofservers[i];
		// send to each server a proper number of items to work on
		// send yHand1 yHand2 dHand iterations/server_count;
		bytes = Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));
	}
	
	// TODO account for timeouts
	// get results
	for (i = 0; i < server_count; ++i) {
		// listen for each one
		bytes = Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &server, &fromlen);
		
		stand_odds=+atoi(strtok(buf, " ,\n"));
		double_odds=+atoi(strtok(NULL, " ,\n"));
		hit_odds=+atoi(strtok(NULL, " ,\n"));
		sprintf(buf, "quit");
		bytes = Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));
	}
	// compute and return
	
	double stand = (double)stand_odds/iterations;
	double duble = (double)double_odds/iterations;
	double hitd  = (double)hit_odds/iterations;
	
	printf("==== THE ODDS ==== \nStand:\t%f\nDouble:\t%f\nHit:\t%f\n", stand, duble, hitd);
	Close(my_socket);
	
	exit(0);
}
