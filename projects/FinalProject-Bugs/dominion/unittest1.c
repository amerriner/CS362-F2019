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
	int discarded;
	int trashed;
	int gained;
	int seed = 1000;
	int handPos = 0, choice1 = 0, choice2 = 0, currentPlayer = 0;
	initializeGame(numPlayers, k, seed, &G);

	printf("\n\n******************BEGIN TEST MINE CARD: DISCARDED CARDS****************\n\n");
	//instantiate current player hand
	G.hand[currentPlayer][0] = mine;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = baron;
	G.hand[currentPlayer][3] = estate;
	G.handCount[currentPlayer] = 4;

	//changing out copper
	choice1 = 1;
	//for silver 
	choice2 = silver;

	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(11, choice1, choice2, 0, &testG, handPos, 0);
	
	//set expectations
	discarded = 1;
	trashed = 1;
	gained = 1;

	int testNumHandCards = testG.handCount[currentPlayer];
	int expectedNumHandCards = G.handCount[currentPlayer] - discarded - trashed + gained;
	int discardTest = testG.discardCount[currentPlayer];
	int discardExpected = G.discardCount[currentPlayer] + discarded;
	int playedCardTest = testG.playedCardCount;
	int playedCardExpected = G.playedCardCount + discarded;
	int otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	int otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1];
	int otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	int otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1];
	int numSupplyCountSilver = testG.supplyCount[silver];
	int numSupplyCountSilverExp = G.supplyCount[silver] - gained;

	/*ASSERTIONS*/

	//Make sure number of cards in hand is 1 less
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
		printf("Expected: %d, Actual: %d\n", discardExpected, discardTest);
	}
	//Make sure number of cards in playedCard pile is correct, should be 1 more
	if (playedCardTest == playedCardExpected) {
		printf("Correct number of cards in played card pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in played card pile: FAILED\n");
		printf("Expected: %d, Actual: %d\n", playedCardExpected, playedCardTest);
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerHandCardCount == otherPlayerHandCardCountExp)) {
		printf("Card counts of other player not affected: PASSED\n");
	}
	else {
		printf("Card counts of other player not affected: FAILED\n");
	}
	//Make sure card count in Silver supply reflects swapped card, should be one less
	if (numSupplyCountSilver == numSupplyCountSilverExp) {
		printf("Supply count of Silver cards changed: PASSED\n");
	}
	else {
		printf("Supply count of Silver cards changed: FAILED\n");
	}

	printf("\n*******************************END TEST**********************************\n");
	return 0;
}