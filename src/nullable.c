#include "nullable.h"
#include "decode.h"
#include "safemem.h"
#include "strutil.h"
#include <string.h>


NULLABLE_BYTE_SEQ* make_nullable_byte_seq(FILE* fp) {
  assert(fp != NULL);

  long start_offset = ftell(fp);
  long end_offset = start_offset;
  bool find = false;
  char buf[LIMIT_CHARS];
  while (fgets(buf, LIMIT_CHARS, fp)) {
    if (is_addr_label(buf)) {
      find = true;
      break;
    }
    end_offset = ftell(fp);
  }
  if (!find) {
    end_offset = ftell(fp);
  }

  assert(end_offset + 1 > start_offset);
  size_t size = end_offset - start_offset + 1;
  char* s = (char*)safe_malloc(size);
  s[0] = '\0';

  fseek(fp, start_offset, SEEK_SET);
  while (fgets(buf, LIMIT_CHARS, fp)) {
    if (is_addr_label(buf)) break;
    char* p = strchr(buf, ';');
    if (p) {
      *p = '\0';
    } else if ((p = strchr(buf, '\n'))) {
      *p = '\0';
    }
    trim(buf, "\t ");
    if (buf[0] != '\0') {
      sprintf(s, "%s %s", s, buf);
    }
  }
  fseek(fp, end_offset, SEEK_SET);

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
