#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>
#include <stdio.h>
typedef const char* (*SN_ERROR)();

#define SN_SUCCESS &ERROR_sucess

//game.h errors
#define SN_POS_OUT_OF_BOUND &ERROR_pos_out_of_bound
#define SN_LENGTH_OUT_OF_BOUND &ERROR_length_out_of_bound
#define SN_INVALID_BOARD_SIZE &ERROR_invalid_board_size
#define SN_END_GAME &ERROR_end_game
#define SN_GROWSIZE_OUT_OF_BOUND &ERROR_growsize_out_of_bound

//util.h errors
#define TEST_FILE_NOT_FOUND &ERROR_file_not_found
#define TEST_FILE_FORMAT_ERROR &ERROR_file_format_error
#define TEST_STREAM_DOESNT_EXIST &ERROR_stream_doesnt_exist

//error functions
const char* ERROR_sucess();

const char* ERROR_pos_out_of_bound();
const char* ERROR_length_out_of_bound();
const char* ERROR_invalid_board_size();
const char* ERROR_end_game();
const char* ERROR_growsize_out_of_bound();

const char* ERROR_file_not_found();
const char* ERROR_file_format_error();
const char* ERROR_stream_doesnt_exist();


//error print
int ERROR_tag(SN_ERROR error, const char* restrict tag);
int ERROR_ftag(FILE* file, SN_ERROR error, const char* restrict tag);

int ERROR_print(SN_ERROR error);
int ERROR_fprint(FILE* file, SN_ERROR error);
#endif

