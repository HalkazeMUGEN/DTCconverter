#ifndef _NULLABLE_H_
#define _NULLABLE_H_


#include "DTCconverter.h"


#define SEQUENCE_DELIMS " "


typedef struct _NULLABLE_BYTE {
  bool exist;
  uint8_t value;
} NULLABLE_BYTE;

typedef struct _NULLABLE_BYTE_SEQ {
  bool isTerminated;
  char* s;
} NULLABLE_BYTE_SEQ, *PNULLABLE_BYTE_SEQ;


// 次のラベル開始直前/ファイル終端までfpを進める
NULLABLE_BYTE_SEQ* make_nullable_byte_seq(char* s) __attribute__((nonnull (1)));

// sequenceがisTerminatedならexist=falseの値を返す
NULLABLE_BYTE get_value_from_seq(NULLABLE_BYTE_SEQ* seq) __attribute__((nonnull (1)));

void free_nullable_byte_seq(NULLABLE_BYTE_SEQ* seq);


#endif  // _NULLABLE_H_
