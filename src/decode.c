#include "decode.h"
#include "dtcgen.h"
#include "nullable.h"
#include <string.h>
#include <regex.h.>


#define ADDR_LABEL_REGEX "(0x)?[0-9a-fA-F]{1,8}:"


void decode(FILE* fp) {
  uint32_t addr;
  while ((addr = read_addr_label(fp)) != 0) {
    fprintf(output, "addr: 0x%08X\n", addr);
    NULLABLE_BYTE values[UNIT_OF_BYTES] = {0};
    NULLABLE_BYTE_SEQ* seq = make_nullable_byte_seq(fp);
    while (!(seq->isTerminated)) {
      // 読み込み + DTCコード生成 (DTCコードが完成すれば出力)
      generate_mugen_code(seq, values);
    }
    // 未完成のDTCコードを出力
    generate_mugen_code_from_buffer();
    free_nullable_byte_seq(seq);
    seq = NULL;
  }
}

uint32_t read_addr_label(FILE* fp) {
  char buf[LIMIT_CHARS];
  uint32_t addr = 0;
  while (fgets(buf, LIMIT_CHARS, fp)) {
    char* p = strchr(buf, '\n');
    if (p) *p = '\0';
    if (is_addr_label(buf)) {
      if (sscanf(buf, "%X", &addr) != 1) {
        show_error("Can't parse address label.");
      }
      break;
    }
  }
  return addr;
}

bool is_addr_label(const char* str) {
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


#undef ADDR_LABEL_REGEX
