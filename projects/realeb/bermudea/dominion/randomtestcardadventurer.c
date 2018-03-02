#include "dominion.h"
#include "rngs.h"
#include<time.h>

// testing the adventurer card
void testAssert(int assertion, char* msg, int fatal, int verboseMode) {

	if (!assertion) {

		printf("ERROR in randomtestcardadventurer: %s\n", msg);

		if (fatal)
			exit(200);
	}
	else {

		if (verboseMode)
			printf("PASS: %s\n", msg);
	}
}

void randomizeGameState(struct gameState *G, int k[10]) {

	// all kingdom cards = 0 to previous value
	int n;
	for (n = 0; n < treasure_map + 1; n++) {

		if (G->supplyCount[n] > 0) {

			// supply: 0-N (N = original supply count)
			G->supplyCount[n] = rand() % (G->supplyCount[n] + 1);

			// embargoTokens = 1 or 0, since there is no additional effect from >1
			G->embargoTokens[n] = rand() % 2;
		}
	}

	G->supplyCount[curse] = rand() % (G->supplyCount[curse] + 1);
	G->supplyCount[estate] = rand() % (G->supplyCount[estate] + 1);
	G->supplyCount[duchy] = rand() % (G->supplyCount[duchy] + 1);
	G->supplyCount[province] = rand() % (G->supplyCount[province] + 1);

	G->supplyCount[copper] = rand() % (G->supplyCount[copper] + 1);
	G->supplyCount[silver] = rand() % (G->supplyCount[silver] + 1);
	G->supplyCount[gold] = rand() % (G->supplyCount[gold] + 1);

	// outpost played = yes or no
	G->outpostPlayed = rand() % 2;

	// outpost turn is not used anywhere, so we will not bother setting it

	// whoseTurn = any valid player index
	G->whoseTurn = rand() % G->numPlayers;
	//printf("G->whoseTurn %d\n", G->whoseTurn);

	// phase = 1 or 0 (the only valid states)
	G->phase = rand() % 2;
	//printf("G->phase %d\n", G->phase); 

	// numActions = random positive number (or 0) less than 100 (chosen as a point unlikely, if even possible, to be reached in play, so as to test all reasonable scenarios)
	G->numActions = rand() % 101;
	//printf("G->numActions %d\n", G->numActions);

	// coins = random positive number (or 0) less than 100 (chosen as a point unlikely, if even possible, to be reached in play, so as to test all reasonable scenarios)
	G->coins = rand() % 101;
	//printf("G->coins %d\n", G->coins);

	// numBuys = random positive number (or 0) less than 100 (chosen as a point unlikely, if even possible, to be reached in play, so as to test all reasonable scenarios)
	G->numBuys = rand() % 101;
	//printf("G->numBuys %d\n", G->numBuys);

	// randomize hand/deck
	if (G->whoseTurn != 0) {

		// undo drawing of player 0's cards
		for (n = 0; n < 5; n++)
			discardCard(0, 0, G, 1);
	}

	// randomize deck
	int numCards = rand() % 101;
	int cardsInHand = rand() % (numCards + 1);
	
	//printf("%d cards in deck\n", numCards);
	for (n = 0; n < numCards; n++) {

		G->deck[G->whoseTurn][n] = rand() % (treasure_map + 1);
		G->deckCount[G->whoseTurn]++;
	}

	//printf("%d cards in hand\n", cardsInHand);
	for (n = 0; n < cardsInHand; n++) {

		drawCard(G->whoseTurn, G);
		//printf("card %d: %d\n", n, G->hand[G->whoseTurn][n]);
	}	
}

int numTreasures(struct gameState *G) {

	int n, treasures = 0;
	for (n = 0; n < numHandCards(G); n++) {

		if (G->hand[G->whoseTurn][n] == copper || G->hand[G->whoseTurn][n] == silver || G->hand[G->whoseTurn][n] == gold)
			treasures++;
	}

	return treasures;
}

void randomtestcardadventurer(struct gameState *G, int verboseMode) {

	int bonus;
	struct gameState Gpre;

	if (verboseMode)
		printf("test start: randomtestcardadventurer\n");

	// take snapshot of game pre-card (Gpre)
	memcpy(&Gpre, G, sizeof(struct gameState));

	// get current player (whose turn before advancement)
	int wTurn = whoseTurn(G);

	int adventPos;
	if (numHandCards(G) > 0) {

		// force hand to contain an adventurer
		adventPos = rand() % numHandCards(G);
		G->hand[G->whoseTurn][adventPos] = adventurer;

		// >>>> do the adventurer card's effects <<<<
		int n = cardEffect(adventurer, 0, 0, 0, G, adventPos, &bonus);



		// test +1 cards
		testAssert((numHandCards(G) <= Gpre.handCount[wTurn] + 2), "incorrect hand count", 0, verboseMode);
		//printf("%d vs %d\n", numHandCards(G), Gpre.handCount[wTurn]);

		// test that we have 2 more treasures in hand
		testAssert((numTreasures(G) <= numTreasures(&Gpre) + 2), "incorrect treasure cards drawn", 0, verboseMode);
		
		for (n = 0; n < numHandCards(G); n++) {

			//printf("card %d: %d\n", n, G->hand[wTurn][n]);
		}
	}
	else {

		printf("no cards in current player's hand\n");
	}
}

int main() {

	int seed, players, k[10], n;
	srand(time(NULL));

	for (n = 0; n < 10000; n++) {

		// randomize seed passed to initializeGame
		seed = rand();

		// randomize kingdom cards
		int m;

		for (m = 0; m < 10; m++)
			k[m] = -1;

		for (m = 0; m < 10; m++) {

			while (k[m] == -1) {

				int randNum = (rand() % (treasure_map - adventurer + 1)) + 7;

				// make sure it's not already chosen
				int p, chosen = 0;
				for (p = 0; p < m; p++) {

					if (randNum == k[p])
						chosen = 1;
				}

				if (!chosen)
					k[m] = randNum;
			}
		}

		// randomize game state variables
		struct gameState G;

		//printf("\n\nNew gameState\n");

		// # players: 2-4
		players = rand() % (MAX_PLAYERS - 1) + 2;

		// set up game state
		if (initializeGame(players, k, seed, &G) == -1)
			printf("ERROR in initializeGame\n");

		// randomize most of the variables in gameState G
		randomizeGameState(&G, k);

		// do a few iterations of the card to simulate differing game conditions (discard, played card pile, etc)
		randomtestcardadventurer(&G, 0);
		//randomtestcardadventurer(&G, 1);
		//randomtestcardadventurer(&G, 1);
	}

	return 0;
}
