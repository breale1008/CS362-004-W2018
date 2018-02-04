#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in endTurnTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

void endTurnTest(int seed, int players, int verboseMode) {
	
	if (verboseMode)
		printf("unit test start: endTurnTest");

	struct gameState G;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };
	initializeGame(players, k, seed, &G);
	
	// get current player (whose turn before advancement)
	int wTurn = whoseTurn(&G);

	// >>>> go to next turn <<<<
	int n = endTurn(&G);

	// test player turn advancement
	testAssert((wTurn != whoseTurn(&G)), "player turn not advanced", 0, verboseMode);
	
	// test next player's hand drawn
	testAssert((numHandCards(&G) == 5), "next player's hand not drawn", 0, verboseMode);

	// test previous hand is discarded
	testAssert((G.handCount[wTurn] == 0), "previous hand not discarded", 0, verboseMode);

	// test numActions
	testAssert((G.numActions == 1), "action count not reset", 0, verboseMode);

	// test numBuys
	testAssert((G.numBuys == 1), "buy count not reset", 0, verboseMode);
}

int main() {

	int seed, players;
	for (seed = 1; seed < 100; seed++) {

		for (players = 2; players < 5; players++) {

			endTurnTest(seed, players, 0);
		}
		
	}

	return 0;
}