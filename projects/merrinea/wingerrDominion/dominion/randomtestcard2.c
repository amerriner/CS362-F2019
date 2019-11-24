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
		printf("\n***********************Begin Random Test Number %d: MINION*************************\n", x);
		struct gameState G, testG;
		int i;
		int xtraCoins = 0;
		int discarded = 0;
		int gainedCard = 0;
		int actionsGained = 1;
		int choice2Discard = 0;
		int nextPlayer1Discarded = 0;
		int nextPlayer2Discarded = 0;
		int nextPlayer3Discarded = 0;
		int nextPlayer1GainedCards = 0;
		int nextPlayer2GainedCards = 0;
		int nextPlayer3GainedCards = 0;
		int k[10] = { -1 };
		k[0] = minion;
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
		int choice2 = 0;
		if (choice1 == 0) {
			choice2 = 1;
		}
		int seed = (rand() % 1001) + 1000;
		int currentPlayer = 0;
		int handPos = (rand() % 4) + 1;
		int nextPlayer1 = currentPlayer + 1;
		int nextPlayer2 = currentPlayer + 2;
		int nextPlayer3 = currentPlayer + 3;
		int nextPlayer2HandCardCount = 0;
		int nextPlayer2HandCardCountExp = 0;
		int nextPlayer2DiscardCount = 0;
		int nextPlayer2DiscardCountExp = 0;
		int nextPlayer3HandCardCount = 0;
		int nextPlayer3HandCardCountExp = 0;
		int nextPlayer3DiscardCount = 0;
		int nextPlayer3DiscardCountExp = 0;

		initializeGame(numPlayers, k, seed, &G);
		G.hand[currentPlayer][handPos] = minion;
		for (int z = 1; z < numPlayers; z++) {
			G.handCount[currentPlayer + z] = (rand() % 5) + 1;
			for (int y = 0; y < G.handCount[currentPlayer + z]; y++) {
				G.hand[currentPlayer + z][y] = (rand() % 27);
			}
		}
		if (choice1 == 1) {
			xtraCoins = 2;
			discarded = 1;
			choice2Discard = 1;
		}
		else {
			discarded = G.handCount[currentPlayer];
			gainedCard = 4;
			int j;
			for (j = 1; j < numPlayers; j++) {
				if (G.handCount[j] >= 5) {
					if (j == 1) {
						nextPlayer1Discarded = G.handCount[j];
						nextPlayer1GainedCards = 4;
					}
					else if (j == 2) {
						nextPlayer2Discarded = G.handCount[j];
						nextPlayer2GainedCards = 4;
					}
					else {
						nextPlayer3Discarded = G.handCount[j];
						nextPlayer3GainedCards = 4;
					}
				}
			}
		}

		memcpy(&testG, &G, sizeof(struct gameState));
		minionRefactor(currentPlayer, &testG, choice1, choice2, handPos);
		printf("Testing Choice1 = %d, Choice2 = %d, NumPlayers = %d\n", choice1, choice2, numPlayers);
		
		int testNumHandCards = testG.handCount[currentPlayer];
		int expectedNumHandCards = G.handCount[currentPlayer] - discarded + gainedCard;
		int testNumActions = testG.numActions;
		int expectedNumActions = G.numActions + actionsGained;
		int testNumCoins = testG.coins;
		int expectedNumCoins = G.coins + xtraCoins;
		int discardTest = testG.discardCount[currentPlayer];
		int discardExpected = G.discardCount[currentPlayer] + choice2Discard;
		int nextPlayer1HandCardCount = testG.handCount[nextPlayer1];
		int nextPlayer1HandCardCountExp = G.handCount[nextPlayer1] - nextPlayer1Discarded + nextPlayer1GainedCards;
		int nextPlayer1DiscardCount = testG.discardCount[nextPlayer1];
		int nextPlayer1DiscardCountExp = G.discardCount[nextPlayer1] + nextPlayer1Discarded;
		if (numPlayers >= 3) {
			nextPlayer2HandCardCount = testG.handCount[nextPlayer2];
			nextPlayer2HandCardCountExp = G.handCount[nextPlayer2] - nextPlayer1Discarded + nextPlayer1GainedCards;
			nextPlayer2DiscardCount = testG.discardCount[nextPlayer2];
			nextPlayer2DiscardCountExp = G.discardCount[nextPlayer2] + nextPlayer1Discarded;
		}
		if (numPlayers == 4) {
			nextPlayer3HandCardCount = testG.handCount[nextPlayer3];
			nextPlayer3HandCardCountExp = G.handCount[nextPlayer3] - nextPlayer1Discarded + nextPlayer1GainedCards;
			nextPlayer3DiscardCount = testG.discardCount[nextPlayer3];
			nextPlayer3DiscardCountExp = G.discardCount[nextPlayer3] + nextPlayer1Discarded;
		}

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
		if (testNumActions == expectedNumActions) {
			printf("Correct number of actions: PASSED\n");
		}
		else {
			printf("Correct number of actions: FAILED\n");
		}
		if (discardTest == discardExpected) {
			printf("Correct number of cards in discard pile: PASSED\n");
		}
		else {
			printf("Correct number of cards in discard pile: FAILED\n");
		}
		if ((nextPlayer1DiscardCount == nextPlayer1DiscardCountExp) && (nextPlayer2DiscardCount == nextPlayer2DiscardCountExp) && (nextPlayer3DiscardCount == nextPlayer3DiscardCountExp)) {
			printf("Discard counts of other player correct: PASSED\n");
		}
		else {
			printf("Discard counts of other player correct: FAILED\n");
		}
		if ((nextPlayer1HandCardCount == nextPlayer1HandCardCountExp) && (nextPlayer2HandCardCount == nextPlayer2HandCardCountExp) && (nextPlayer3HandCardCount == nextPlayer3HandCardCountExp)) {
			printf("Hand counts of other players correct: PASSED\n");
		}
		else {
			printf("Hand counts of other players correct: FAILED\n");
		}
		x++;
	}

	return 0;
}
			


