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
		printf("\n***********************Begin Random Test Number %d: Tribute*************************\n", x);
		struct gameState G, testG;
		int i;
		int card = -1;
		int cardsFromDiscard = 0;
		int xtraCoins = 0;
		int discarded = 1;
		int otherPlayerDiscard = 0;
		int gainedCards = 0;
		int xtraActions = 0;
		int k[10] = { -1 };
		k[0] = tribute;
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
		int seed = (rand() % 1001) + 1000;
		int currentPlayer = 0;
		int handPos = (rand() % 4) + 1;
		int nextPlayer = currentPlayer + 1;

		initializeGame(numPlayers, k, seed, &G);
		//0 = 0 cards in discard, 1 = 1 card in discard, 2 = multiple cards in discard
		int cardsInDiscard = (rand() % 3);
		printf("cardsindiscard: %d\n", cardsInDiscard);
		if (cardsInDiscard == 0) {
			G.discardCount[nextPlayer] = 0;
		}
		else if (cardsInDiscard == 1) {
			G.discardCount[nextPlayer] = 1;
		}
		else {
			G.discardCount[nextPlayer] = 5;
			for (int l = 0; l < 5; l++) {
				int otherNum2 = (rand() % 27) + 1;
				G.deck[nextPlayer][l] = otherNum2;
			}
		}
		G.hand[currentPlayer][handPos] = tribute;
		randNum = (rand() % 6);
		printf("Cards in deck: %d\n", randNum);
		G.deckCount[nextPlayer] = randNum;
		for (int q = 0; q < randNum; q++) {
			int otherNum = (rand() % 27) + 1;
			G.deck[nextPlayer][q] = otherNum;
		}
		printf("Current player deck count: %d, Other player deck count: %d, cards in discard: %d, Deck card 1: %d, deck card 2: %d\n", G.deckCount[currentPlayer], G.deckCount[nextPlayer], G.discardCount[nextPlayer], G.deck[nextPlayer][0], G.deck[nextPlayer][1]);
		if (randNum >= 1) {
			//at least 1 card in deck or discard to reveal
			if (randNum == 1 && G.discardCount[nextPlayer] == 0) {
				otherPlayerDiscard = 1;
			}
			else {
				otherPlayerDiscard = 2;
			}
			for (int j = 0; j < otherPlayerDiscard; j++) {
				card = G.deck[nextPlayer][G.deckCount[nextPlayer] - (j + 1)];
				//if same card
				if (j == 1 && card == G.deck[nextPlayer][G.deckCount[nextPlayer] - (j - 1)]) {
					continue;
				}
				else if (card == copper || card == silver || card == gold) {
				
					xtraCoins += 2;
				}
				else if (card == estate || card == duchy || card == province || card == gardens || card == great_hall) {
				
					gainedCards += 2;
				}
				else {
				
					xtraActions += 2;
				}
			}
		}
		else {
			//if 0 in deck, 1 in discard, discard count stays the same (1 out, then back in)
			if (G.discardCount[nextPlayer] == 1) {
				if (card == copper || card == silver || card == gold) {
					xtraCoins += 2;
				}
				else if (card == estate || card == duchy || card == province || card == gardens || card == great_hall) {
					gainedCards += 2;
				}
				else {
					xtraActions += 2;
				}
			}
			//if 0 in deck, multiple in discard
			else if (G.discardCount[nextPlayer] >= 2) {
				cardsFromDiscard = G.discardCount[nextPlayer];
				otherPlayerDiscard = 2;
				for (int j = 0; j < otherPlayerDiscard; j++) {
					card = G.deck[nextPlayer][j];
					if (j == 1 && card == G.deck[nextPlayer][0]) {
						continue;
					}
					else if (card == copper || card == silver || card == gold) {
						xtraCoins += 2;
					}
					else if (card == estate || card == duchy || card == province || card == gardens || card == great_hall) {
						gainedCards += 2;
					}
					else {
						xtraActions += 2;
					}
				}
			}
		}

		memcpy(&testG, &G, sizeof(struct gameState));
		tributeRefactor(currentPlayer, &testG, nextPlayer);

		int testNumHandCards = testG.handCount[currentPlayer];
		int expectedNumHandCards = G.handCount[currentPlayer] - discarded + gainedCards;
		//printf("Hand cards: %d, Expected: %d\n", testNumHandCards, expectedNumHandCards);
		int testNumCoins = testG.coins;
		int expectedNumCoins = G.coins + xtraCoins;
		//printf("Coins: %d, Expected: %d\n", testNumCoins, expectedNumCoins);
		int numActions = testG.numActions;
		int numActionsExpected = G.numActions + xtraActions;
		//printf("Actions: %d, Expected: %d\n", numActions, numActionsExpected);
		int discardTest = testG.discardCount[currentPlayer];
		int discardExpected = G.discardCount[currentPlayer] + discarded;
		int otherPlayerDeckCardCount = testG.deckCount[nextPlayer];
		int otherPlayerDeckCardCountExp = G.deckCount[nextPlayer] - otherPlayerDiscard + cardsFromDiscard;
		int otherPlayerDiscardCount = testG.discardCount[nextPlayer];
		int otherPlayerDiscardCountExp = G.discardCount[nextPlayer] - cardsFromDiscard + otherPlayerDiscard;

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
		if (numActions == numActionsExpected) {
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
		if (otherPlayerDiscardCount == otherPlayerDiscardCountExp) {
			printf("Discard counts of other player correct: PASSED\n");
		}
		else {
			printf("Discard counts of other player correct: FAILED\n");
		}
		if (otherPlayerDeckCardCount == otherPlayerDeckCardCountExp) {
			printf("Deck counts of other player correct: PASSED\n");
		}
		else {
			printf("Deck counts of other player correct: FAILED\n");
		}

		printf("\n*************************END TEST NUMBER %d: Tribute****************************\n", x);
		x++;
	}
	return 0;
}