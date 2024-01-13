#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef uint8_t SN_BYTE;
typedef uint16_t SN_WORD;
typedef uint32_t SN_DWORD;
typedef uint64_t SN_QWORD;

typedef int8_t SN_sBYTE;
typedef int16_t SN_sWORD;
typedef int32_t SN_sDWORD;
typedef int64_t SN_sQWORD;

typedef double SN_QWORD_F;
typedef float SN_DWORD_F;

typedef char* SN_STRING;

typedef SN_BYTE SN_BOOL;
#define SN_FALSE 0
#define SN_TRUE 1

typedef int16_t SN_ERROR;

#define SN_SUCCESS 0
