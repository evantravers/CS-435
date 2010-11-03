#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int getCard();
float stnd(int, int, int, int, int);
float dubl(int, int, int, int, int);
float hitd(int, int, int, int, int);

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
	float stand = stnd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD);
	float dbl = dubl(yourHand, dealerHand, iterations, aceFlagY, aceFlagD);
	float hit = hitd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD);
	
	printf("Stand odds:\t%f\n", stand);
	printf("Double odds:\t%f\n", dbl);
	printf("Hit odds:\t%f\n", hit);
	
	// TODO return the one that has the best winning ratio
}

int getCard() {
	unsigned int RN = random();
	RN = (RN % 14)+1;
	if (RN > 9) {
		RN = 10;
	}
	return RN;
}

float stnd(int yourHand, int dealerHand, int iterations, int aceFlagY, int aceFlagD) {
	int odds = 0, cnt = 0, dHand, yHand, aFlagY=0, aFlagD=0;
	while (cnt < iterations) {
		// are either you or the dealer holding an ace?
		aFlagD=aceFlagD;
		aFlagY=aceFlagY;
		
		// what are the hands, calculating any aces involved
		dHand=optHand(dealerHand, aFlagD);
		yHand=optHand(yourHand, aFlagY);
		
		// your turn
		// you sit here
		
		// dealer hits until 17
		while (dHand<18) {
			int newCard=getCard();
			if (newCard==1) {
				aFlagD=1;
			}
			dHand=optHand(dHand+newCard,aFlagD);
		}
		
		yHand=optHand(yHand,aFlagY);
		dHand=optHand(dHand,aFlagD);

		// if your hands are not the same (tie)
		if (dHand!=yHand) {
			if ((yHand>dHand&&yHand<22)||(yHand<22&&dHand>21)) {
				odds++;
			}
			else {
				odds--;
			}
		}
		
		cnt++;
	}
	return (float)odds/iterations;
}

float dubl(int yourHand, int dealerHand, int iterations, int aceFlagY, int aceFlagD) {
	int odds = 0, cnt = 0, dHand, yHand, aFlagY, aFlagD;
	while (cnt < iterations) {
		// are either you or the dealer holding an ace?
		aFlagD=aceFlagD;
		aFlagY=aceFlagY;
		
		// what are the hands, calculating any aces involved
		yHand=optHand(yourHand, aFlagY);
		dHand=optHand(dealerHand, aFlagD);
		
		// your turn
		// you hit once
		int newCard = getCard();
		if (newCard==1) {
			aFlagY=1;
		}
		yHand=optHand(yHand+newCard,aFlagY);
		// you sit here
		
		// dealer hits until 17
		while (dHand<18) {
			int newCard=getCard();
			if (newCard==1) {
				aFlagD=1;
			}
			dHand=optHand(dHand+newCard,aFlagD);
		}
		
		yHand=optHand(yHand,aFlagY);
		dHand=optHand(dHand,aFlagD);
		
		// if dealerHand is worth more than yours and is less than 21, he wins.
		if (dHand!=yHand) {
			if ((yHand>dHand&&yHand<22)||(yHand<22&&dHand>21)) {
				odds=odds+2;
			}
			else {
				odds=odds-2;
			}
		}
		cnt++;
	}
	return (float)odds/iterations;
}

float hitd(int yourHand, int dealerHand, int iterations, int aceFlagY, int aceFlagD) {
	int odds = 0, cnt = 0, dHand, yHand, aFlagY, aFlagD;
	while (cnt < iterations) {
		// are either you or the dealer holding an ace?
		aFlagD=aceFlagD;
		aFlagY=aceFlagY;
		
		// what are the hands, calculating any aces involved
		yHand=optHand(yourHand, aFlagY);
		dHand=optHand(dealerHand, aFlagD);
		
		// your turn
		// you hit once
		int newCard = getCard();
		if (newCard==1) {
			aFlagY=1;
		}
		yHand=optHand(yHand+newCard,aFlagY);
		// you sit here
		if (yHand>22) {
			odds--;
		}
		
		// dealer hits
		if (dHand<18) {
			int newCard=getCard();
			if (newCard==1) {
				aFlagD=1;
			}
			dHand=optHand(dHand+newCard,aFlagD);
		}
		
		if (dHand>22) {
			odds++;
		}
		
		if (yHand<18) {
			// now you have to try all the options for this hand
			float stand = stnd(yHand, dHand, iterations-cnt, aFlagY, aFlagD);
			float hit = hitd(yHand, dHand, iterations-cnt, aFlagY, aFlagD);
			if (stand>hit) {
				return stand;
			}
			else {
				return hit;
			}
		}
		cnt++;
	}
	return (float)odds/iterations;
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