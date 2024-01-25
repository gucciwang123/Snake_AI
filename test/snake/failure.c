#include "util.h"
#include <pch.h>
#include <game.h>

static inline int failureTest(SN_STRING name, SN_GAME_TEMPLATE* template, SN_ERROR expected) {
	SN_GAME* game;
	SN_ERROR error = SN_createGame(template, &game);
	int returnvalue = 0;

	if(error != expected) {
		fprintf(stderr, "%s:\n\tExpected: %s\n\tGot: %s\n", name, expected(), error());
		returnvalue = -1;
	}

	if(error == SN_SUCCESS)
		free(game);

	return returnvalue;
}

int snake_failure(int argc, char* argv[]) {
	//size failure (size must be greater than 3)
	SN_GAME_TEMPLATE sizeX_tem = {3, 4, SN_TEMPLATE_DEFAULT_HEAD, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //x too small
	SN_GAME_TEMPLATE sizeY_tem = {4, 3, SN_TEMPLATE_DEFAULT_HEAD, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //y too small

	SN_GAME_TEMPLATE size_tem = {4, 4, SN_TEMPLATE_DEFAULT_HEAD, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //should succeed

	//grow size failure (grow size must be greater than 0)
	SN_GAME_TEMPLATE growSize1_tem = {4, 4, SN_TEMPLATE_DEFAULT_HEAD, SN_TEMPLATE_DEFAULT_LENGTH, 0}; //too small
	SN_GAME_TEMPLATE growSize2_tem = {4, 4, SN_TEMPLATE_DEFAULT_HEAD, SN_TEMPLATE_DEFAULT_LENGTH, 1}; //should succeed

	//head failure (head must be within grid with at least one extra space in front)
	SN_GAME_TEMPLATE headX1_tem = {20, 20, {0, 1}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //on the wall
	SN_GAME_TEMPLATE headX2_tem = {20, 20, {19, 1}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //no head space

	SN_GAME_TEMPLATE headY1_tem = {40, 40, {1, 0}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //on the wall
	SN_GAME_TEMPLATE headY2_tem = {40, 40, {1, 39}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //on the wall

	SN_GAME_TEMPLATE head1_tem = {40, 40, {1, 1}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //should succeed
	SN_GAME_TEMPLATE head2_tem = {40, 40, {37, 37}, SN_TEMPLATE_DEFAULT_LENGTH, SN_TEMPLATE_DEFAULT_GROWSIZE}; //should succeed

	//length failure (length must not exceed available tail room and must be greater than 0)
	SN_GAME_TEMPLATE length1_tem = {4, 4, SN_TEMPLATE_DEFAULT_HEAD, 0, SN_TEMPLATE_DEFAULT_GROWSIZE}; //length too small
	SN_GAME_TEMPLATE length2_tem = {6, 6, {3, 1}, 5, SN_TEMPLATE_DEFAULT_GROWSIZE}; //too long

	SN_GAME_TEMPLATE length3_tem = {6, 6, {3, 1}, 4, SN_TEMPLATE_DEFAULT_GROWSIZE}; //should succeed
	SN_GAME_TEMPLATE length4_tem = {6, 6, {3, 1}, 1, SN_TEMPLATE_DEFAULT_GROWSIZE}; //should succeed


	//running tests
	int returnValue = 0;

	//size failure
	returnValue = failureTest("sizeX", &sizeX_tem, SN_INVALID_BOARD_SIZE);
	returnValue = failureTest("sizeY", &sizeY_tem, SN_INVALID_BOARD_SIZE);

	returnValue = failureTest("size", &size_tem, SN_SUCCESS);


	//grow size failure
	returnValue = failureTest("growSize1", &growSize1_tem, SN_GROWSIZE_OUT_OF_BOUND);

	returnValue = failureTest("growSize2", &growSize2_tem, SN_SUCCESS);


	//head failure
	returnValue = failureTest("headX1", &headX1_tem, SN_POS_OUT_OF_BOUND);
	returnValue = failureTest("headX2", &headX2_tem, SN_POS_OUT_OF_BOUND);
	returnValue = failureTest("headY1", &headY1_tem, SN_POS_OUT_OF_BOUND);
	returnValue = failureTest("headY2", &headY2_tem, SN_POS_OUT_OF_BOUND);

	returnValue = failureTest("head1", &head1_tem, SN_SUCCESS);
	returnValue = failureTest("head2", &head2_tem, SN_SUCCESS);

	//length failure
	returnValue = failureTest("length1", &length1_tem, SN_LENGTH_OUT_OF_BOUND);
	returnValue = failureTest("length2", &length2_tem, SN_LENGTH_OUT_OF_BOUND);

	returnValue = failureTest("length3", &length3_tem, SN_SUCCESS);
	returnValue = failureTest("length4", &length4_tem, SN_SUCCESS);

	return returnValue;
}
