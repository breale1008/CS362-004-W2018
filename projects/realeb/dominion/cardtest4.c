#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in villageCardTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

void villageCardTest(int handPos, int seed, int players, int verboseMode) {

	int bonus;
	struct gameState G, Gpre;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };
	initializeGame(players, k, seed, &G);

	// get current player (whose turn before advancement)
	int wTurn = whoseTurn(&G);

	// take snapshot of game pre-card (Gpre)
	memcpy(&Gpre, &G, sizeof(struct gameState));

	// >>>> do the card's effects <<<<
	int n = cardEffect(village, 0, 0, 0, &G, handPos, &bonus);

	// test +1 card
	testAssert((G.handCount[wTurn] == Gpre.handCount[wTurn]), "incorrect hand count", 0, verboseMode);

	// test +2 actions
	testAssert((G.numActions == Gpre.numActions + 2), "incorrect action count", 0, verboseMode);

	// test discard, could be 1 if this card caused a shuffle or 1+(pre-discard count)
	testAssert((G.playedCardCount == Gpre.playedCardCount + 1 || G.playedCardCount == 1), "incorrect discard count", 0, verboseMode);
}


int main() {

	int handPos, seed, players;

	for (handPos = 0; handPos < 5; handPos++) {
		for (players = 2; players < 5; players++) {
			for (seed = 1; seed < 20; seed++) {

				villageCardTest(handPos, seed, players, 0);
			}
		}
	}

	return 0;
}