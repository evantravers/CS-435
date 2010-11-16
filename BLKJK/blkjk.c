#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
		arc4random();
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
	printf("Calculating stand...\n");
	double stand = (double)stnd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD)/iterations;
	printf("Calculating double...\n");
	double dbl = (double)dubl(yourHand, dealerHand, iterations, aceFlagY, aceFlagD)/iterations;
	printf("Calculating hit...\n");
	double hit = (double)hitd(yourHand, dealerHand, iterations, aceFlagY, aceFlagD)/iterations;
	printf("Stand odds:\t%f\n", stand);
	printf("Double odds:\t%f\n", dbl);
	printf("Hit odds:\t%f\n", hit);
	
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