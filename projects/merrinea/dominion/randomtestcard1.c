#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main() {
	int x = 1;
	srand(time(NULL));
	while (x < 1000) {
		printf("\n***********************Begin Random Test Number %d: BARON*************************\n", x);
		struct gameState G, testG;
		int i;
		int xtraCoins = 0; 
		int discarded = 0;
		int gainedCard = 0;
		int k[10] = { -1 };
		k[0] = baron;
		int xtraBuys = 1;
		int nums[26] = { 0 };
		nums[15] = 1;
		int randNum = 0;
		for (i = 1; i < 10; i++) {
			randNum = (rand() % (18)) + 7;
		
			while (nums[randNum] == 1) {
				randNum = (rand() % (18)) + 7;
			}
			k[i] = randNum;
			nums[randNum] = 1;
		}

		int numPlayers = (rand() % 3) + 2;
		int choice1 = (rand() % 2);
		int seed = (rand() % 1001) + 1000;
		int currentPlayer = 0;
		//printf("Current player: %d\n", currentPlayer);
		int handPos = (rand() % 4) + 1;
		//printf("NumPlayers = %d, choice1 = %d, handPos = %d\n", numPlayers, choice1, handPos);
		int nextPlayer = currentPlayer + 1;

		initializeGame(numPlayers, k, seed, &G);
		G.handCount[currentPlayer] = 5;
		randNum = 0;
		int j;
		for (j = 0; j < 5; j++) {
			randNum = (rand() % 27);
			G.hand[currentPlayer][j] = randNum;
		}
		
		G.hand[currentPlayer][handPos] = baron;
		int noSupplyEstates = (rand() % 2);
		if (noSupplyEstates == 1) {
			G.supplyCount[estate] = 0;
		}
		//printf("handCount: %d\n", G.handCount[currentPlayer]);
		if (choice1 == 1) {
			int m;
			int s = 0;
			for (m = 0; m < G.handCount[currentPlayer]; m++) {
				if (G.hand[currentPlayer][m] == estate) {
					s++;
				}
			}
			//printf("handCount: %d\n", G.handCount[currentPlayer]);
			//printf("num esates in hand: %d\n", s);
			if (s >= 1) {
				xtraCoins = 4;
				discarded = 2;
			}
			else {
				discarded = 1;
				if (!noSupplyEstates) {
					gainedCard = 1;
				}
			}
			//printf("handCount: %d\n", G.handCount[currentPlayer]);
		}
		else {
			if (!noSupplyEstates) {
				gainedCard = 1;
			}
			discarded = 1;
		}
		
		memcpy(&testG, &G, sizeof(struct gameState));
		baronEffect(choice1, &testG, currentPlayer, handPos);

		int testNumHandCards = testG.handCount[currentPlayer];
		int expectedNumHandCards = G.handCount[currentPlayer] - discarded + gainedCard;
		int testNumCoins = testG.coins;
		int expectedNumCoins = G.coins + xtraCoins;
		int numBuys = testG.numBuys;
		int numBuysExpected = G.numBuys + xtraBuys;
		int discardTest = testG.discardCount[currentPlayer];
		int discardExpected = G.discardCount[currentPlayer] + discarded;
		int otherPlayerHandCardCount = testG.handCount[nextPlayer];
		int otherPlayerHandCardCountExp = G.handCount[nextPlayer];
		int otherPlayerDiscardCount = testG.discardCount[nextPlayer];
		int otherPlayerDiscardCountExp = G.discardCount[nextPlayer];
		int numSupplyCountEstate = testG.supplyCount[estate];
		int numSupplyCountEstateExp = G.supplyCount[estate] - gainedCard;
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
			printf("Supply count of Baron and Estate cards correct: PASSED\n");
		}
		else {
			printf("Supply count of Baron and Estate cards correct: FAILED\n");
		}

		printf("\n*************************END TEST NUMBER %d: BARON****************************\n", x);
		x++;
	}

	return 0;
}
