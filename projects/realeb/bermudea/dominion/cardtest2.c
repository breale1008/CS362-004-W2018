#include "dominion.h"

void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in councilRoomCardTest: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

void councilRoomCardTest(int handPos, int seed, int players, int verboseMode) {

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
	int n = cardEffect(council_room, 0, 0, 0, &G, handPos, &bonus);

	// check current player hand count
	testAssert((G.handCount[wTurn] == Gpre.handCount[wTurn] + 3), "incorrect hand count", 0, verboseMode);

	// check other players hand counts
	for (n = 0; n < G.numPlayers; n++) {

		if (n != wTurn) {

			testAssert((G.handCount[n] == Gpre.handCount[n] + 1), "incorrect hand count, secondary effect", 0, verboseMode);
		}
	}

	// test +1 buys
	testAssert((G.numBuys == Gpre.numBuys + 1), "incorrect buy count", 0, verboseMode);

	// test discard, could be 1 if this card caused a shuffle or 1+(pre-discard count)
	testAssert(( G.playedCardCount == Gpre.playedCardCount + 1 || G.playedCardCount == 1), "incorrect discard count", 0, verboseMode);
}

int main() {

	int players, seed, handPos;

	for (players = 2; players < 5; players++) {
		for (seed = 1; seed < 40; seed++) {
			for (handPos = 0; handPos < 5; handPos++) {

				councilRoomCardTest(handPos, seed, players, 0);
			}
		}
	}

	return 0;
}