#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int getCard();
float stnd(int, int, int);
float dubl(int, int, int);

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
	
	int yourHand = atoi(argv[1]) + atoi(argv[2]);
	int dealerHand = atoi(argv[3]);
	int iterations = atoi(argv[4]);
	printf("Generating %d hands using your hand: %d, %d, (%d), and dealer up: %d...\n", iterations, atoi(argv[1]), atoi(argv[2]), yourHand, dealerHand);
	
	// time to compute
	// 1 for stand, 2 for hit, 3 for double
	float stand = stnd(yourHand, dealerHand, iterations);
	//float hit = hit(yourHand, dealerHand, iterations);
	float dbl = dubl(yourHand, dealerHand, iterations);
	
	printf("Stand odds: %f\n", stand);
	printf("Double odds: %f\n", dbl);
	
	// TODO return the one that has the best winning ratio
}

int getCard(int hand) {
	unsigned int RN = random();
	RN = (RN % 10)+1;
	// TODO handle the ace
	if (RN==1) {
		if ((hand+10)>21) {
			return 1;
		}
		else {
			return 11;
		}
	}
	return RN;
}

float stnd(int yourHand, int dealerHand, int iterations) {
	// 1 for stand, 2 for hit, 3 for double
	int odds = 0, cnt = 0, dHand, yHand;
	while (cnt < iterations) {
		// set the values back
		yHand=yourHand;
		dHand=dealerHand;
		
		// your turn
		// you sit here
		printf("You end with: %d\n", yHand);
		
		// dealer hits until 17
		while (dHand<17&&dHand<22) {
			int newCard = getCard(dHand);
			// printf("Dealer drew a %d\n", newCard);
			dHand=dHand+newCard;
		}
		printf("Dealer ends with: %d\n", dHand);
		
		// if dealerHand is worth more than yours and is less than 21, he wins.
		if (dHand>yHand&&dHand<22) {
			printf("Dealer wins!\n");
			odds--;
		}
		// you win!
		else {
			printf("You win!\n");
			odds++;
		}
		cnt++;
	}
	return (float)odds/iterations;
}

float dubl(int yourHand, int dealerHand, int iterations) {
	// 1 for stand, 2 for hit, 3 for double
	int odds = 0, cnt = 0, dHand, yHand;
	while (cnt < iterations) {
		// set the values back
		yHand=yourHand;
		dHand=dealerHand;
		
		// your turn
		int newCard = getCard(yHand);
		// printf("You drew a %d\n", newCard);
		yHand=yHand+newCard;
		// you hit once
		
		// you sit here
		printf("You end with: %d\n", yHand);
		
		// dealer hits until 17
		while (dHand<17&&dHand<22) {
			int newCard = getCard(dHand);
			// printf("Dealer drew a %d\n", newCard);
			dHand=dHand+newCard;
		}
		printf("Dealer ends with: %d\n", dHand);
		
		// if dealerHand is worth more than yours and is less than 21, he wins.
		if (dHand>yHand&&dHand<22) {
			// printf("Dealer wins!\n");
			odds-2;
		}
		// you win!
		else {
			// printf("You win!\n");
			odds+2;
		}
		cnt++;
	}
	return odds/iterations;
}