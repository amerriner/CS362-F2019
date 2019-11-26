#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState G, testG;
	int numPlayers = 2;
	int seed = 1000;
	initializeGame(numPlayers, k, seed, &G);

	printf("\n\n*********************BEGIN TEST ISGAMEOVER************************\n");

	//instantiate supply counts 
	for (int i = 0; i < 27; i++) {
		G.supplyCount[i] = 5;
	}

	G.supplyCount[6] = 0;
	G.supplyCount[20] = 0;
	G.supplyCount[26] = 0;

	memcpy(&testG, &G, sizeof(struct gameState));
	int result = isGameOver(&testG);

	int numEmptySupplies = 0;
	for (int j = 0; j < 27; j++) {
		if (testG.supplyCount[j] == 0) {
			numEmptySupplies++;
		}
	}

	/*ASSERTIONS*/
	if (result == 1) {
		printf("Game is over: PASSED\n");
	}
	else {
		printf("Game is over: FAILED\n");
	}
	if (numEmptySupplies == 3) {
		printf("Correct number (3) of empty supply piles: PASSED\n", numEmptySupplies);
	}
	else {
		printf("Correct number of empty supplies: FAILED\n");
	}

	printf("\n*****************************END TEST****************************\n");
	return 0;
}