#include "util.h"
#include <game.h>
#include <pch.h>

static inline int test(SN_STRING name, SN_GAME* game, SN_MOVE* move, size_t length, SN_STRING file) {
	SN_ERROR error;

	TEST_printBoard(stderr, game, "%s: Start", name);

	//running test
	for(size_t i = 0; i < length; i++) {
		error = SN_nextMove(game, move[i]);
		TEST_printBoard(stderr, game,
				(move[i] == SN_UP) ? "%s: SN_UP" :
				(move[i] == SN_DOWN) ? "%s: SN_DOWN" :
				(move[i] == SN_RIGHT) ? "%s: SN_RIGHT" :
				(move[i] == SN_LEFT) ? "%s: SN_LEFT" :
				"%s: UNKNOWN",
				name);
		if(error != SN_SUCCESS) {
			ERROR_tag(error, "%s at %ldth iteration on move", name, i);
			return -1;
		}

		error = SN_validateGame(game);
		if(error != SN_SUCCESS) {
			ERROR_tag(error, "%s at %ldth iteration at board validation", name, i);
			return -1;
		}

		TEST_removeFood(game);

	}

	//generating reference board
	SN_GAME expectedGame;
	error = TEST_parseGameMatrix(file, &(expectedGame.matrix), &(expectedGame.sizeX), &(expectedGame.sizeY));
	if(error != SN_SUCCESS) {
		ERROR_tag(error, name);
		return -1;
	}

	//checking result
	if(!TEST_boardValueEquality(game, &expectedGame, SN_TRUE)) {
		fprintf(stderr, ANSI_COLOR_RED "%s: Boards Do not match" ANSI_COLOR_RESET "\n", name);
		TEST_printBoard(stderr, &expectedGame, "Reference Board");
		TEST_printBoard(stderr, game, "Generated Board");

		return -1;
	}

	if(!TEST_snakeValidity(game)) {
		fprintf(stderr, ANSI_COLOR_RED "%s: Snake not valid" ANSI_COLOR_RESET "\n", name);
		TEST_printBoard(stderr, &expectedGame, "Reference Board");
		TEST_printBoard(stderr, game, "Generated Board");

		return -1;
	}

	fprintf(stderr, ANSI_COLOR_GREEN "%s: Boards matched and validated" ANSI_COLOR_RESET "\n", name);

	return 0;
}

static inline int testIteration(SN_STRING name, SN_GAME* game, SN_MOVE move) {
	SN_ERROR error;

	TEST_printBoard(stderr, game, "%s: Start", name);

	//running test
	error = SN_nextMove(game, move);
	TEST_printBoard(stderr, game,
			(move == SN_UP) ? "%s: SN_UP" :
			(move == SN_DOWN) ? "%s: SN_DOWN" :
			(move == SN_RIGHT) ? "%s: SN_RIGHT" :
			(move == SN_LEFT) ? "%s: SN_LEFT" :
			"%s: UNKNOWN",
			name);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "%s on move", name);
		return -1;
	}

	error = SN_validateGame(game);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "%s at board validation", name);
		return -1;
	}

	return 0;
}

int snake_food(int argc, char* argv[]) {
	SN_GAME_TEMPLATE template = SN_DEFAULT_BOARD;
	template.length = 5;
	template.growSize = 3;

	SN_GAME *game1;
	SN_ERROR error;
	int returnValue = 0;

	//single food event
	error = SN_createGame(&template, &game1);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for test 1, got");
		return -1;
	}

	if(!TEST_validFood(game1)) { //checks that a food is placed on init
		fprintf(stderr, ANSI_COLOR_RED "ERROR: food not propertly placed on init" ANSI_COLOR_RESET);
		returnValue--;
	}

	SN_POSITION pos = {16, 10};

	TEST_removeFood(game1);
	error = TEST_placeFood(game1, pos);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error placing food for test 1, got");
		return -1;
	}

	SN_MOVE moves1[] = {
		SN_RIGHT,
		SN_UP,
		SN_LEFT,
		SN_LEFT,
		SN_LEFT,
		SN_LEFT,
		SN_DOWN,
		SN_DOWN,
		SN_RIGHT

	};

	returnValue += test("TEST1", game1, moves1, sizeof(moves1)/sizeof(SN_MOVE), "food1.sn_game");

	free(game1);

	//double food event
	SN_GAME *game2;
	error = SN_createGame(&template, &game2);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for test 2, got");
		return -1;
	}

	SN_POSITION pos1 = {16, 10};
	SN_POSITION pos2 = {17, 10};

	TEST_removeFood(game2);
	error = TEST_placeFood(game2, pos1);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error placing first food for test 2, got");
		return -1;
	}

	SN_MOVE moves2[] = {
		SN_RIGHT,
		SN_UP,
		SN_UP,
		SN_LEFT,
		SN_LEFT,
		SN_LEFT,
		SN_DOWN
	};

	returnValue += testIteration("TEST2 Initial Move", game2, SN_RIGHT);

	if(!TEST_validFood(game2)) { //checks that a new food is placed
		fprintf(stderr, ANSI_COLOR_RED "ERROR: food not propertly replaced" ANSI_COLOR_RESET);
		returnValue--;
	}

	TEST_removeFood(game2);
	error = TEST_placeFood(game2, pos2);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error placing second food for test 2, got");
		return -1;
	}

	returnValue += test("TEST2", game2, moves2, sizeof(moves2)/sizeof(SN_MOVE), "food2.sn_game");

	free(game2);

	return returnValue;
}

//TODO test food placement never occures on snake
