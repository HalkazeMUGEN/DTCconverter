#ifndef _NULLABLE_H_
#define _NULLABLE_H_


#include "DTCconverter.h"


#define SEQUENCE_DELIMS " "

#define NULLABLE_VALUE_TYPE_NOT_EXIST ((NULLABLE_VALUE_TYPE)0)
#define NULLABLE_VALUE_TYPE_ZERO      ((NULLABLE_VALUE_TYPE)1)
#define NULLABLE_VALUE_TYPE_NOT_ZERO  ((NULLABLE_VALUE_TYPE)2)

#define NVT_XX                        NULLABLE_VALUE_TYPE_NOT_EXIST
#define NVT_00                        NULLABLE_VALUE_TYPE_ZERO
#define NVT_AA                        NULLABLE_VALUE_TYPE_NOT_ZERO


typedef unsigned int NULLABLE_VALUE_TYPE;

typedef struct _NULLABLE_BYTE {
  bool exist;
  uint8_t value;
} NULLABLE_BYTE;

typedef struct _NULLABLE_BYTE_SEQ {
  bool isTerminated;
  uint32_t addr;
  char* s;
} NULLABLE_BYTE_SEQ, *PNULLABLE_BYTE_SEQ;


NULLABLE_VALUE_TYPE get_value_type(NULLABLE_BYTE* b) __attribute__((nonnull (1)));

NULLABLE_BYTE_SEQ* make_nullable_byte_seq(uint32_t addr, char* restrict s) __attribute__((nonnull (2)));

// sequenceがisTerminatedならexist=falseの値を返す
NULLABLE_BYTE get_value_from_seq(NULLABLE_BYTE_SEQ* seq) __attribute__((nonnull (1)));

void free_nullable_byte_seq(NULLABLE_BYTE_SEQ* seq);


#endif  // _NULLABLE_H_
