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
	int xtraCoins;
	int seed = 1000;
	int handPos = 0, choice1 = 0, choice2 = 0, currentPlayer = 0;
	initializeGame(numPlayers, k, seed, &G);

	printf("\n\n******************BEGIN TEST REMODEL CARD: TRYING TO TRADE COPPER FOR GOLD****************\n\n");

	//instantiate current player hand
	G.hand[currentPlayer][0] = mine;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = baron;
	G.hand[currentPlayer][3] = estate;
	G.handCount[currentPlayer] = 4;
	G.coins = 2;

	//changing out silver
	choice1 = 1;
	//for gold
	choice2 = gold;
	memcpy(&testG, &G, sizeof(struct gameState));
	int result = cardEffect(12, choice1, choice2, 0, &testG, handPos, 0);

	int costHandCard = getCost(G.hand[currentPlayer][1]);
	int costReplacingCard = getCost(choice2);
	int costDiff = costReplacingCard - costHandCard;
	int maxCostDiff = 2;
	xtraCoins = 0;

	int testNumCoins = testG.coins;
	int expectedNumCoins = G.coins + xtraCoins;
	int numSupplyCountGold = testG.supplyCount[gold];
	int numSupplyCountGoldExp = G.supplyCount[gold];

	//function should exit because gold is too expensive
	if (result == -3) {
		printf("Choice 2 is recognized as too expensive: PASSED\n");
	}
	else {
		printf("Choice 2 is recognized as too expensive: FAILED\n");
	}
	//choice 2 should be recognized as too expensive
	if (costDiff > maxCostDiff) {
		printf("Choice 2 costs more than 2 more coins than choice 1: PASSED\n");
	}
	else {
		printf("Choice 2 costs more than 2 more coins than choice 1: FAILED\n");
	}
	//#number coins should be the same, no swap should occur becuase type is too expensive
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//supply count of gold should be the same, as swap should not have happened
	if (numSupplyCountGold == numSupplyCountGoldExp) {
		printf("Supply count of Gold cards stayed the same: PASSED\n");
	}
	else {
		printf("Supply count of Gold cards stayed the same: FAILED\n");
	}

	printf("\n\n***************************************END TEST******************************************\n");
	return 0;
}
