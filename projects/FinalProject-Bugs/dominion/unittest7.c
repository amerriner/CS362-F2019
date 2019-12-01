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
    int handPos = 0;
    int currentCoins = 0;
    int seed = 1000;

    int k[10] = { tribute, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;

    //initialize game
    initializeGame(2, k, seed, &state);

	printf("\n---- Begin testing Tribute array loop bug ----\n");

    //specific test variables
    currentCoins = state.coins;
    state.discard[1][0] = estate;
    state.discard[1][1] = estate;
    state.discard[1][2] = gold;

    //call tribute card
    cardEffect(19, choice1, choice2, 0, &state, handPos, 0);

    //assert for coin count updated correctly
    if ((state.coins != (currentCoins + 2))) {
        printf("Player coin count increased correctly: PASSED\n");
    }
    else {
        printf("Player coin count increased correctly: FAILED\n");
    }

	printf("\n------------------ End Test ------------------\n");
    return 0;
}