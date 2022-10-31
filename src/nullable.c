#include "nullable.h"
#include "safemem.h"
#include <string.h>


NULLABLE_BYTE_SEQ* make_nullable_byte_seq(char* s) {
  assert(s != NULL);

  NULLABLE_BYTE_SEQ* seq = (NULLABLE_BYTE_SEQ*)safe_malloc(sizeof(NULLABLE_BYTE_SEQ));
  seq->isTerminated = false;
  seq->s = s;
  return seq;
}

NULLABLE_BYTE get_value_from_seq(NULLABLE_BYTE_SEQ* seq) {
  assert(seq != NULL);

  NULLABLE_BYTE ret = { .exist = false };

  if (seq->isTerminated) {
    return ret;
  }

  static char* target_s = NULL;

  char* c;
  if (target_s != seq->s) {
    target_s = seq->s;
    c = strtok(seq->s, SEQUENCE_DELIMS);
  } else {
    c = strtok(NULL, SEQUENCE_DELIMS);
  }

  if (c == NULL) {
    target_s = NULL;
    seq->isTerminated = true;
  } else {
    uint8_t b;
    if (strncmp(c, UNMODIFIED_DATA, sizeof(UNMODIFIED_DATA)) != 0) {
      if (sscanf_s(c, "%02hhX", &b) != 1) {
        show_error("Can't parse value.");
      }
      ret.exist = true;
      ret.value = b;
    }
  }
  return ret;
}

void free_nullable_byte_seq(NULLABLE_BYTE_SEQ* seq) {
  if (seq == NULL) return;

  free(seq->s);
  free(seq);
}
