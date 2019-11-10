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
	int numPlayers = 3;
	int xtraCoins = 2;
	int discarded;
	int actions = 1;
	int seed = 1000;
	int handPos = 0, choice1 = 1, choice2 = 0, currentPlayer = 0;
	initializeGame(numPlayers, k, seed, &G);


	printf("\n\n*****************BEGIN TEST MINION CARD*******************\n");

	//Test 1: player chooses to gain coins
	printf("\nTEST 1: Choice1 = 1, Choice2 = 0: Gain 2 coins\n");

	//make sure the player has the minion card to play
	G.hand[currentPlayer][0] = minion;

	memcpy(&testG, &G, sizeof(struct gameState));
	minionEffect(choice1, choice2, &testG, currentPlayer, handPos);
	discarded = 1;

	int testNumHandCards = testG.handCount[currentPlayer];
	int expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	int discardTest = testG.discardCount[currentPlayer];
	int discardExpected = G.discardCount[currentPlayer] + discarded;
	int testNumCoins = testG.coins;
	int expectedNumCoins = G.coins + xtraCoins;
	int numActions = testG.numActions;
	int numActionsExpected = G.numActions + actions;
	int playerLeftHandCardCount = testG.handCount[currentPlayer + 1];
	int playerLeftHandCardCountExp = G.handCount[currentPlayer + 1];
	int playerRightHandCardCount = testG.handCount[currentPlayer + 2];
	int playerRightHandCardCountExp = G.handCount[currentPlayer + 2];
	int playerLeftDiscardCount = testG.discardCount[currentPlayer + 1];
	int playerLeftDiscardCountExp = G.discardCount[currentPlayer + 1];
	int playerRightDiscardCount = testG.discardCount[currentPlayer + 2];
	int playerRightDiscardCountExp = G.discardCount[currentPlayer + 2];

	/*ASSERTIONS*/
	//# hand cards should be one less for discarding minion
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of handcards: PASSED\n");
	} else {
		printf("Correct number of handcards: FAILED\n");
	}
	//# of coins should be 2 more
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	} else {
		printf("Correct number of coins: FAILED\n");
	}
	//should have one more Action
	if (numActions == numActionsExpected) {
		printf("Correct number of Actions: PASSED\n");
	} else {
		printf("Correct number of Actions: FAILED\n");
	}
	//should have one more card in discard from discarded minion
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	} else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//other players should not gain a card in his hand or his discard deck
	if ((playerLeftDiscardCount == playerLeftDiscardCountExp) && (playerRightDiscardCount == playerRightDiscardCountExp) && (playerLeftHandCardCount == playerLeftHandCardCountExp) && (playerRightHandCardCount == playerRightHandCardCountExp)) {
		printf("Card counts of other player not affected: PASSED\n");
	} else {
		printf("Card counts of other player not affected: FAILED\n");
	}

	printf("\nTEST 2: Choice1 = 0, Choice2 = 1: Discard hand, draw 4; 1 player with 5+ does the same\n");
	choice1 = 0; 
	choice2 = 1;

	//initialize all three players hands; all will be discarded from players 0 and 1, but not 2
	G.hand[currentPlayer][0] = minion;
	G.hand[currentPlayer][1] = silver;
	G.hand[currentPlayer][2] = silver;
	G.hand[currentPlayer][3] = silver;
	G.hand[currentPlayer][4] = silver;
	G.handCount[currentPlayer] = 5;

	G.hand[currentPlayer + 1][0] = gold;
	G.hand[currentPlayer + 1][1] = gold;
	G.hand[currentPlayer + 1][2] = gold;
	G.hand[currentPlayer + 1][3] = gold;
	G.hand[currentPlayer + 1][4] = gold;
	G.handCount[currentPlayer + 1] = 5;

	G.hand[currentPlayer + 2][0] = copper;
	G.hand[currentPlayer + 2][1] = copper;
	G.hand[currentPlayer + 2][2] = copper;
	G.hand[currentPlayer + 2][3] = copper;
	G.handCount[currentPlayer + 2] = 4;

	memcpy(&testG, &G, sizeof(struct gameState));
	
	minionEffect(choice1, choice2, &testG, currentPlayer, handPos);
	discarded = 5;
	int drawnCard = 4;

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded + drawnCard;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded; //whole hand discarded
	
	numActions = testG.numActions;
	numActionsExpected = G.numActions + actions;
	int testNumDeck = testG.deckCount[currentPlayer];
	int numDeckExpected = G.deckCount[currentPlayer] - drawnCard;
	int testNumDeckPlayer2 = testG.deckCount[currentPlayer + 1];
	int numDeckExpectedPlayer2 = G.deckCount[currentPlayer + 1] - drawnCard;
	int testNumDeckPlayer3 = testG.deckCount[currentPlayer + 2];
	int numDeckExpectedPlayer3 = G.deckCount[currentPlayer + 2]; //should be the same
	playerLeftHandCardCount = testG.handCount[currentPlayer + 1];
	playerLeftHandCardCountExp = G.handCount[currentPlayer + 1] - discarded + drawnCard;
	playerRightHandCardCount = testG.handCount[currentPlayer + 2];
	playerRightHandCardCountExp = G.handCount[currentPlayer + 2];
	playerLeftDiscardCount = testG.discardCount[currentPlayer + 1];
	playerLeftDiscardCountExp = G.discardCount[currentPlayer + 1] + discarded;
	playerRightDiscardCount = testG.discardCount[currentPlayer + 2];
	playerRightDiscardCountExp = G.discardCount[currentPlayer + 2];

	//# of deck cards should be 4 less than originally
	if (testNumDeck == numDeckExpected) {
		printf("Correct number of deck cards: PASSED\n");
	}
	else {
		printf("Correct number of deck cards: FAILED\n");
	}
	//# hand cards should be one less for discarding 5 cards gaining 4
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of handcards: PASSED\n");
	} else {
		printf("Correct number of handcards: FAILED\n");
	}
	//should have one more Action
	if (numActions == numActionsExpected) {
		printf("Correct number of Actions: PASSED\n");
	}
	else {
		printf("Correct number of Actions: FAILED\n");
	}
	//should have 5 more card in discard from discarded minion and discarded cards
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//Left player's deck should decrease, his discard count should increase, and his hand card count should decrease by 1 (from 5 to 4)
	if ((testNumDeckPlayer2 == numDeckExpectedPlayer2) && (playerLeftDiscardCount == playerLeftDiscardCountExp) && (playerLeftHandCardCount == playerLeftHandCardCountExp)) {
		printf("Card counts of left player changed: PASSED\n");
	}
	else {
		printf("Card counts of left player changed: FAILED\n");
	}
	//Right player's decks should stay the exact same, as they have less than 5 hand cards
	if ((testNumDeckPlayer3 == numDeckExpectedPlayer3) && (playerRightDiscardCount == playerRightDiscardCountExp) && (playerRightHandCardCount == playerRightHandCardCountExp)) {
		printf("Card counts of right player not affected: PASSED\n");
	}
	else {
		printf("Card counts of right player not affected: FAILED\n");
	}

	printf("\n****************TEST MINION CARD COMPLETE*******************\n");

	return 0;
}
