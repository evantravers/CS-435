// this file contains notes, scraps, and things from lectures. Not intended to be compiled, just named scrap.c for the code coloring in textmate.

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
#include <sys/time.h>
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
	
		// (1) dictates the last possible descriptor to watch [1-16], (2) look at this set of file descriptors, watch for a possible read. last one is a timeout variable.
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

// signal 090710

/*
	used to create and manage child processes, because they'll return when they are
	completed
	
	usually triggers a procedure called a handler.
	
	handler() {
		---------
		---------
		---------
	}
	
	main() {
		---------
		---------
		---------
		---------
	}
	
	EINTR is an interrupt error no. Equivalent of errno
	
	SIGCHLD is useful for when child process terminates
	
	Every signal has a default action. Some terminate, some just ignore... list here
	man signal 7
	
	if you return from a signal, and you are blocked, then you are unblocked
	so you have to handle the select error code
	
	*** Remember... you can't use it to count. It's either on, or off. ***
	
*/

/*
	
	Do we got any zombies?

	If a process explicitly specifies SIG_IGN as the action for the signal
     SIGCHLD, the system will not create zombie processes when children of the
     calling process exit.  As a consequence, the system will discard the exit
     status from the child processes.  If the calling process subsequently
     issues a call to wait(2) or equivalent, it will block until all of the
     calling process's children terminate, and then return a value of -1 with
     errno set to ECHILD.

*/

#include <signal.h>

// this can be named whatever
void SignalInterrupt(int sigtype) {
	// note no open close paren here
	// the below line is only needed on Sun Solaris
	// signal(SIGINT,SignalInterrupt);
	printf("Caught a signal <%d>\n",sigtype);
	counter++;
}

int main() {
	---------
	// when this signal occurs, run this procedure
	// stick this in with the instantiation of constants at the top
	signal(SIGINT,SignalInterrupt);
	---------
	---------
	---------
}

===============================================================

/*

	Race conditions
	for a single file descriptor, only one process should read or write it. Same thing
	applies to sockets.
	
	Hiatt totally whitehatted Cray's mainframe. 

*/

===============================================================

/* 
	Processes in unix 091410
*/

	i=fork();
	// creates an exact clone of the current program at the point directly after the fork.

===============================================================