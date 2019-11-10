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
	int discarded;
	int seed = 1000;
	int handPos = 0, currentPlayer = 0;
	int nextPlayer = currentPlayer + 1;
	initializeGame(numPlayers, k, seed, &G);

	printf("\n\n**********************BEGIN TEST TRIBUTE CARD**********************\n");

	printf("\nTest 1: Golden Path, Player 2 has victory and treasure card\n");
	G.deck[nextPlayer][0] = estate;
	G.deck[nextPlayer][1] = copper;
	G.deck[nextPlayer][2] = estate;
	G.deck[nextPlayer][3] = copper;
	G.deckCount[nextPlayer] = 4;

	G.hand[currentPlayer][0] = tribute;
	int gainedCards = 2;
	xtraCoins = 2;
	discarded = 1;
	int otherPlayerDiscard = 2;
	memcpy(&testG, &G, sizeof(struct gameState));
	tributeEffect(&testG, currentPlayer, nextPlayer, handPos);

	int testNumHandCards = testG.handCount[currentPlayer];
	int expectedNumHandCards = G.handCount[currentPlayer] - discarded + gainedCards;
	int testNumCoins = testG.coins;
	int expectedNumCoins = G.coins + xtraCoins;
	int discardTest = testG.discardCount[currentPlayer];
	int discardExpected = G.discardCount[currentPlayer] + discarded;
	int otherPlayerDeckCardCount = testG.deckCount[currentPlayer + 1];
	int otherPlayerDeckCardCountExp = G.deckCount[currentPlayer + 1] - otherPlayerDiscard;
	
	int otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	int otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1] + otherPlayerDiscard;
	

	//Make sure number of cards in hand is 1 more (-1 for discard, +2 for victory reveal)
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be 2 more than before
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp)) {
		printf("Card counts of other player changed: PASSED\n");
	}
	else {
		printf("Card counts of other player changed: FAILED\n");
	}

	printf("\nTest 2: Golden Path, Player 2 has action and treasure card\n");
	G.deck[nextPlayer][0] = tribute;
	G.deck[nextPlayer][1] = copper;
	G.deck[nextPlayer][2] = tribute;
	G.deck[nextPlayer][3] = copper;
	G.deckCount[nextPlayer] = 4;

	G.hand[currentPlayer][0] = tribute;
	int gainedActions = 2;
	xtraCoins = 2;
	discarded = 1;
	otherPlayerDiscard = 2;
	memcpy(&testG, &G, sizeof(struct gameState));
	tributeEffect(&testG, currentPlayer, nextPlayer, handPos);

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins;
	int testNumActions = testG.numActions;
	int numActionsExp = G.numActions + gainedActions;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerDeckCardCount = testG.deckCount[currentPlayer + 1];
	otherPlayerDeckCardCountExp = G.deckCount[currentPlayer + 1] - otherPlayerDiscard;
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1] + otherPlayerDiscard;


	//Make sure number of cards in hand is 1 less (-1 for discard)
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of actions at end is correct, should be 2 more
	if (testNumActions == numActionsExp) {
		printf("Correct number of actions: PASSED\n");
	}
	else {
		printf("Correct number of actions: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be 2 more than before
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp)) {
		printf("Card counts of other player changed: PASSED\n");
	}
	else {
		printf("Card counts of other player changed: FAILED\n");
	}

	printf("\nTest 3: Only 1 card to reveal (1 deck card, 0 discard cards): treasure card\n");
	G.deck[nextPlayer][0] = copper;
	G.deckCount[nextPlayer] = 1;
	G.discardCount[nextPlayer] = 0;

	G.hand[currentPlayer][0] = tribute;
	xtraCoins = 2;
	discarded = 1;
	otherPlayerDiscard = 1;
	memcpy(&testG, &G, sizeof(struct gameState));
	tributeEffect(&testG, currentPlayer, nextPlayer, handPos);

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerDeckCardCount = testG.deckCount[currentPlayer + 1];
	otherPlayerDeckCardCountExp = G.deckCount[currentPlayer + 1] - otherPlayerDiscard;
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1] + otherPlayerDiscard;

	//Make sure number of cards in hand is 1 less (-1 for discard)
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be 2 more than before
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp)) {
		printf("Card counts of other player changed: PASSED\n");
	}
	else {
		printf("Card counts of other player changed: FAILED\n");
	}

	printf("\nTest 4: No cards to reveal\n");
	G.deckCount[nextPlayer] = 0;
	G.discardCount[nextPlayer] = 0;

	G.hand[currentPlayer][0] = tribute;
	discarded = 1;
	memcpy(&testG, &G, sizeof(struct gameState));
	tributeEffect(&testG, currentPlayer, nextPlayer, handPos);

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins;
	testNumActions = testG.numActions;
	numActionsExp = G.numActions;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerDeckCardCount = testG.deckCount[currentPlayer + 1];
	otherPlayerDeckCardCountExp = G.deckCount[currentPlayer + 1];
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1];

	//Make sure number of cards in hand is 1 less (-1 for discard)
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be the same
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of actions at end is correct, should be the same
	if (testNumActions == numActionsExp) {
		printf("Correct number of actions: PASSED\n");
	}
	else {
		printf("Correct number of actions: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp)) {
		printf("Card counts of other player has not changed: PASSED\n");
	}
	else {
		printf("Card counts of other player has not changed: FAILED\n");
	}

	printf("\nTest 5: Duplicate Cards revealed, only 1 bonus awarded\n");
	G.deck[nextPlayer][0] = copper;
	G.deck[nextPlayer][1] = copper;
	G.deck[nextPlayer][2] = copper;
	G.deckCount[nextPlayer] = 3;

	G.hand[currentPlayer][0] = tribute;
	discarded = 1;
	otherPlayerDiscard = 2;
	xtraCoins = 2;
	memcpy(&testG, &G, sizeof(struct gameState));
	tributeEffect(&testG, currentPlayer, nextPlayer, handPos);

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerDeckCardCount = testG.deckCount[currentPlayer + 1];
	otherPlayerDeckCardCountExp = G.deckCount[currentPlayer + 1] - otherPlayerDiscard;
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1] + otherPlayerDiscard;

	//Make sure number of cards in hand is 1 less (-1 for discard)
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be 2 more than before
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp)) {
		printf("Card counts of other player has changed: PASSED\n");
	}
	else {
		printf("Card counts of other player has changed: FAILED\n");
	}

	printf("\nTest 6: Other player has no cards in Deck, >= 2 cards in discard, all treasure\n");
	G.deckCount[nextPlayer] = 0;
	G.discard[nextPlayer][0] = gold;
	G.discard[nextPlayer][1] = copper;
	G.discard[nextPlayer][2] = silver;
	G.discardCount[nextPlayer] = 3;

	G.hand[currentPlayer][0] = tribute;
	discarded = 1;
	xtraCoins = 4;
	memcpy(&testG, &G, sizeof(struct gameState));
	tributeEffect(&testG, currentPlayer, nextPlayer, handPos);

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerDeckCardCount = testG.deckCount[nextPlayer];
	otherPlayerDeckCardCountExp = G.deckCount[nextPlayer];
	otherPlayerDiscardCount = testG.discardCount[nextPlayer];
	otherPlayerDiscardCountExp = G.discardCount[nextPlayer];

	//Make sure number of cards in hand is 1 less (-1 for discard)
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be 4 more than before
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 1 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Make sure card count for other player is correct
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp)) {
		printf("Card counts of other player has changed: PASSED\n");
	}
	else {
		printf("Card counts of other player has changed: FAILED\n");
	}

	printf("\n**********************END TEST TRIBUTE CARD*************************\n");
	return 0;
}