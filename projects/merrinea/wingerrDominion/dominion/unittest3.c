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
	int seed = 1000;
	int handPos = 0, choice1 = 0, choice2 = 0, currentPlayer = 0;
	initializeGame(numPlayers, k, seed, &G);

	printf("\n\n*****************TESTING AMBASSADOR CARD*******************\n\n");

	printf("\nTest 1: Choice 1 = estate, Choice 2 = 2 cards: Testing ability to discard 2 cards\n");
	G.hand[currentPlayer][0] = ambassador;
	G.hand[currentPlayer][1] = estate;
	G.hand[currentPlayer][2] = estate;
	G.hand[currentPlayer][3] = estate;
	G.hand[currentPlayer][4] = smithy;
	G.handCount[currentPlayer] = 5;

	G.hand[currentPlayer + 1][0] = baron;
	G.hand[currentPlayer + 1][1] = baron;
	G.hand[currentPlayer + 1][2] = baron;
	G.handCount[currentPlayer + 1] = 3;

	choice1 = 1;
	choice2 = 2;
	discarded = 3;
	int numInDiscard = 1;
	memcpy(&testG, &G, sizeof(struct gameState));
	ambassadorRefactor(currentPlayer, &testG, choice1, choice2, handPos);
	int gainedHandCards = 1;
	int gainedSupplyCards = 2;

	int i, j;
	int numEstatesCurrPlayer = 0;
	int numEstatesOtherPlayer = 0;
	for (i = 0; i < testG.handCount[currentPlayer + 1]; i++) {
		if (testG.hand[currentPlayer + 1][i] == estate) {
			numEstatesOtherPlayer++;
		}
	}
	for (j = 0; j < testG.handCount[currentPlayer]; j++) {
		if (testG.hand[currentPlayer][j] == estate) {
			numEstatesCurrPlayer++;
		}
	}
	int testNumHandCards = testG.handCount[currentPlayer];
	int expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	int discardTest = testG.discardCount[currentPlayer];
	int discardExpected = G.discardCount[currentPlayer] + numInDiscard;
	int otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	int otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1] + gainedHandCards;
	int numSupplyCountEstate = testG.supplyCount[estate];
	int numSupplyCountEstateExp = G.supplyCount[estate] + gainedSupplyCards - gainedHandCards;
	int playedCardCountTest = testG.playedCardCount;
	int playedCardCountExpected = G.playedCardCount;

	//Make sure number of cards in hand is 1 less
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of cards in currentPlayer's discard pile is correct, should be 3 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if (otherPlayerHandCardCount == otherPlayerHandCardCountExp) {
		printf("Card counts of other player affected: PASSED\n");
	}
	else {
		printf("Card counts of other player affected: FAILED\n");
	}
	//Make sure current Player only has 1 estate card now
	if (numEstatesCurrPlayer == 1) {
		printf("Current player's hand contains only 1 Estate card: PASSED\n");
	}
	else {
		printf("Current player's hand contains only 1 Estate card: FAILED\n");
	}
	//Make sure other player's hand now contains an estate card
	if (numEstatesOtherPlayer == 1) {
		printf("Other player's hand contains Estate card: PASSED\n");
	}
	else {
		printf("Other player's hand contains Estate card: FAILED\n");
	}
	//Make sure card count in Estate supply reflects discarded card, should be 1 more
	if (numSupplyCountEstate == numSupplyCountEstateExp) {
		printf("Supply count of Estate cards changed: PASSED\n");
	}
	else {
		printf("Supply count of Estate cards changed: FAILED\n");
	}
	//Make sure the duplicates are being trashed
	if (playedCardCountTest == playedCardCountExpected) {
		printf("Duplicate cards are trashed, not placed in played cards: PASSED\n");
	}
	else {
		printf("Duplicate cards are trashed, not placed in played cards: FAILED\n");
	}

	printf("\nTest 2: choice 1 = estate, choice 2 = 3: Testing choice 2 failure, > 3\n");

	G.hand[currentPlayer][0] = ambassador;
	G.hand[currentPlayer][1] = estate;
	G.hand[currentPlayer][2] = estate;
	G.hand[currentPlayer][3] = estate;
	G.hand[currentPlayer][4] = smithy;
	G.handCount[currentPlayer] = 5;

	G.hand[currentPlayer + 1][0] = baron;
	G.hand[currentPlayer + 1][1] = baron;
	G.hand[currentPlayer + 1][2] = baron;
	G.handCount[currentPlayer + 1] = 3;

	choice1 = 1;
	choice2 = 3;
	memcpy(&testG, &G, sizeof(struct gameState));
	int result = ambassadorRefactor(currentPlayer, &testG, choice1, choice2, handPos);
	gainedHandCards = 0;
	gainedSupplyCards = 0;

	numEstatesCurrPlayer = 0;
	numEstatesOtherPlayer = 0;
	for (i = 0; i < testG.handCount[currentPlayer + 1]; i++) {
		if (testG.hand[currentPlayer + 1][i] == estate) {
			numEstatesOtherPlayer++;
		}
	}
	for (j = 0; j < testG.handCount[currentPlayer]; j++) {
		if (testG.hand[currentPlayer][j] == estate) {
			numEstatesCurrPlayer++;
		}
	}
	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer];
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer];
	otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1];
	numSupplyCountEstate = testG.supplyCount[estate];
	numSupplyCountEstateExp = G.supplyCount[estate];

	//Make sure initial asserts do not pass
	if (result == -1) {
		printf("Choice 2 is too large: PASSED\n");
	}
	else {
		printf("Choice 2 is too large: FAILED\n");
	}
	//Make sure number of cards in hand is same
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of cards in currentPlayer's discard pile is correct, should be same
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if (otherPlayerHandCardCount == otherPlayerHandCardCountExp) {
		printf("Card counts of other player not affected: PASSED\n");
	}
	else {
		printf("Card counts of other player not affected: FAILED\n");
	}
	//Make sure current Player still has 3 estate cards
	if (numEstatesCurrPlayer == 3) {
		printf("Current player's hand still contains 3 Estate cards: PASSED\n");
	}
	else {
		printf("Current player's hand still contains 3 Estate card: FAILED\n");
	}
	//Make sure other player's hand now contains an estate card
	if (numEstatesOtherPlayer == 0) {
		printf("Other player's hand contains no Estate card: PASSED\n");
	}
	else {
		printf("Other player's hand contains no Estate card: FAILED\n");
	}
	//Make sure card count in Estate supply reflects discarded card, should be 2 more
	if (numSupplyCountEstate == numSupplyCountEstateExp) {
		printf("Supply count of Estate cards did not change: PASSED\n");
	}
	else {
		printf("Supply count of Estate cards ddi not change: FAILED\n");
	}

	printf("\n****************TEST AMBASSADOR CARD COMPLETE********************\n");
	return 0;
}