#ifndef _SAFE_MEM_H_
#define _SAFE_MEM_H_


#include "DTCconverter.h"


void* safe_malloc(size_t x) __attribute__((malloc, malloc (free, 1)));
void* safe_calloc(size_t n, size_t size) __attribute__((malloc, malloc (free, 1)));
void* safe_realloc(void* p, size_t x) __attribute__((malloc, malloc (free, 1)));


#endif  // _SAFE_MEM_H_
