#ifndef _DECODE_H_
#define _DECODE_H_


#include "DTCconverter.h"


void decode(FILE* fp) __attribute__((nonnull (1)));
uint32_t read_addr_label(FILE* fp) __attribute__((nonnull (1)));
bool is_addr_label(const char* str);

#endif  // _DECODE_H_
