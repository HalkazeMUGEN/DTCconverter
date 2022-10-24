#ifndef _FORMAT_H_
#define _FORMAT_H_


#include "DTCconverter.h"


typedef enum _FORMAT_TYPES {
  FT_PERCENT = '%',
  FT_C_SMALL = 'c',
  FT_C_LARGE = 'C',
  FT_D = 'd',
  FT_I = 'i',
  FT_O = 'o',
  FT_U = 'u',
  FT_X_SMALL = 'x',
  FT_X_LARGE = 'X',
  FT_E_SMALL = 'e',
  FT_E_LARGE = 'E',
  FT_F_SMALL = 'f',
  FT_F_LARGE = 'F',
  FT_G_SMALL = 'g',
  FT_G_LARGE = 'G',
  FT_A_SMALL = 'a',
  FT_A_LARGE = 'A',
  FT_N = 'n',
  FT_P = 'p',
  FT_S_SMALL = 's',
  FT_S_LARGE = 'S',
  FT_Z = 'Z',
} FORMAT_TYPES;


char* get_head_of_format(FORMAT_TYPES target, const char* restrict text, size_t* restrict plen) __attribute__((nonnull (2)));

int get_number_of_formats(const char* format) __attribute__((nonnull (1)));

size_t get_length_of_formatted_str(const char* restrict format, const uint32_t* restrict args, size_t argc) __attribute__((nonnull (1, 2)));


#endif  // _FORMAT_H_
