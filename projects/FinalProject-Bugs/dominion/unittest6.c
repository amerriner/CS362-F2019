#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    //test variables
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int handPos = 0;
    int currentCoins, currentHandSize = 0;
    int seed = 1000;

    int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;

    //initialize game
    initializeGame(2, k, seed, &state);

	printf("\n----- Begin testing Feast gain card bug -----\n");

    //pre function call coin count
    currentCoins = state.coins;

    //call feast card
    cardEffect(9, choice1, choice2, choice3, &state, handPos, 0);

    //assert for coin updating
    if (state.coins == currentCoins) {
        printf("Coin count correct after Feast: PASSED\n");
    }
    else {
        printf("Coin count correct after Feast: FAILED\n");
    }

    state.coins = 10;
    currentHandSize = state.handCount[0];
    cardEffect(9, choice1, choice2, choice3, &state, handPos, 0);

    //assert for hand size updating
    if (state.handCount[0] = (currentHandSize + 1)) {
        printf("Card gained if cost less than 5: FAILED\n");
    }
    else {
        printf("Card gained if cost less than 5: PASSED \n");
    }

	printf("\n------------------ End Test ------------------\n");
    return 0;
}