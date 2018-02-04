#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in discardCardTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

int discardCardTest(int handPos, int trashFlag, int seed, int players, int verboseMode) {

	struct gameState G, Gpre;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };
	initializeGame(players, k, seed, &G); // will draw 5 cards
	
	int wTurn = whoseTurn(&G);

	// take snapshot of game pre-discard (Gpre)
	memcpy(&Gpre, &G, sizeof(struct gameState));
	
	// >>>> discard card <<<<
	int n = discardCard(handPos, wTurn, &G, trashFlag);
	
	if (trashFlag < 1) {
	
		testAssert((Gpre.playedCardCount + 1 == G.playedCardCount), "incorrect discard count after discard", 0, verboseMode);
	}
	else {

		testAssert((Gpre.playedCardCount == G.playedCardCount), "incorrect discard count after trash", 0, verboseMode);
	}
	
	testAssert((numHandCards(&Gpre) - 1 == numHandCards(&G)), "incorrect current hand count", 0, verboseMode);

	return 0;
}

int main() {

	int handPos, trashFlag, seed = 1, players;

	for (handPos = 0; handPos < 5; handPos++) {
		for (players = 2; players < 5; players++) {
			for (trashFlag = 0; trashFlag < 2; trashFlag++) {
				for (seed = 1; seed < 20; seed++) {
					discardCardTest(handPos, trashFlag, seed, players, 0);
				}
			}
		}
	}

	return 0;
}