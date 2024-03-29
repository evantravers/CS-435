// CS-435
// Evan Travers

/*
	Testing for packet loss:
		I tested many different methods of ensuring packet loss, to different measures of success. The methods were server sleep, buffer sizes, and "packet" sizes.
	Server sleep
		The first thing I tested simulates massive network traffic or busy a processor on the server. The only way this will cause packet loss is when the timeout exceeds the client timeout, or if you add a sleep to the client that will stagger with the server, and occasionally timeout.
	Buffer sizes
		Buffer sizes are pretty obvious. If the server or client has its hands full of messages all ready, then potentially things can sit in the queue long enough to time out. Another method is to send very large messages back and forth, filling up a normally reasonably sized buffer.
	Conclusions
		The main difficulty with causing packet loss with the way my code is written is that the client only handles one connection and one packet at a time. It's very difficult to cause dropping under those circumstances. I suppose it might be possible if you forked off the client code that handles the servers, and handled them all at once, but as it stands it's nearly impossible, without skewing things horrifically as described above.
*/

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
	if (argc != 3) {
		printf("Usage: ./client <subnet mask (i.e. 255.255.255.0)> <port> \n");
		exit(0);
	}
	
	// initialize
	int my_socket, on=1, off=0;
	struct hostent *hp;
	struct sockaddr_in server;
	struct sockaddr_in listofservers[30];
	char buf[512];
	int bytes, scanning=1;
	int more_servers=1, server_count=0, server_count_b=0, fromlen;

	// initialize sockets	
	hp = gethostbyname(argv[1]);
	memcpy((unsigned char *) & server.sin_addr, (unsigned char *) hp->h_addr, hp->h_length);
	
	my_socket = Socket(AF_INET, SOCK_DGRAM, 0);
	
	// enable broadcast
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr = *((struct in_addr *)hp->h_addr);
	server.sin_addr.s_addr|=htonl(0xff);
	
	// this creates an array of all the servers on the subnet
	memset(&listofservers, 0, sizeof(listofservers));
	while (scanning=1) {
		// begin the server scan
		server_count=0;
		// send the greeting
		sprintf(buf, "ping");
		bytes = Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));	
		// printf("sent %d bytes to %s\n", bytes, inet_ntoa(server.sin_addr));
		
		// generate an arrary of IPs.
		while(more_servers==1) {
			fromlen = sizeof(server);
			// this is to handle blocking on the recvfrom
			signal(SIGALRM, timeout_handler);
			alarm(RECV_TIMEOUT);
			if (sigsetjmp(recv_timed_out, 1)) {
				printf("Scanning...\n");
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
		
		// if the number of servers you found this time is same as last, then stop looking
		if (server_count==server_count_b) {
			break;
		}
		else {
			server_count_b=server_count;
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
	}
	
	// turn off broadcast
	Setsockopt(my_socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(off));
	
	// talk to each server
	int i;
	for (i = 0; i < server_count; ++i) {
		server = listofservers[i];
		int j;
		for (j = 0; j < 4000; ++j) {
			sprintf(buf, "%d", j);
			printf("Sending \"%s\" to %s\n", buf, inet_ntoa(server.sin_addr));
			bytes = Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *)&server, sizeof(server));
			signal(SIGALRM, timeout_handler);
			alarm(RECV_TIMEOUT);
			if (sigsetjmp(recv_timed_out, 1)) {
				printf("ERROR on packet #%d\n:", j);
				j=j-1;
			}
			bytes = Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &server, &fromlen);
			alarm(0);
			signal(SIGALRM, SIG_DFL);
			printf("Received message from Host #%d: %s\n", i, buf);
			if (atoi(buf)!=j) {
				printf("ERROR, packets out of order.\n");
				break;
			}
		}
	}
	
	Close(my_socket);
}
