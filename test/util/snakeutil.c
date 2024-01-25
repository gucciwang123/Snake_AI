#include "include/util.h"
#include <game.h>
#include <pch.h>

SN_BOOL TEST_boardValueEquality(SN_GAME* a, SN_GAME* b, SN_BOOL checkFood) {
	if(a->sizeX != b->sizeX || a->sizeY != b->sizeY)
		return SN_FALSE;

	for(int i = 0; i < a->sizeX; i++) {
		for(int j = 0; j < a->sizeY; j++) {
			if(a->matrix[i][j].value != b->matrix[i][j].value &&
				!(!checkFood &&
				(a->matrix[i][j].value == SN_GAME_FOOD || b->matrix[i][j].value == SN_GAME_FOOD)
				 ))
				return SN_FALSE;
		}
	}

	return SN_TRUE;
}

SN_BOOL TEST_snakeValidity(SN_GAME* a) {
	SN_GAME_ELEMENT* currentElement = a->tail;
	SN_GAME_ELEMENT* head = &(a->matrix[a->head.x][a->head.y]);

	if(currentElement->value != SN_GAME_SNAKE)
		return SN_FALSE;

	for(int i = 1; i < a->currentLength; i++) {
		currentElement = currentElement->next;
		if(currentElement->value != SN_GAME_SNAKE)
			return SN_FALSE;
	}

	return currentElement == head;
}

#define SN_TEST_UTIL_PROPERTY_LINE_LENGTH 12
#define SN_TEST_UTIL_DATA_LINE_LENGTH l_sizeX+2
#define SN_TEST_CHAR_LOWER(c) c | 0x20

SN_ERROR TEST_parseGameMatrix(SN_STRING name, SN_GAME_ELEMENT*** a, SN_WORD* sizeX, SN_WORD* sizeY) {
	FILE* file = fopen(name, "r");
	*a = NULL;

	if(file == NULL) {
		fclose(file);
		return TEST_FILE_NOT_FOUND;
	}

	char line[SN_TEST_UTIL_PROPERTY_LINE_LENGTH];
	fgets(line, SN_TEST_UTIL_PROPERTY_LINE_LENGTH, file);

	if(strlen(line) == SN_TEST_UTIL_PROPERTY_LINE_LENGTH - 1 && line[SN_TEST_UTIL_PROPERTY_LINE_LENGTH - 2] != '\n') {
		fclose(file);
		return TEST_FILE_FORMAT_ERROR;
	}

	SN_WORD l_sizeX;
	SN_WORD l_sizeY;

	if(sscanf(line, "%hu,%hu", &l_sizeX, &l_sizeY) != 2) {
		fclose(file);
		return TEST_FILE_FORMAT_ERROR;
	}

	SN_GAME_ELEMENT** list = (SN_GAME_ELEMENT**) malloc(sizeof(SN_GAME_ELEMENT*) * l_sizeX);
	SN_GAME_ELEMENT* matrix = (SN_GAME_ELEMENT*) malloc(sizeof(SN_GAME_ELEMENT) * l_sizeY * l_sizeX);

	for(int i = 0; i < l_sizeX; i++)
		list[i] = &(matrix[l_sizeX * i]);

	for(int j = 0; j < l_sizeY; j++) {
		char line[SN_TEST_UTIL_DATA_LINE_LENGTH];

		if(feof(file)) {
			fclose(file);
			free(list);
			free(matrix);
			return TEST_FILE_FORMAT_ERROR;
		}
		fgets(line, SN_TEST_UTIL_DATA_LINE_LENGTH, file);

		if(line[SN_TEST_UTIL_DATA_LINE_LENGTH - 2] != '\n') {
			fclose(file);
			free(list);
			free(matrix);
			return TEST_FILE_FORMAT_ERROR;
		}

		for(int i = 0; i < l_sizeX; i++) {
			switch(SN_TEST_CHAR_LOWER(line[i])) {
				case 'w':
					list[i][j].value = SN_GAME_WALL;
					break;
				case 's':
					list[i][j].value = SN_GAME_SNAKE;
					break;
				case 'f':
					list[i][j].value = SN_GAME_FOOD;
					break;
				case 'o':
					list[i][j].value = SN_GAME_SPACE;
					break;
				default:
					fclose(file);
					free(list);
					free(matrix);
					return TEST_FILE_FORMAT_ERROR;

			}
		}

	}

	*sizeX = l_sizeX;
	*sizeY = l_sizeY;
	*a = list;

	return SN_SUCCESS;
}


