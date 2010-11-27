// CS-435
// Evan Travers
// server

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Socket.h"
#include "Bind.h"
#include "Recvfrom.h"
#include "Close.h"
#include "Sendto.h"
#include "random32.c"

// basic UDP client, returns everything it is sent until is sent a 'quit' message, modified to play a ton of blackjack hands

int getCard();
int stnd(int, int, int, int, int);
int dubl(int, int, int, int, int);
int hitd(int, int, int, int, int);
char* play(int, int, int, int, int);

main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: ./server <port> \n");
		exit(0);
	}
  	srandom(Random32());	
	struct sockaddr_in client;
	struct hostent *hp;
	int my_socket, bytes;
	char buf[512];
	unsigned int fromlen;

	my_socket=Socket(AF_INET, SOCK_DGRAM, 0);
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=htonl(INADDR_ANY);
	client.sin_port=htons(atoi(argv[1]));
	
	Bind(my_socket, (struct sockaddr *) &client, sizeof(client));
	
	while(1) {
		bzero(buf, sizeof(buf));
		fromlen=sizeof(client);
		bytes=Recvfrom(my_socket, buf, 512, 0, (struct sockaddr *) &client, &fromlen);
		// detect type of transmission
		if (!strcmp(buf, "quit")) {
			printf("Quitting!\n");
			exit(0);
		}
		printf("Received this buf: %s\n", buf);
		// if it's ping, ping back
		if (strcmp(buf, "ping")==0) {
			printf("Sending the ping back...\n");
			bytes=Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *) &client, sizeof(client));
		}
		else {
			printf("No longer pinging now!\n");
			// otherwise, parse the input, run blackjack
			int yHand1=atoi(strtok(buf, " ,\n"));
			
			int yHand2=atoi(strtok(NULL, " ,\n"));
			int dealerHand=atoi(strtok(NULL, " ,\n"));
			int iterations=atoi(strtok(NULL, " ,\n"));
			int aceFlagY=0, aceFlagD=0;
			int yourHand = yHand1 + yHand2;
			
			if (yHand1==1||yHand2==1) {
				aceFlagY=1;
			}
			if (dealerHand==1) {
				aceFlagD=1;
			}
			printf("This is what we just read: %d %d %d %d %d\n", yourHand, dealerHand, aceFlagY, aceFlagD, iterations);
			
			int stand = stnd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD);
			int dbl = dubl(yourHand, dealerHand, iterations, aceFlagY, aceFlagD);
			int hit = hitd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD);
			sprintf(buf, "%d %d %d\n", stand, dbl, hit);
			
			bytes=Sendto(my_socket, buf, strlen(buf), 0, (struct sockaddr *) &client, sizeof(client));
		}
		
		// return the odds back to the client
	}
	
	// send results string back
	Close(my_socket);
	exit(0);
}

int getCard() {
	unsigned int RN = random();
	RN = (RN % 13)+1;
	if (RN > 9) {
		RN = 10;
	}
	return RN;
}

int stnd(int yourHand, int dealerHand, int iterations, int aceFlagY, int aceFlagD) {
	int odds = 0, cnt = 0, dHand=0, yHand=0, aFlagY=0, aFlagD=0;
	while (cnt < iterations) {
		// what is on the table?
		yHand = yourHand;
		dHand = dealerHand;
		
		// are either you or the dealer holding an ace?
		aFlagD=aceFlagD;
		aFlagY=aceFlagY;
		
		// your turn
		// you sit here
		
		// dealer hits until 17
		while (optHand(dHand, aFlagD)<17) {
			int newCard=getCard();
			if (newCard==1) {
				aFlagD=1;
			}
			dHand=dHand+newCard;
		}

		// final positions
		int dHandF = optHand(dHand,aFlagD);
		int yHandF = optHand(yHand,aFlagY);
		
		// if your hands are not the same (tie)
		if (dHandF!=yHandF) {
			if ((yHandF>dHandF)&&(yHandF<22)||(yHandF<22)&&(dHandF>21)) {
				odds++;
			}
			else {
				odds--;
			}
		}
		cnt++;
	}
	return odds;
}

int dubl(int yourHand, int dealerHand, int iterations, int aceFlagY, int aceFlagD) {
	int odds = 0, cnt = 0, dHand, yHand, aFlagY, aFlagD;
	while (cnt < iterations) {
		// what is on the table?
		yHand = yourHand;
		dHand = dealerHand;
		
		// are either you or the dealer holding an ace?
		aFlagD=aceFlagD;
		aFlagY=aceFlagY;
		
		// your turn
		// you hit once
		int newCard = getCard();
		if (newCard==1) {
			aFlagY=1;
		}
		yHand=yHand+newCard;
		// you sit here
		
		// dealer hits until 17
		while (optHand(dHand,aFlagD)<17) {
			int newCard=getCard();
			if (newCard==1) {
				aFlagD=1;
			}
			dHand=dHand+newCard;
		}
		
		// final positions
		int dHandF = optHand(dHand,aFlagD);
		int yHandF = optHand(yHand,aFlagY);
		
		// if your hands are not the same (tie)
		if (dHandF!=yHandF) {
			if ((yHandF>dHandF)&&(yHandF<22)||(yHandF<22)&&(dHandF>21)) {
				odds=odds+2;
			}
			else {
				odds=odds-2;
			}
		}
		cnt++;
	}
	return odds;
}

int hitd(int yourHand, int dealerHand, int iterations, int aceFlagY, int aceFlagD) {
	static int odds = 0, dHand=0, yHand=0, aFlagY=0, aFlagD=0, cnt=0;
	while (cnt < iterations) {
		int newCard;
		// what is on the table?
		yHand = yourHand;
		dHand = dealerHand;
		
		// are either you or the dealer holding an ace?
		aFlagD = aceFlagD;
		aFlagY = aceFlagY;
		hit:
		// I hit it
		newCard = getCard();
		if (newCard==1) {
			aFlagY=1;
		}
		yHand=yHand+newCard;
		
		// did you bust?
		if (optHand(yHand, aFlagY)>21) {
			odds--;
		}
		else {
			// dealer hits until 17
			while (optHand(dHand,aFlagD)<17) {
				newCard=getCard();
				if (newCard==1) {
					aFlagD=1;
				}
				dHand=dHand+newCard;
			}
			
			// final positions
			int dHandF = optHand(dHand,aFlagD);
			int yHandF = optHand(yHand,aFlagY);
			
			// if you win
			if (yHandF>dHandF||dHandF>21) {
				odds++;
			}
			else {
				// if you are over your limit
				if (yHandF>16) {
					odds--;
				}
				else {
					goto hit;
				}
			}
		}
		cnt++;
	}
	return odds;
}
// optimizes the hand, deals with the ace
int optHand(int sum, int flag) {
	if (flag==1&&((sum+10)<22)) {
		return sum+10;
	}
	else {
		return sum;
	}
}
