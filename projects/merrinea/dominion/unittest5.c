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
	int handPos = 0, choice1 = 0, choice2 = 0, currentPlayer = 0;
	initializeGame(numPlayers, k, seed, &G);

	printf("******************BEGIN TEST MINE CARD****************\n");

	printf("Test 1: Choice1 = copper, choice2 = Gold: Golden Path Correct Card Swap\n");
	
	//instantiate current player hand
	G.hand[currentPlayer][0] = mine;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = baron;
	G.hand[currentPlayer][3] = estate;
	G.handCount[currentPlayer] = 4;
	G.coins = 1;

	//changing out copper
	choice1 = 1;
	//for silver
	choice2 = silver;
	memcpy(&testG, &G, sizeof(struct gameState));
	int result = mineEffect(&testG, currentPlayer, choice1, choice2, handPos);
	discarded = 2;
	int costHandCard = getCost(G.hand[currentPlayer][1]);
	int costReplacingCard = getCost(choice2);
	int costDiff = costReplacingCard - costHandCard;
	int maxCostDiff = 3;
	int gainedCards = 1;
	xtraCoins = 1;

	int testNumHandCards = testG.handCount[currentPlayer];
	int expectedNumHandCards = G.handCount[currentPlayer] - discarded + gainedCards;
	int testNumCoins = testG.coins;
	int expectedNumCoins = G.coins + xtraCoins;
	int discardTest = testG.discardCount[currentPlayer];
	int discardExpected = G.discardCount[currentPlayer] + discarded;
	int otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	int otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1];
	int otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	int otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1];
	int numSupplyCountSilver = testG.supplyCount[silver];
	int numSupplyCountSilverExp = G.supplyCount[silver] - gainedCards;

	/*ASSERTIONS*/
	//Make sure beginning assertions pass and work
	if (result != -1) {
		printf("Choice 1 is a treasure card: PASSED\n");
	}
	else {
		printf("Choice 1 is a treasure card: FAILED\n");
	}
	if (result != -2) {
		printf("Choice 2 is a treasure card: PASSED\n");
	}
	else {
		printf("Choice 2 is a treasure card: FAILED\n");
	}
	if (result != -3) {
		printf("Price assertion check: PASSED\n");
	}
	else {
		printf("Price assertion check: PASSED\n");
	}
	//Make sure card allowed to trade is correct price
	if (costDiff <= maxCostDiff) {
		printf("Choice 2 costs 3 coins or less more than choice 1: PASSED\n");
	}
	else {
		printf("Choice 2 costs 3 coins or less more than choice 1: FAILED\n");
	}
	//Make sure number of cards in hand is 1 less
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//Make sure number of coins at end is correct, should be 1 more than before
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//Make sure number of cards in discard pile is correct, should be 2 more
	if (discardTest == discardExpected) {
		printf("Correct number of cards in discard pile: PASSED\n");
	}
	else {
		printf("Correct number of cards in discard pile: FAILED\n");
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

	printf("\nTest 2: Choice1 = copper, Choice2 = gold: Too expensive\n");

	//instantiate current player hand
	G.hand[currentPlayer][0] = mine;
	G.hand[currentPlayer][1] = silver;
	G.hand[currentPlayer][2] = baron;
	G.hand[currentPlayer][3] = estate;
	G.handCount[currentPlayer] = 4;
	G.coins = 2;

	//changing out silver
	choice1 = 1;
	//for gold
	choice2 = gold;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = mineEffect(&testG, currentPlayer, choice1, choice2, handPos);
	costHandCard = getCost(G.hand[currentPlayer][1]);
	costReplacingCard = getCost(choice2);
	costDiff = costReplacingCard - costHandCard;
	maxCostDiff = 3;
	xtraCoins = 0;

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer]; //should be the same, could not continue
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins; //should be the same, couldn't continue 
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer]; //should not have been able to discard 
	otherPlayerHandCardCount = testG.handCount[currentPlayer + 1];
	otherPlayerHandCardCountExp = G.handCount[currentPlayer + 1]; 
	otherPlayerDiscardCount = testG.discardCount[currentPlayer + 1];
	otherPlayerDiscardCountExp = G.discardCount[currentPlayer + 1]; //other player's cards would not be affected
	int numSupplyCountGold = testG.supplyCount[gold];
	int numSupplyCountGoldExp = G.supplyCount[gold]; //shouldn't make it far enough to take card from supply

	/*ASSERTIONS*/
	//both choices should be treasure cards
	if (result != -1) {
		printf("Choice 1 is a treasure card: PASSED\n");
	}
	else {
		printf("Choice 1 is a treasure card: FAILED\n");
	}
	if (result != -2) {
		printf("Choice 2 is a treasure card: PASSED\n");
	}
	else {
		printf("Choice 2 is a treasure card: FAILED\n");
	}
	//the cost difference between choice 1 and choice 2 should be no more than 3 coins
	if (costDiff <= maxCostDiff) {
		printf("Choice 2 costs 3 coins or less more than choice 1: PASSED\n");
	}
	else {
		printf("Choice 2 costs 3 coins or less more than choice 1: FAILED\n");
	}
	//# cards in hand should be the same, function should not continue past cost check
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//#number coins should be the same, no swap should occur
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//No cards should be discarded, function should end at cost check
	if (discardTest == discardExpected) {
		printf("No cards discarded: PASSED\n");
	}
	else {
		printf("No cards discarded: FAILED\n");
	}
	//other player's cards should not be tammpered with
	if ((otherPlayerDiscardCount == otherPlayerDiscardCountExp) && (otherPlayerHandCardCount == otherPlayerHandCardCountExp)) {
		printf("Card counts of other player not affected: PASSED\n");
	}
	else {
		printf("Card counts of other player not affected: FAILED\n");
	}
	//supply count of gold should be the same, as swap should not have happened
	if (numSupplyCountGold == numSupplyCountGoldExp) {
		printf("Supply count of Gold cards stayed the same: PASSED\n");
	}
	else {
		printf("Supply count of Gold cards stayed the same: FAILED\n");
	}

	printf("\nTest 3: Choice1 = copper, Choice2 = baron: Choice2 not a treasure card\n");

	//instantiate current player hand
	G.hand[currentPlayer][0] = mine;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = baron;
	G.hand[currentPlayer][3] = estate;
	G.handCount[currentPlayer] = 4;
	G.coins = 1;

	//changing out silver
	choice1 = 1;
	//for baron
	choice2 = baron;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = mineEffect(&testG, currentPlayer, choice1, choice2, handPos);
	costHandCard = getCost(G.hand[currentPlayer][1]);
	costReplacingCard = getCost(choice2);
	costDiff = costReplacingCard - costHandCard;
	maxCostDiff = 3;
	xtraCoins = 0;

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer]; //should be the same, could not continue
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins; //should be the same, couldn't continue 
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer]; //should not have been able to discard 
	int numSupplyCountBaron = testG.supplyCount[baron];
	int numSupplyCountBaronExp = G.supplyCount[baron]; //shouldn't make it far enough to take card from supply

	//First should return 0, is a treasure card
	if (result != -1) {
		printf("Choice 1 is a treasure card: PASSED\n");
	}
	else {
		printf("Choice 1 is a treasure card: FAILED\n");
	}//This should return -2
	if (result == -2) {
		printf("Choice 2 is not a treasure card: PASSED\n");
	}
	else {
		printf("Choice 2 is not a treasure card: FAILED\n");
	}
	//the cost difference between choice 1 and choice 2 should be no more than 3 coins
	if (costDiff <= maxCostDiff) {
		printf("Choice 2 costs 3 coins or less more than choice 1: PASSED\n");
	}
	else {
		printf("Choice 2 costs 3 coins or less more than choice 1: FAILED\n");
	}
	//# cards in hand should be the same, function should not continue past type check
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//#number coins should be the same, no swap should occur becuase type is wrong
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//should not have been able to take a baron from the supply stack
	if (numSupplyCountBaron == numSupplyCountBaronExp) {
		printf("Supply count of Baron cards stayed the same: PASSED\n");
	}
	else {
		printf("Supply count of Baron cards stayed the same: FAILED\n");
	}

	printf("\nTest 4: Choice1 = baron, Choice2 = gold: Choice1 not a treasure card\n");

	//instantiate current player hand
	G.hand[currentPlayer][0] = mine;
	G.hand[currentPlayer][1] = copper;
	G.hand[currentPlayer][2] = baron;
	G.hand[currentPlayer][3] = estate;
	G.handCount[currentPlayer] = 4;
	G.coins = 1;

	//changing out baron
	choice1 = 2;
	//for gold
	choice2 = gold;
	memcpy(&testG, &G, sizeof(struct gameState));
	result = mineEffect(&testG, currentPlayer, choice1, choice2, handPos);
	costHandCard = getCost(G.hand[currentPlayer][2]);
	costReplacingCard = getCost(choice2);
	costDiff = costReplacingCard - costHandCard;
	maxCostDiff = 3;
	xtraCoins = 0;

	testNumHandCards = testG.handCount[currentPlayer];
	expectedNumHandCards = G.handCount[currentPlayer]; //should be the same, could not continue
	testNumCoins = testG.coins;
	expectedNumCoins = G.coins + xtraCoins; //should be the same, couldn't continue 
	discardTest = testG.discardCount[currentPlayer];
	discardExpected = G.discardCount[currentPlayer]; //should not have been able to discard 
	numSupplyCountGold = testG.supplyCount[gold];
	numSupplyCountGoldExp = G.supplyCount[gold]; //shouldn't make it far enough to take card from supply

	//First should return -1, is not a treasure card, should end function
	if (result == -1) {
		printf("Choice 1 is not a treasure card: PASSED\n");
	}
	else {
		printf("Choice 1 is not a treasure card: FAILED\n");
	}
	//the cost difference between choice 1 and choice 2 should be no more than 3 coins
	if (costDiff <= maxCostDiff) {
		printf("Choice 2 costs 3 coins or less more than choice 1: PASSED\n");
	}
	else {
		printf("Choice 2 costs 3 coins or less more than choice 1: FAILED\n");
	}
	//# cards in hand should be the same, function should not continue past type check
	if (testNumHandCards == expectedNumHandCards) {
		printf("Correct number of hand cards: PASSED\n");
	}
	else {
		printf("Correct number of hand cards: FAILED\n");
	}
	//#number coins should be the same, no swap should occur becuase type is wrong
	if (testNumCoins == expectedNumCoins) {
		printf("Correct number of coins: PASSED\n");
	}
	else {
		printf("Correct number of coins: FAILED\n");
	}
	//should not have been able to take a gold from the supply stack
	if (numSupplyCountGold == numSupplyCountGoldExp) {
		printf("Supply count of Gold cards stayed the same: PASSED\n");
	}
	else {
		printf("Supply count of Gold cards stayed the same: FAILED\n");
	}
	
	return 0;

}	
