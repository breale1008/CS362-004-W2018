#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in gainCardTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

int gainCardTest(int supplyPos, int toFlag, int seed, int players, int verboseMode) {

	struct gameState G, Gpre;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };
	initializeGame(players, k, seed, &G); // will draw 5 cards

	int wTurn = whoseTurn(&G);

	// take snapshot of game pre-gain (Gpre)
	memcpy(&Gpre, &G, sizeof(struct gameState));

	// >>>> gain card <<<<
	int n = gainCard(supplyPos, &G, toFlag, 0);

	if (supplyCount(supplyPos, &G) < 1) {

		testAssert((memcmp(&Gpre, &G, sizeof(struct gameState)) == 0), "card not available but changes made to game state", 0, verboseMode);
	}
	else if (toFlag == 1) {

		testAssert((G.deckCount[0] == Gpre.deckCount[0] + 1), "incorrect deck count", 0, verboseMode);
		testAssert((G.supplyCount[supplyPos] == Gpre.supplyCount[supplyPos] - 1), "incorrect supply count", 0, verboseMode);
	} 
	else if (toFlag == 2) {

		testAssert((G.handCount[0] == Gpre.handCount[0] + 1), "incorrect hand count", 0, verboseMode);
		testAssert((G.supplyCount[supplyPos] == Gpre.supplyCount[supplyPos] - 1), "incorrect supply count", 0, verboseMode);
	}
	else {

		testAssert((G.discardCount[0] == Gpre.discardCount[0] + 1), "incorrect discard count", 0, verboseMode);
		testAssert((G.supplyCount[supplyPos] == Gpre.supplyCount[supplyPos] - 1), "incorrect supply count", 0, verboseMode);
	}

	return 0;
}

int main() {

	int supplyPos, toFlag, seed = 1, players;

	for (supplyPos = 0; supplyPos < 27; supplyPos++) {
		for (toFlag = 1; toFlag < 4; toFlag++) {
			for (players = 2; players < 5; players++) {

				gainCardTest(supplyPos, toFlag, seed, players, 0);
				seed++;
			}
		}
	}

	return 0;
}