SN_BOOL TEST_validFood(SN_GAME* game) {
	SN_BOOL returnValue = SN_FALSE;

	for(int i = 0; i < game->sizeX; i++) {
		for(int j = 0; j < game->sizeY; j++) {
			if(game->matrix[i][j].value == SN_GAME_FOOD) {
				if(returnValue)
					return SN_FALSE;
				returnValue = SN_TRUE;
			}
		}
	}

	return returnValue;
}

SN_ERROR TEST_printBoard(FILE* stream, SN_GAME* a, const char* restrict format, ...) {
	va_list argv;
	va_start(argv, format);
	char tag[100];
	vsprintf(tag, format, argv);
	va_end(argv);

	if(fprintf(stream, "%s\n", tag) < 0)
		return TEST_STREAM_DOESNT_EXIST;
	if(fprintf(stream, "%hu,%hu\n", a->sizeX, a->sizeY) < 0)
		return TEST_STREAM_DOESNT_EXIST;

	for(int j = 0; j < a->sizeY; j++) {
		for(int i = 0; i < a->sizeX; i++) {
			if(a->matrix[i][j].value == SN_GAME_SPACE){
				if(fprintf(stream, ANSI_COLOR_BLUE "o" ANSI_COLOR_RESET) < 0)
					return TEST_STREAM_DOESNT_EXIST;
			}
			else if(a->matrix[i][j].value == SN_GAME_WALL) {
				if(fprintf(stream, ANSI_COLOR_YELLOW "#" ANSI_COLOR_RESET) < 0)
					return TEST_STREAM_DOESNT_EXIST;
			}
			else if(a->matrix[i][j].value == SN_GAME_SNAKE) {
				if(fprintf(stream, ANSI_COLOR_GREEN "*" ANSI_COLOR_RESET) < 0)
					return TEST_STREAM_DOESNT_EXIST;
			}
			else if(a->matrix[i][j].value == SN_GAME_FOOD) {
				if(fprintf(stream, ANSI_COLOR_RED "&" ANSI_COLOR_RESET) < 0)
					return TEST_STREAM_DOESNT_EXIST;
			}
			else {
				if(fprintf(stream, ANSI_COLOR_RED "E" ANSI_COLOR_RESET) < 0)
					return TEST_STREAM_DOESNT_EXIST;
			}

		}
		if(fprintf(stream, "\n") < 0)
			return TEST_STREAM_DOESNT_EXIST;
	}

	return SN_SUCCESS;
}

SN_ERROR TEST_placeFood(SN_GAME* game, SN_POSITION position) {
	if(game->matrix[position.x][position.y].value == SN_GAME_SPACE) {
		game->matrix[position.x][position.y].value = SN_GAME_FOOD;

		return SN_SUCCESS;
	}

	return TEST_FOOD_CANNOT_BE_PLACED;
}

SN_ERROR TEST_removeFood(SN_GAME* game) {
	for(SN_DWORD i = 0; i < game->sizeX; i++) {
		for(SN_DWORD j = 0; j < game->sizeY; j++) {
			if(game->matrix[i][j].value == SN_GAME_FOOD)
				game->matrix[i][j].value = SN_GAME_SPACE;
		}
	}

	return SN_SUCCESS;
}
