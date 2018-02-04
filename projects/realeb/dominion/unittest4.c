#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in updateCoinsTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

int updateCoinsTest(int seed, int players, int bonus, int verboseMode) {

	struct gameState G, Gpre;
	int k[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy };
	initializeGame(players, k, seed, &G); // will draw 5 cards

	int wTurn = whoseTurn(&G);
	int value = bonus;

	// >>>> update coins <<<<
	int n = updateCoins(wTurn, &G, bonus);

	for (n = 0; n < G.handCount[wTurn]; n++) {

		if (G.hand[wTurn][n] == copper) {

			value++;
		}
		else if (G.hand[wTurn][n] == silver) {

			value += 2;
		}
		else if (G.hand[wTurn][n] == gold) {

			value += 3;
		}
	}

	testAssert(value == G.coins, "incorrect coins value", 0, verboseMode);
}

int main() {

	int seed = 1, players, bonus;

	for (players = 2; players < 5; players++) {
		for (bonus = 0; bonus < 5; bonus++) {
			for (seed = 1; seed < 30; seed++) {
				updateCoinsTest(seed, players, bonus, 0);
			}
		}
	}

	return 0;
}