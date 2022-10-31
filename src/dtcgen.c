#include "dtcgen.h"


void generate_mugen_code(NULLABLE_BYTE_SEQ* restrict seq, NULLABLE_BYTE values[static restrict UNIT_OF_BYTES]) {
  assert(seq != NULL);
  assert(values != NULL);

  values[0] = get_value_from_seq(seq);
  values[1] = get_value_from_seq(seq);
  values[2] = get_value_from_seq(seq);
  values[3] = get_value_from_seq(seq);

  if (values[0].exist) {
    printf(" %02X\n", values[0].value);
  } else {
    printf(" XX\n");
  }
  if (values[1].exist) {
    printf(" %02X\n", values[1].value);
  } else {
    printf(" XX\n");
  }
  if (values[2].exist) {
    printf(" %02X\n", values[2].value);
  } else {
    printf(" XX\n");
  }
  if (values[3].exist) {
    printf(" %02X\n", values[3].value);
  } else {
    printf(" XX\n");
  }
}

void generate_mugen_code_from_buffer(void) {
}
