#include "dtcgen.h"
#include <string.h>


static uint32_t written = 0;
static char text[LIMIT_CHARS] = {0};
static size_t paramsCnt = 0;
static uint32_t params[MAX_PARAMS];

static void print_dtc_code(char* restrict text, size_t paramsCnt, uint32_t params[restrict static paramsCnt]) __attribute__((nonnull (1, 3)));
static void print_warning(uint32_t addr);


void generate_mugen_code(NULLABLE_BYTE_SEQ* restrict seq, NULLABLE_BYTE values[static restrict UNIT_OF_BYTES]) {
  assert(values != NULL);

  size_t d = 0;
  if ((get_value_type(&values[1]) == NVT_AA)
  || (get_value_type(&values[0]) == NVT_XX && get_value_type(&values[1]) != NVT_XX)
  || (get_value_type(&values[0]) == NVT_AA && get_value_type(&values[1]) == NVT_00 && get_value_type(&values[2]) == NVT_00 && get_value_type(&values[3]) == NVT_XX)) {
    d = 1;
  } else if ((get_value_type(&values[2]) == NVT_AA)
  || (get_value_type(&values[0]) != NVT_00 && get_value_type(&values[1]) == NVT_XX && get_value_type(&values[2]) == NVT_00)) {
    d = 2;
  } else if ((get_value_type(&values[2]) == NVT_XX && get_value_type(&values[3]) != NVT_XX)
  || (get_value_type(&values[0]) == NVT_00 && get_value_type(&values[1]) == NVT_XX && get_value_type(&values[2]) == NVT_00 && get_value_type(&values[3]) == NVT_AA)
  || (get_value_type(&values[0]) != NVT_XX && get_value_type(&values[1]) == NVT_00 && get_value_type(&values[2]) == NVT_00 && get_value_type(&values[3]) == NVT_AA)) {
    d = 3;
  } else {
    d = 4;
  }

  switch (d) {
    case 1:
      values[0] = values[1];
      values[1] = values[2];
      values[2] = values[3];
      values[3] = get_value_from_seq(seq);
      break;
    case 2:
      values[0] = values[2];
      values[1] = values[3];
      values[2] = get_value_from_seq(seq);
      values[3] = get_value_from_seq(seq);
      break;
    case 3:
      values[0] = values[3];
      values[1] = get_value_from_seq(seq);
      values[2] = get_value_from_seq(seq);
      values[3] = get_value_from_seq(seq);
      break;
    case 4:
      values[0] = get_value_from_seq(seq);
      values[1] = get_value_from_seq(seq);
      values[2] = get_value_from_seq(seq);
      values[3] = get_value_from_seq(seq);
      break;
  }

  char bufText[7];
  size_t bufParamsCnt;
  uint32_t bufParams[2];
  if (get_value_type(&values[0]) == NVT_00 && get_value_type(&values[1]) != NVT_AA && get_value_type(&values[2]) == NVT_00) {
    strcpy(bufText, "%n");
    bufParamsCnt = 1;
    bufParams[0] = seq->addr - 4;
  } else if (get_value_type(&values[0]) == NVT_00) {
    strcpy(bufText, "%hn");
    bufParamsCnt = 1;
    bufParams[0] = seq->addr - 4;
  } else if (get_value_type(&values[0]) == NVT_AA && get_value_type(&values[1]) == NVT_00 && get_value_type(&values[2]) == NVT_00 && get_value_type(&values[3]) != NVT_XX) {
    strcpy(bufText, "%*B%n");
    bufParamsCnt = 2;
    bufParams[0] = values[0].value;
    bufParams[1] = seq->addr - 4;
  } else if (get_value_type(&values[0]) == NVT_AA) {
    strcpy(bufText, "%*B%hn");
    bufParamsCnt = 2;
    bufParams[0] = values[0].value;
    bufParams[1] = seq->addr - 4;
  } else {
    bufText[0] = '\0';
    bufParamsCnt = 0;
  }

  bool print = false;
  if (get_value_type(&values[0]) == NVT_00 && get_value_type(&values[1]) != NVT_AA) {
    // written == 0 条件付き
    if (written != 0) print = true;
  } else if (get_value_type(&values[0]) == NVT_AA && (get_value_type(&values[1]) == NVT_XX || (get_value_type(&values[1]) == NVT_00 && (get_value_type(&values[2]) != NVT_00 || get_value_type(&values[3]) != NVT_XX)))) {
    // written <= values[0].value 条件付き
    if (written > values[0].value) print = true;
  }
  if (!print && paramsCnt + bufParamsCnt > 5) {
    print = true;
  }

  if (print && text[0] != '\0') {
    print_dtc_code(text, paramsCnt, params);
    written = 0;
    text[0] = '\0';
    paramsCnt = 0;
  }

  if (get_value_type(&values[0]) != NVT_XX && get_value_type(&values[1]) == NVT_XX) {
    print_warning(seq->addr - 3);
  }
  if (get_value_type(&values[0]) == NVT_00 && get_value_type(&values[1]) != NVT_AA && get_value_type(&values[2]) == NVT_00 && get_value_type(&values[3]) == NVT_XX) {
    print_warning(seq->addr - 1);
  }

  if (bufParamsCnt == 1) {
    strcat(text, bufText);
    params[paramsCnt] = bufParams[0];
    ++paramsCnt;
  } else if (bufParamsCnt == 2) {
    strcat(text, bufText);
    params[paramsCnt] = (0x100 + bufParams[0] - written) & 0xFF;
    written += params[paramsCnt];
    params[paramsCnt + 1] = bufParams[1];
    paramsCnt += 2;
  }
}

void generate_mugen_code_from_buffer(NULLABLE_BYTE_SEQ* restrict seq, NULLABLE_BYTE values[static restrict UNIT_OF_BYTES]) {
  while (get_value_type(&values[0]) != NVT_XX || get_value_type(&values[1]) != NVT_XX || get_value_type(&values[2]) != NVT_XX || get_value_type(&values[3]) != NVT_XX) {
    generate_mugen_code(seq, values);
  }

  if (text[0] != '\0') {
    print_dtc_code(text, paramsCnt, params);
    written = 0;
    text[0] = '\0';
    paramsCnt = 0;
  }
}


static void print_dtc_code(char* restrict text, size_t paramsCnt, uint32_t params[restrict static paramsCnt]) {
  assert(text != NULL);
  assert(params != NULL);
  assert(paramsCnt <= MAX_PARAMS);

  fprintf(output, "[State ]\n");
  fprintf(output, "type=DisplayToClipboard\n");
  fprintf(output, "trigger1=1\n");
  fprintf(output, "text=\"%s\"\n", text);
  switch (paramsCnt) {
    case 0:
      break;
    case 1:
      fprintf(output, "params=%d\n", params[0]);
      break;
    case 2:
      fprintf(output, "params=%d,%d\n", params[0], params[1]);
      break;
    case 3:
      fprintf(output, "params=%d,%d,%d\n", params[0], params[1], params[2]);
      break;
    case 4:
      fprintf(output, "params=%d,%d,%d,%d\n", params[0], params[1], params[2], params[3]);
      break;
    default:
      fprintf(output, "params=%d,%d,%d,%d,%d\n", params[0], params[1], params[2], params[3], params[4]);
      break;
  }
  fprintf(output, "ignorehitpause=1\n");

  text[0] = '\0';
  paramsCnt = 0;
}

static void print_warning(uint32_t addr) {
  fprintf(stderr, "WARNING: The address of 0x%08X required keep its value, but it's overwritten to 0.\n", addr);
}
