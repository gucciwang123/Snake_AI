#ifndef GAME_H
#define GAME_H

#include <pch.h>

typedef SN_QWORD SN_MOVE;

#define SN_UP 0x10000
#define SN_DOWN 0xffffffffffff0000
#define SN_RIGHT 0x1
#define SN_LEFT 0xffffffffffffffff

#define SN_GAME_WALL -1
#define SN_GAME_SNAKE -2
#define SN_GAME_FOOD 3
#define SN_GAME_SPACE 0

#define SN_GAME_COLLISION_FOOD 1
#define SN_GAME_COLLISION_ENDGAME -3

typedef union {
	struct{
		SN_WORD x;
		SN_WORD y;
	};
	SN_DWORD pos;
} SN_POSITION;

typedef struct SN_GAME_ELEMENT SN_GAME_ELEMENT;

struct SN_GAME_ELEMENT{
	SN_sBYTE value;
	SN_GAME_ELEMENT* next;
};

typedef struct {
	SN_WORD sizeX;
	SN_WORD sizeY;

	SN_DWORD length;
	SN_DWORD currentLength;

	SN_DWORD boardSize;
	SN_WORD growSize;

	SN_POSITION head;
	SN_GAME_ELEMENT* tail;

	SN_MOVE lastMove;

	SN_GAME_ELEMENT** matrix;
} SN_GAME;

#define SN_TEMPLATE_DEFAULT_SIZE_X 31
#define SN_TEMPLATE_DEFAULT_SIZE_Y 21
#define SN_TEMPLATE_DEFAULT_HEAD {1, 1}
#define SN_TEMPLATE_DEFAULT_LENGTH 1
#define SN_TEMPLATE_DEFAULT_GROWSIZE 5

//default board
//dimensions: 31x21
//head_pos: (15, 10)
//length: 1
//grow size: 5
#define SN_DEFAULT_BOARD {SN_TEMPLATE_DEFAULT_SIZE_X, SN_TEMPLATE_DEFAULT_SIZE_Y, {15, 10}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}

typedef struct{
	SN_WORD sizeX;
	SN_WORD sizeY;

	SN_POSITION head;

	SN_WORD length;

	SN_WORD growSize;
} SN_GAME_TEMPLATE;


//NOTE must set seed of randomizer in C stdlib for acual randomization
SN_ERROR SN_createGame(SN_GAME_TEMPLATE* template, SN_GAME** o_game);

SN_ERROR SN_nextMove(SN_GAME* game, SN_MOVE move);

SN_ERROR SN_validateGame(SN_GAME* game);

SN_ERROR SN_deleteGame(SN_GAME* game);

//NOTE: SN_resetGame ignores the following arguments in SN_GAME_TEMPLATE: sizeX, sizeY, growSize
SN_ERROR SN_resetGame(SN_GAME* game, SN_GAME_TEMPLATE* template);

#endif
