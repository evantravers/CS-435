#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// TODO implement the twist random number dev
#include "mtwist.h"

int getCard();
int stnd(int, int, int, int, int);
int dubl(int, int, int, int, int);
int hitd(int, int, int, int, int);

/* Params: (1) Your card, (2) Your card, (3) Dealer up, (4) number of iterations, (5) [optional] seed value */
int main(int argc, char *argv[]) {
	// handle the input
	if (argc==6) {
		// set seed value
		unsigned int seed = atoi(argv[5]);
		printf("Setting seed to %u\n", seed);
		srandom(seed);
	}
	else if (argc==5) {
		// set random seed value.
		srandomdev();
	}
	else {
		printf("Params: (1) Your card, (2) Your card, (3) Dealer up, (4) number of iterations, (5) [optional] seed value\n");
		exit(0);
	}
	int aceFlagY=0, aceFlagD=0;
	
	int yourHand = atoi(argv[1]) + atoi(argv[2]);
	if (atoi(argv[1])==1||atoi(argv[2])==1) {
		aceFlagY=1;
	}
	int dealerHand = atoi(argv[3]);
	if (dealerHand==1) {
		aceFlagD=1;
	}
	int iterations = atoi(argv[4]);
	printf("Generating %d hands using your hand: %d, %d, (%d), and dealer up: %d...\n", iterations, atoi(argv[1]), atoi(argv[2]), optHand(yourHand, aceFlagY), optHand(dealerHand, aceFlagD));

	// time to compute
	float stand = (float)stnd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD)/iterations;
	float dbl = (float)dubl(yourHand, dealerHand, iterations, aceFlagY, aceFlagD)/iterations;
	float hit = (float)hitd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD)/iterations;

	printf("Stand odds:\t%f\n", stand);
	printf("Double odds:\t%f\n", dbl);
	printf("Hit odds:\t%f\n", hit);
	if (stand>dbl&&dbl>hit) {
		printf("You should stand.\n");
	}
	if (dbl>stand&&dbl>hit) {
		printf("You should double.\n");
	}
	if (hit>stand&&hit>dbl) {
		printf("You should hit.\n");
	}
	
	// TODO return the one that has the best winning ratio
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
		while (optHand(dHand,aFlagD)<18) {
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
		while (optHand(dHand,aFlagD)<18) {
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
		if (optHand(yHand,aFlagY)>22) {
			odds--;
			break;
		}
		
		// dealer hits
		if (optHand(dHand,aFlagD)<18) {
			int newCard=getCard();
			if (newCard==1) {
				aFlagD=1;
			}
			dHand=dHand+newCard;
		}
		
		if (optHand(dHand,aFlagD)>22) {
			odds++;
			break;
		}
		
		// TODO consider how to handle the hand changing constantly. Static?
		// ALSO: there's an issue with static being run twice here. Not exactly the same params.
		// try all the options for this hand.	
		
		// stand
		int stndodds = stnd(yHand, dHand, iterations, aFlagY, aFlagD);
		// hit
		int hitdodds = hitd(yHand, dHand, iterations, aFlagY, aFlagD);
		
		if (stndodds>hitdodds) {
			return stndodds;
		}
		else {
			return hitdodds;
		}
		
		// final positions
		int dHandF = optHand(dHand,aFlagD);
		int yHandF = optHand(yHand,aFlagY);
		
		// if your hands are not the same (tie)
		if (dHandF!=yHandF) {
			if ((yHandF>dHandF)&&(yHandF<22)||(yHandF<22)&&(dHandF>21)) {
				odds=odds+1;
			}
			else {
				odds=odds-1;
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