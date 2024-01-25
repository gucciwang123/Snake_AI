#ifndef UTIL_H
#define UTIL_H

#include <pch.h>
#include <game.h>

SN_BOOL TEST_boardValueEquality(SN_GAME* a, SN_GAME* b, SN_BOOL checkFood);

SN_BOOL TEST_snakeValidity(SN_GAME* a);

SN_BOOL TEST_validFood(SN_GAME* game);

//file format
//[sizeX]x[sizeY]
//Data block (no spaces, new line for new row)
//	w - wall
//	s - snake
//	f - food
//	o - space
SN_ERROR TEST_parseGameMatrix(SN_STRING name, SN_GAME_ELEMENT*** a, SN_WORD* sizeX, SN_WORD* sizeY);

SN_ERROR TEST_printBoard(FILE* stream, SN_GAME* a, const char* restrict format,...);

SN_ERROR TEST_removeFood(SN_GAME* game);

SN_ERROR TEST_placeFood(SN_GAME* game, SN_POSITION position);

#endif
