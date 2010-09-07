// string comparison

#include <string.h>
#include <stdio.h>

char string[255];
// char string[255] = {"hello");

scanf("%s", string);
if (strcmp(string, "hello")) {
	printf("string does not match hello\n");
}
else {
	printf("you entered hello\n");
}

===============================================================
// select 090210
// this code is intended to set up a listener on a both server and input sockets,
// that only reads when it has data to work with, thus preventing blocks.

#include <sys/select.h>
#include <sys.time.h>
#include <sys/types.h>
#include <unistd.h>

	// server = ConnectToServer(argc,argv);

	// this is a string of bits, type fd_set is detailed by select.h. Set bit 0 and bit 3. 
	fd_set readfds;
	int bytes;
	int i,j,k;

	while (1) {
		
		// these are macros that set the proper bits. 
		// sets every single bit to zero
		FD_ZERO(&readfds);
	
		// sets bit zero (standard in) to one.
		FD_SET(0, &readfds);
	
		// sets the server bit normally 3 to start out with 
		FD_SET(server, &readfds);

		// int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds,fd_set *restrict errorfds, struct timeval *restrict timeout);
	
		// (1) dicates the last possible descriptor to watch [1-16], (2) look at this set of file descriptors, watch for a possible read. last one is a timeout variable.
		// select sets bits if fd's are open.
		i=select(16, &readfds, 0, 0, 0);
	
		// check to see if the server bit is being set, and read from it
		if (FD_ISSET(server, &readfds)) {
			bytes=recv(server,buffer,512,0);
			if (bytes<=0) {
				break;
			}
		}
	
		// check to see if the stdin is being set, and read from it
		if (FD_ISSET(0, &readfds)) {
			bytes=read(fileno(stdin),buffer,512);
			if (bytes<=0) {
				break;
			}
		}
	}
	
===============================================================