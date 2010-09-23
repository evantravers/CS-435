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

//===============================================================
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
	
//===============================================================

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

//===============================================================

/*

	Race conditions
	for a single file descriptor, only one process should read or write it. Same thing
	applies to sockets.
	
	Hiatt totally whitehatted Cray's mainframe. 

*/

//===============================================================

/* 
	Processes in unix 091410
	
	Meanwhile, back at the ranch... the lone ranger disguised as a pool table got his balls racked. 
	
	The fit begins to hit the shan.
	
		This man. He is awesome.
		
	just like in real life, parents killing their child isn't all that uncommon, but the child shouldn't kill the parent.
	
	
	
*/
	int i;
	i=fork();
	if (i) {
		printf("hello from the parent\n");
	}
	else {
		sleep(5);
		printf("this is from the child\n");
		exit(0);
	}
	// creates an exact clone of the current program at the point directly after the fork.
	
	void SignalInterrupt(int sigtype) {
		printf("caught a signal <%d>\n", sigtype);
		while (1) {
			int status, r;
			r=waitpid(-1, &status, WNOHANG);
			if (r <= 0) {
				return;
				printf("exit status=%d\n", WEXITSTATUS(status));
			}
		}
	}
	
	// .......
	
	signal(SIGCHLD,SignalInterrupt);
	
	if (errno == EINTR) continue

//===============================================================

/* 
	092110 Working with unix in C programs
	AKA, making your program behave as standard in. Most unix programs read from standard in, so we'll have to write to stdin

	// takes two file descriptors, makes the destination look exactly like the source
	// you can reroute stdin stdout this way.
	
	// Hiatt recommends that you don't dup within your main program, but instead use an outside source.
	
	----[MyProg]<----------------------
	|							      |
	|							      |
	---->STDIN-->[ExtProg]-->STDONT-->
*/
	
	dup2(int oldfd, int newfd);
	
	int fd=open("file.out", O_WRONLY | O_CREAT | S_IRWXU);
	printf("this is before the dup2\n");
	write(fd,"this is data\n",13);
	// dupes the current file over stdout
	// no way to get it back unless you you "save" a copy of stdin somewhere else
	// fd<-1 i.e. copies/points 1 to fd
	dup2(fd,1);
	printf("this is after the dup2\n");
	close (fd);
	
/*
	child process is going to spawn another child, that will make the execs. One process for each command sent over the network.
	
	This way we'll learn to levy the power of the smaller programs in Unix
	TODO looks like I'll need to write wrappers for pipe, execvp, dup2, and sleep.
*/

	int i, infd[2];
	char buf[4096];
	signal(SIGCHLD,SignalInterrupt);
	pipe(infd);
	printf("creating child process...\n");
	i=fork();
	if (i==0) {
		char *argv[3];
		argv[0]="/bin/ls";
		argv[1]="-L";
		argv[2]=0;
		// read from 0 end, write to 1 end. (pipe)
		dup2(infd[1],1);
		dup2(infd[1],2);
		execvp(argv[0], argv);
		printf("exec failed\n");
		exit(0);
	}
	else {
		sleep(5);
		printf("reading from the pipe...\n");
		int bytes=read(infd[0],buf,4096);
		printf("read %d bytes\n", bytes);
		write(1,buf,bytes);
	}

//===============================================================

	/*
		Assignment 4: should be able to execute /bin/ls -al and return the results from the server
		to the client.
		
		
		
	*/

//===============================================================