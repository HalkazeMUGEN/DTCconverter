#ifndef _DTC_GEN_H_
#define _DTC_GEN_H_


#include "DTCconverter.h"
#include "nullable.h"


#define UNIT_OF_BYTES 4


void generate_mugen_code(NULLABLE_BYTE_SEQ* restrict seq, NULLABLE_BYTE values[static restrict UNIT_OF_BYTES]) __attribute__((nonnull (1, 2)));

void generate_mugen_code_from_buffer(NULLABLE_BYTE_SEQ* restrict seq, NULLABLE_BYTE values[static restrict UNIT_OF_BYTES]) __attribute__((nonnull (1, 2)));


#endif  // _DTC_GEN_H_
