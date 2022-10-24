#ifndef _STR_UTIL_H_
#define _STR_UTIL_H_


#include "DTCconverter.h"


bool split_values(uint32_t* restrict dst, size_t n, const char* restrict src, char delim) __attribute__((nonnull (1, 3)));

char* trim(char* restrict str, const char* restrict seps) __attribute__((nonnull (1, 2)));
char* ltrim(char* restrict str, const char* restrict seps) __attribute__((nonnull (1, 2)));
char* rtrim(char* restrict str, const char* restrict seps) __attribute__((nonnull (1, 2)));

char* tolowers(char* str, size_t len) __attribute__((nonnull (1)));


#endif  // _STR_UTIL_H_
