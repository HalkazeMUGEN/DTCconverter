#include "decode.h"
#include "dtcgen.h"
#include "nullable.h"
#include "safemem.h"
#include "strutil.h"
#include <string.h>
#include <regex.h>


#define ADDR_LABEL_REGEX "(0x)?[0-9a-fA-F]{1,8}:"


static uint32_t read_addr_label(FILE* fp) __attribute__((nonnull (1)));
static bool is_addr_label(const char* str);
static char* make_bytes_str_from_read_values(FILE* fp) __attribute__((nonnull (1)));


void decode(FILE* fp) {
  uint32_t addr;
  while ((addr = read_addr_label(fp)) != 0) {
    fprintf(output, "; 0x%08X -\n", addr);
    NULLABLE_BYTE values[UNIT_OF_BYTES] = {0};
    NULLABLE_BYTE_SEQ* seq = make_nullable_byte_seq(addr, make_bytes_str_from_read_values(fp));
    while (!(seq->isTerminated)) {
      // 読み込み + DTCコード生成 (DTCコードが完成すれば出力)
      generate_mugen_code(seq, values);
    }
    // 未出力のDTCコードを出力
    generate_mugen_code_from_buffer(seq, values);
    free_nullable_byte_seq(seq);
    seq = NULL;
    fprintf(output, "\n");
  }
}


static uint32_t read_addr_label(FILE* fp) {
  char buf[LIMIT_CHARS];
  uint32_t addr = 0;
  while (fgets(buf, LIMIT_CHARS, fp)) {
    char* p = strchr(buf, '\n');
    if (p) *p = '\0';
    if (is_addr_label(buf)) {
      int i;
      if (sscanf(buf, "%i", &i) != 1) {
        show_error("Can't parse address label.");
      }
      addr = (uint32_t)i;
      break;
    }
  }
  return addr;
}

static bool is_addr_label(const char* str) {
  static regex_t regex = {0};
  if (regex.re_nsub == 0) {
    int err = regcomp(&regex, ADDR_LABEL_REGEX, REG_EXTENDED);
    if (err != 0) {
      show_error(ERRMSG_FAIL_REGCOMP);
    }
  }

  regmatch_t match;
  return (regexec(&regex, str, 1, &match, 0) != REG_NOMATCH);
}

static char* make_bytes_str_from_read_values(FILE* fp) {
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

  return s;
}


#undef ADDR_LABEL_REGEX
