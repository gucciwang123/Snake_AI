#ifndef PCH_H
#define PCH_H

#include <error.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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

#endif
