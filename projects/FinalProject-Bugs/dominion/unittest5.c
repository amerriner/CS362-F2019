#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {

    //test variables
    int currentPlayerScore;
    int seed = 1000;

    int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };
	struct gameState state;

    //initialize game
    initializeGame(2, k, seed, &state);

	printf("\n------ Begin testing scoreFor count bug ------\n");

    //specific test variables
    state.handCount[0] = 0;
    state.discardCount[0] = 2;
    state.deck[0][0] = duchy;
    state.deck[0][1] = province;
    state.deck[0][2] = great_hall;
    state.deck[0][3] = estate;

    //scoreFor calculation
    currentPlayerScore = scoreFor(0, &state);

    //assert statement for correct score
    if (currentPlayerScore == 11) {
        printf("Correct score from scoreFor deck count: PASSED\n");
    }
    else {
        printf("Correct score from scoreFor deck count: FAILED\n");
    }

	printf("\n------------------ End Test ------------------\n");
    return 0;
}