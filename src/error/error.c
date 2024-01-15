#include <stdio.h>
#include "error.h"
const char* ERROR_sucess() {
	static const char* pos_out_of_bound_error = "SN_SUCCESS, No error.";
	return pos_out_of_bound_error;
}
//game.h errors
const char* ERROR_pos_out_of_bound() {
	static const char* pos_out_of_bound_error = "SN_POS_OUT_OF_BOUND, value of position out of bounds of board.";
	return pos_out_of_bound_error;
}

const char* ERROR_length_out_of_bound() {
	static const char* length_out_of_bound_error = "SN_LENGHT_OUT_OF_BOUND, length of snake out of bounds.";
	return length_out_of_bound_error;
}

const char* ERROR_invalid_board_size() {
	static const char* invalid_board_size = "SN_INVALID_BOARD_SIZE, board size not within specification.";
	return invalid_board_size;
}

const char* ERROR_end_game() {
	static const char* end_game = "SN_END_GAME, game has ended.";
	return end_game;
}

const char* ERROR_growsize_out_of_bound() {
	static const char* growsize_out_of_bound = "SN_GROWSIZE_OUT_OF_BOUND, invalid grow size.";
	return growsize_out_of_bound;
}

//util.h errors
const char* ERROR_file_not_found() {
	static const char* file_not_found = "TEST_FILE_NOT_FOUND, file was not found.";
	return file_not_found;
}

const char* ERROR_file_format_error() {
	static const char* file_format_error = "TEST_FILE_FORMAT_ERROR, file was not correctly formatted.";
	return file_format_error;
}

const char* ERROR_stream_doesnt_exist() {
	static const char* stream_doesnt_exist = "TEST_STREAM_DOESNT_EXIST, invalid output stream.";
	return stream_doesnt_exist;
}

//error output
int ERROR_tag(SN_ERROR error, const char* restrict tag) {
	return fprintf(stderr, "%s: %s", tag, error());
}

int ERROR_ftag(FILE* file, SN_ERROR error, const char* restrict tag) {
	return fprintf(file, "%s: %s", tag, error());
}

int ERROR_print(SN_ERROR error) {
	return fprintf(stderr, "%s" , error());
}

int ERROR_fprint(FILE* file, SN_ERROR error) {
	return fprintf(file, "%s", error());
}
