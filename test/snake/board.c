#include "util.h"
#include "pch.h"
#include <game.h>

int snake_board(int argc, char* argv[]) {
	srand(time(NULL));
	SN_ERROR error;
	int returnValue = 0;

	//generate general board
	SN_GAME_TEMPLATE template = {
		 40, 40,
		{29, 24},
		29,
		SN_TEMPLATE_DEFAULT_GROWSIZE
	};
	SN_GAME *game, game_file;

	error = SN_createGame(&template, &game);
	if(error != SN_SUCCESS) {
		ERROR_print(error);
		return -1;
	}

	error = TEST_parseGameMatrix("boardtest.sn_game", &(game_file.matrix), &(game_file.sizeX), &(game_file.sizeY));
	if(error != SN_SUCCESS) {
		ERROR_print(error);

		free(game);

		return -1;
	}

	//generate default board
	SN_GAME_TEMPLATE default_template = SN_DEFAULT_BOARD;
	SN_GAME *default_game, default_game_file;

	error = SN_createGame(&default_template, &default_game);
	if(error != SN_SUCCESS) {
		ERROR_print(error);
		free(game);
		return -1;
	}

	error = TEST_parseGameMatrix("default.sn_game", &(default_game_file.matrix), &(default_game_file.sizeX), &(default_game_file.sizeY));
	if(error != SN_SUCCESS) {
		ERROR_print(error);

		free(game);
		free(default_game);

		return -1;
	}

	//testing general board
	if(!TEST_boardValueEquality(game, &game_file, SN_FALSE)) {
		fprintf(stderr, ANSI_COLOR_RED "General boards do no match" ANSI_COLOR_RESET "\n");
		TEST_printBoard(stderr, game, "General Board:");
		TEST_printBoard(stderr, &game_file, "General Board File:");
		returnValue = -2;
	}
	else
		fprintf(stderr, ANSI_COLOR_GREEN "General boards match" ANSI_COLOR_RESET "\n");

	//testing default board
	if(!TEST_boardValueEquality(default_game, &default_game_file, SN_FALSE)) {
		fprintf(stderr, ANSI_COLOR_RED "Default boards do no match" ANSI_COLOR_RESET "\n");
		TEST_printBoard(stderr, default_game, "Default Board:");
		TEST_printBoard(stderr, &default_game_file, "Default Board File:");
		returnValue = -2;
	}
	else
		fprintf(stderr, ANSI_COLOR_GREEN "Default boards match" ANSI_COLOR_RESET "\n");


	free(game);
	free(default_game);

	return returnValue;
}
