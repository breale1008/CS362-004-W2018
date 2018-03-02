#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in adventurerCardTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

void adventurerCardTest(int seed, int players, int verboseMode) {

	int bonus;
	struct gameState G, Gpre;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };
	initializeGame(players, k, seed, &G);

	// get current player (whose turn before advancement)
	int wTurn = whoseTurn(&G);
	
	// take snapshot of game pre-card (Gpre)
	memcpy(&Gpre, &G, sizeof(struct gameState));
	
	// >>>> do the adventurer card's effects <<<<
	int n = cardEffect(adventurer, 0, 0, 0, &G, 0, &bonus);

	// test +2 cards
	testAssert((G.handCount[wTurn] == Gpre.handCount[wTurn] + 2), "incorrect hand count", 0, verboseMode);
	
	// test that both are treasures
	int handSize = G.handCount[wTurn];
	testAssert(((G.hand[wTurn][handSize - 2] == copper) || (G.hand[wTurn][handSize - 2] == silver) || (G.hand[wTurn][handSize - 2] == gold)), "drawn card not a treasure", 0, verboseMode);
	testAssert(((G.hand[wTurn][handSize - 1] == copper) || (G.hand[wTurn][handSize - 1] == silver) || (G.hand[wTurn][handSize - 1] == gold)), "drawn card not a treasure", 0, verboseMode);
}

int main() {

	int players, seed;

	for (players = 2; players < 5; players++) {
		for (seed = 1; seed < 200; seed++) {

			adventurerCardTest(seed, players, 0);
		}
	}

	return 0;
}