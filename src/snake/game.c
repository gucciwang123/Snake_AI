#include <pch.h>
#include "game.h"

#define SN_FOOD_SEQUENTIAL_CUTOFF 0.999f

static inline SN_ERROR addFood(SN_GAME* game) {
	if(game->boardSize <= game->currentLength)
		return SN_END_GAME;

	SN_WORD random = rand() % (game->boardSize - game->length);

	SN_WORD count = 0;

	for(SN_WORD i = 1; i < game->sizeX-1; i++) {
		for(SN_WORD j = 1; j < game->sizeY-1; j++) {
			if(game->matrix[i][j].value == SN_GAME_SPACE && random < count++) {
				game->matrix[i][j].value = SN_GAME_FOOD;
				return SN_SUCCESS;
			}
		}
	}

	return SN_LENGTH_OUT_OF_BOUND;
}
//TODO add random selection

static inline SN_MOVE reverseMove(SN_MOVE move) {
	return ~move + 1;
}

//TODO change parsing of default board
SN_ERROR SN_createGame(SN_GAME_TEMPLATE* template, SN_GAME** o_game) {
	*o_game = NULL;

	//initialize properties
	SN_GAME* game = (SN_GAME*) malloc(sizeof(SN_GAME));

	game->length = template->length;

	game->sizeX = template->sizeX;
	game->sizeY = template->sizeY;

	game->head.x = template->head.x;
	game->head.y = template->head.y;

	game->growSize = template->growSize;

	//validate properties
	if(game->sizeX < 4 || game->sizeY < 4) {
		free(game);
		return SN_INVALID_BOARD_SIZE;
	}

	if(game->head.x >= game->sizeX - 2 ||
		game->head.y >= game->sizeY - 2 ||
		game->head.x == 0 ||
		game->head.y == 0
		) {
		free(game);
		return SN_POS_OUT_OF_BOUND;
	}

	if(game->length > game->head.x) {
		free(game);
		return SN_LENGTH_OUT_OF_BOUND;
	}

	if(game->growSize == 0) {
		free(game);
		return SN_GROWSIZE_OUT_OF_BOUND;
	}

	if(game->length == 0) {
		free(game);
		return SN_LENGTH_OUT_OF_BOUND;
	}

	//initialize misc properties
	game->boardSize = (game->sizeX-2)*(game->sizeY-2);
	game->currentLength = game->length;
	game->lastMove = 0;

	//initialize matrix
	game->matrix = (SN_GAME_ELEMENT**) malloc(game->sizeX*sizeof(SN_GAME_ELEMENT*));

	SN_GAME_ELEMENT* matrix = (SN_GAME_ELEMENT*) malloc(game->sizeX*game->sizeY*sizeof(SN_GAME_ELEMENT));

	for(SN_WORD i = 0; i < game->sizeX; i++)
		game->matrix[i] = &(matrix[i* game->sizeY]);

	//initialize walls
	for(SN_WORD i = 0; i < game->sizeX; i++) {
		game->matrix[i][0].value = SN_GAME_WALL;
		game->matrix[i][game->sizeY-1].value = SN_GAME_WALL;
	}
	for(SN_WORD i = 0; i < game->sizeY; i++) {
		game->matrix[0][i].value = SN_GAME_WALL;
		game->matrix[game->sizeX-1][i].value = SN_GAME_WALL;
	}

	//initialize space
	for(SN_WORD i = 1; i < game->sizeX - 1; i++)
		for(SN_WORD j = 1; j < game->sizeY - 1; j++)
			game->matrix[i][j].value = SN_GAME_SPACE;

	//initialize snake
	game->tail = &(game->matrix[game->head.x][game->head.y]);
	game->matrix[game->head.x][game->head.y].value = SN_GAME_SNAKE;
	for(SN_WORD i = 1;  i < game->length; i++) {
		game->matrix[game->head.x- i][game->head.y].value = SN_GAME_SNAKE;
		game->matrix[game->head.x - i][game->head.y].next = game->tail;

		game->tail = &(game->matrix[game->head.x - i][game->head.y]);
	}

	*o_game = game;
	return addFood(*o_game);
}

SN_ERROR SN_nextMove(SN_GAME* game, SN_MOVE move) {
	SN_POSITION newHead;
	newHead.pos = game->head.pos + move + (move == reverseMove(game->lastMove)) * 2 * game->lastMove;

	game->matrix[game->head.x][game->head.y].next = &(game->matrix[newHead.x][newHead.y]);
	game->head = newHead;

	game->tail = (game->currentLength == game->length) ? game->tail->next : game->tail;
	game->currentLength += (game->currentLength != game->length);

	game->matrix[newHead.x][newHead.y].value += SN_GAME_SNAKE;

	return SN_SUCCESS;
}

SN_ERROR SN_validateGame(SN_GAME* game) {
	SN_WORD i, j;
	SN_BOOL stop = SN_FALSE;
	for(i = 0; i < game->sizeX; i++) {
		for(j = 0; j < game->sizeY; j++) {
			if (game->matrix[i][j].value == SN_GAME_COLLISION_FOOD) {
				game->length += game->growSize;
				addFood(game);
				return SN_SUCCESS;
			}
			if(game->matrix[i][j].value <= SN_GAME_COLLISION_ENDGAME)
				return SN_END_GAME;
		}
	}

	return SN_SUCCESS;
}

SN_ERROR SN_deleteGame(SN_GAME* game) {
	free(game->matrix[0]);
	free(game->matrix);
	free(game);

	return SN_SUCCESS;
}

SN_ERROR SN_resetGame(SN_GAME* game, SN_GAME_TEMPLATE* template) {
	//initialize walls
	game->matrix[game->head.x][game->head.y].value = SN_GAME_WALL;

	//initialize space
	for(SN_WORD i = 1; i < game->currentLength - 1; i++) {
		game->tail = SN_GAME_SPACE;
		game->tail = game->tail->next;
	}

	//initialize misc properties
	game->length = template->length;
	game->currentLength = template->length;
	game->head.x = template->head.x;
	game->head.y = template->head.y;

	//validate properties
	if(game->head.x >= game->sizeX - 2 ||
		game->head.y >= game->sizeY - 2 ||
		game->head.x == 0 ||
		game->head.y == 0
		) {
		free(game);
		return SN_POS_OUT_OF_BOUND;
	}

	if(game->length > game->head.x) {
		free(game);
		return SN_LENGTH_OUT_OF_BOUND;
	}
	if(game->length == 0) {
		free(game);
		return SN_LENGTH_OUT_OF_BOUND;
	}

	//initialize snake
	game->tail = &(game->matrix[game->head.x][game->head.y]);
	for(SN_WORD i = 1;  i < game->length; i++) {
		game->matrix[game->head.y - i][game->head.y].value = SN_GAME_SNAKE;
		game->matrix[game->head.y - i][game->head.y].next = game->tail;

		game->tail = &(game->matrix[game->head.y - i][game->head.y]);
	}

	return addFood(game);
}
