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
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Socket.h"
#include "Bind.h"
#include "Recvfrom.h"
#include "Close.h"
#include "Sendto.h"

// basic UDP client, returns everything it is sent until is sent a 'quit' message

main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./client <port> \n");
		exit(0);
	}
	
	struct sockaddr_in client;
	struct hostent *hp;
	int my_socket, bytes;
	char buf[512];
	unsigned int fromlen;

	my_socket=Socket(AF_INET, SOCK_DGRAM,0);
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=htonl(INADDR_ANY);
	client.sin_port=htons(atoi(argv[1]));
	
	Bind(my_socket, (struct sockaddr *) &client, sizeof(client));
	
	while(1) {
		bzero(buf, sizeof(buf));
		fromlen=sizeof(client);
		bytes=Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &client, &fromlen);
		printf("SERVER: read %d bytes from client: (%s)\n", bytes, buf);
		printf("Sending the data back...\n");
		bytes=Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *) &client, sizeof(client));
		if (!strcmp(buf, "quit")) {
			printf("Quitting!\n");
			exit(0);
		}
	}
	Close(my_socket);
}
