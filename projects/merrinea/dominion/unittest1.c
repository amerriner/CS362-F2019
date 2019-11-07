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
	int xtraCoins, discarded;
	int seed = 1000;
	int handPos = 0, choice1 = 1, currentPlayer = 0;
	initializeGame(numPlayers, k, seed, &G);

	printf("****************Testing Card: Baron****************\n\n\n");

	//Test 1: coins gained, baron and estate discarded, golden path desired
	printf("TEST 1: Choice1 = 1: Discard Estate for Coins\n");
	//set hand so we know that player will have the right cards for golden path test
	G.hand[currentPlayer][0] = baron;
	G.hand[currentPlayer][1] = estate;
	G.hand[currentPlayer][2] = estate;
	G.hand[currentPlayer][3] = estate;
	G.hand[currentPlayer][4] = estate;

	memcpy(&testG, &G, sizeof(struct gameState));
	baronEffect(choice1, &testG, currentPlayer, handPos);
	xtraCoins = 4;
	discarded = 2;
	//test assertion variables: number of hand cards decreased, number of coins increased, number of buys increased,
	//discard pile grew, other player's hand and discard count stayed the same, state's supply count for 
	//estate and baron cards stayed the same
	int testNumHandCards = testG.handCount[currentPlayer];
	int expectedNumHandCards = G.handCount[currentPlayer] - discarded;
	int testNumCoins = testG.coins;
	int expectedNumCoins = G.coins + xtraCoins;
	int numBuys = testG.numBuys;
	int numBuysExpected = G.numBuys + 1;
	int discardTest = testG.discardCount[currentPlayer];
	int discardExpected = G.discardCount[currentPlayer] + discarded;
	int otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	int otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1];
	int otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	int otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1];
	int numSupplyCountEstate = testG.supplyCount[estate];
	int numSupplyCountEstateExp = G.supplyCount[estate];
	int numSupplyCountBaron = testG.supplyCount[baron];
	int numSupplyCountBaronExp = G.supplyCount[baron];
	
	/*ASSERTIONS*/
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of handcards: PASSED\n");
	}
	else {
		printf("Correct number of handcards: FAILED\n");
	}
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	if (numBuys == numBuysExpected) {
		printf("Correct number of buys: PASSED\n");
	}
	else {
		printf("Correct number of buys: FAILED\n");
	}
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerHandCardCount == otherPlayerHandCardCountExp)) {
		printf("Card counts of other player not affected: PASSED\n");
	}
	else {
		printf("Card counts of other player not affected: FAILED\n");
	}
	if ((numSupplyCountBaron == numSupplyCountBaronExp) && (numSupplyCountEstate == numSupplyCountEstateExp)) {
		printf("Supply count of Baron and Estate cards did not change: PASSED\n");
	}
	else {
		printf("Supply count of Baron and Estate cards did not change: FAILED\n");
	}

	printf("\nTEST 2: Choice1 = 1: Attempt to Discard Estate for Coins Without Any Estates in Hand and No Estates in Supply Count\n");
	//set hand so we know that player will have the right cards
	G.hand[currentPlayer][0] = baron;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = copper;
	G.hand[currentPlayer][3] = copper;
	G.hand[currentPlayer][4] = copper;
	
	G.supplyCount[estate] = 0;

	memcpy(&testG, &G, sizeof(struct gameState));
	baronEffect(choice1, &testG, currentPlayer, handPos);
	discarded = 1;
	
	int i;
	//check to make sure player gained an estate card
	int estateCardInHand = 0;
	for (i = 0; i < 5; i++) {
		if (testG.hand[currentPlayer][i] == 1) {
			estateCardInHand++;
		}
	}

	//Testing variables
	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer] - discarded; //should not be equal, couldn't gain estate and lost baron
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins; //should not gain any, as no estate to discard
	numBuys = testG.numBuys;
	numBuysExpected = G.numBuys + 1;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1];
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1];
	numSupplyCountEstate = testG.supplyCount[estate];
	numSupplyCountEstateExp = G.supplyCount[estate]; //no estates gained or lost
	numSupplyCountBaron = testG.supplyCount[baron];
	numSupplyCountBaronExp = G.supplyCount[baron];

	/*ASSERTIONS*/

	//should not be able to get an estate card from the supply, as supply is set to 0
	if (estateCardInHand == 0) {
		printf("No estate cards in hand: PASSED\n");
	} else {
		printf("No estate cards in hand: FAILED\n");
	}
	//# hand cards should be one less for discarding baron
	if (testNumHandCards == expectedNumHandCards) { 
		printf("Correct number of handcards: PASSED\n");
	} else {
		printf("Correct number of handcards: FAILED\n");
	}
	//# of coins should be the same, no estate in hand to discard for coins
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	} else {
		printf("Correct number of coins: FAILED\n");
	}
	//should have one more buy
	if (numBuys == numBuysExpected) {
		printf("Correct number of buys: PASSED\n");
	} else {
		printf("Correct number of buys: FAILED\n");
	}
	//should have one more card in discard from discarded Baron
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	} else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//other player should not gain a card in his hand or his discard deck
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerHandCardCount == otherPlayerHandCardCountExp)) {
		printf("Card counts of other player not affected: PASSED\n");
	} else {
		printf("Card counts of other player not affected: FAILED\n");
	}
	//state supply should not change, as no estate cards in supply to give
	if ((numSupplyCountBaron == numSupplyCountBaronExp) && (numSupplyCountEstate == numSupplyCountEstateExp)) {
		printf("Supply count of Baron and Estate cards did not change: PASSED\n");
	} else {
		printf("Supply count of Baron and Estate cards did not change: FAILED\n");
	}
	
	printf("\nTEST 3: Choice1 = 0: Choose to gain an Estate Card with Esate Cards in Supply\n");
	//player chooses to gain an estate card, choice1 set to false
	choice1 = 0;
	//set hand so we know that player will have the right cards
	G.hand[currentPlayer][0] = baron;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = copper;
	G.hand[currentPlayer][3] = copper;
	G.hand[currentPlayer][4] = copper;

	//resupply supplyCount of estates from last test
	G.supplyCount[estate] = 8;
	memcpy(&testG, &G, sizeof(struct gameState));
	baronEffect(choice1, &testG, currentPlayer, handPos);
	discarded = 1;

	//check to make sure player gained an estate card
	estateCardInHand = 0;
	for (i = 0; i < 5; i++) {
		if (testG.hand[currentPlayer][i] == 1) {
			estateCardInHand++;
		}
	}
	discarded = 1;
	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer]; //should be equal, as discarded baron and gained estate
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins; //should not gain any, as no estate to discard
	numBuys = testG.numBuys;
	numBuysExpected = G.numBuys + 1;
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer] + discarded;
	otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1];
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1];
	numSupplyCountEstate = testG.supplyCount[estate];
	numSupplyCountEstateExp = G.supplyCount[estate] - 1; //estate gained, should decrement from supply count
	numSupplyCountBaron = testG.supplyCount[baron];
	numSupplyCountBaronExp = G.supplyCount[baron];

	/*ASSERTIONS*/
	//should have one estate card in hand from supply 
	if (estateCardInHand == 1) {
		printf("One estate card in hand: PASSED\n");
	} else {
		printf("One estate card in hand: FAILED\n");
	}
	//should have the same number of hand cards as before: gained estate, discarded baron
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of handcards: PASSED\n");
	} else {
		printf("Correct number of handcards: FAILED\n");
	}
	//should not have any more coins, did not discard estate
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	} else {
		printf("Correct number of coins: FAILED\n");
	}
	//should have one more buy
	if (numBuys == numBuysExpected) {
		printf("Correct number of buys: PASSED\n");
	} else {
		printf("Correct number of buys: FAILED\n");
	}
	//discard count should be one higher
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	} else {
		printf("Correct number of cards in discard pile: FAILED\n");
	}
	//other player's discard and hand count should remain the same
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerHandCardCount == otherPlayerHandCardCountExp)) {
		printf("Card counts of other player not affected: PASSED\n");
	} else {
		printf("Card counts of other player not affected: FAILED\n");
	}
	//supply's estate count should be one less from giving it to player
	if ((numSupplyCountBaron == numSupplyCountBaronExp) && (numSupplyCountEstate == numSupplyCountEstateExp)) {
		printf("Supply count of Baron and Estate cards did not change: PASSED\n");
	} else {
		printf("Supply count of Baron and Estate cards did not change: FAILED\n");
	}

	printf("\n****************TEST BARON CARD COMPLETE****************\n\n");
	return 0;
}	