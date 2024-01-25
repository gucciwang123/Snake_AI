#include "util.h"
#include "pch.h"
#include <game.h>

//TODO move remove food to game.c
static inline SN_ERROR createGame(SN_GAME_TEMPLATE* template, SN_GAME** o_game) {
	SN_ERROR error = SN_createGame(template, o_game);

	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error creating game, got");
		return error;
	}

	TEST_removeFood(*o_game);

	return SN_SUCCESS;
}

static inline int basicTest(SN_STRING name, SN_GAME* game, SN_MOVE move, SN_STRING file) {
	TEST_printBoard(stderr, game, "%s: Start", name);

	//running operation on test board
	SN_ERROR error = SN_nextMove(game, move);

	if(error != SN_SUCCESS) {
		ERROR_tag(error, name);
		return -1;
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

static inline int listTest(SN_STRING name, SN_GAME* game, SN_MOVE* move, size_t length, SN_STRING file) {
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
			ERROR_tag(error, "%s at %ldth iteration", name, i);
			return -1;
		}
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

int snake_movement(int argc, char* argv[]) {
	SN_ERROR error;
	int returnValue = 0;

	//checking movement operations
	SN_GAME_TEMPLATE basic_template = SN_DEFAULT_BOARD;
	SN_GAME *basic_up, *basic_left, *basic_right,*basic_down ;

	error = createGame(&basic_template, &basic_up);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for basic UP test, got");

		return -1;
	}

	error = createGame(&basic_template, &basic_left);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for basic LEFT test, got");

		free(basic_up);

		return -1;
	}

	error = createGame(&basic_template, &basic_right);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for basic RIGHT test, got");

		free(basic_up);
		free(basic_left);

		return -1;
	}

	error = createGame(&basic_template, &basic_down);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for basic DOWN test, got");

		free(basic_up);
		free(basic_right);
		free(basic_left);

		return -1;
	}

	returnValue += basicTest("UP test", basic_up, SN_UP, "basic_up.sn_game");
	returnValue += basicTest("DOWN test", basic_down, SN_DOWN, "basic_down.sn_game");
	returnValue += basicTest("LEFT test", basic_left, SN_LEFT, "basic_left.sn_game");
	returnValue += basicTest("RIGHT test", basic_right, SN_RIGHT, "basic_right.sn_game");

	free(basic_up);
	free(basic_left);
	free(basic_right);
	free(basic_down);

	//checking movement operations using a string of commands on a snake of length 5
	SN_MOVE list1_1[] = {
		SN_RIGHT,
		SN_RIGHT,
		SN_UP,
		SN_DOWN, //equivilent to SN_UP
		SN_LEFT,
		SN_RIGHT, //equivilent to SN_LEFT
		SN_LEFT,
		SN_LEFT
	};

	SN_MOVE list1_2[] = {
		SN_LEFT,
		SN_LEFT,
		SN_DOWN,
		SN_UP, //equivilent to SN_DOWN
		SN_RIGHT,
		SN_LEFT, //equivilent to SN_RIGHT
		SN_RIGHT,
		SN_RIGHT
	};

	SN_GAME_TEMPLATE list1_template = SN_DEFAULT_BOARD;
	list1_template.length = 5;

	SN_GAME *list1_game;

	error = createGame(&list1_template, &list1_game);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for list1 , got");
		return -1;
	}

	returnValue += listTest("List1_1 test", list1_game, list1_1, sizeof(list1_1)/sizeof(SN_MOVE), "list1_1.sn_game");
	returnValue += listTest("List1_2 test", list1_game, list1_2, sizeof(list1_2)/sizeof(SN_MOVE), "list1_2.sn_game");

	free(list1_game);

	//checking movement operations using a string of commands on a snake of length 1
	SN_MOVE list2[] = {
		SN_RIGHT,
		SN_LEFT,
		SN_RIGHT,
		SN_DOWN,
		SN_UP,
		SN_DOWN,
		SN_LEFT,
		SN_RIGHT,
	};

	SN_GAME_TEMPLATE list2_template = SN_DEFAULT_BOARD;

	SN_GAME *list2_game;

	error = createGame(&list2_template, &list2_game);
	if(error != SN_SUCCESS) {
		ERROR_tag(error, "Error constructing board for list2 , got");
		return -1;
	}

	returnValue += listTest("List2 test", list2_game, list2, sizeof(list2)/sizeof(SN_MOVE), "list2.sn_game");

	free(list2_game);

	return returnValue;
}

