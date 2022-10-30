#include "encode.h"
#include "addrlist.h"
#include "strutil.h"
#include "safemem.h"
#include "format.h"
#include <string.h>


#define MAX_PARAMS  5


static ADDR_MODIFIED_LIST* list;


static bool is_state_controller_start(char* line) __attribute__((nonnull (1)));
static void read_state_controller(FILE* fp) __attribute__((nonnull (1)));
static bool is_display_to_clipboard(char* line) __attribute__((nonnull (1)));
static void read_display_to_clipboard(FILE* fp) __attribute__((nonnull (1)));


void encode(FILE* fp) {
  list = list_init();
  char buf[LIMIT_CHARS];
  while (fgets(buf, LIMIT_CHARS, fp)) {
    char* p = strrchr(buf, '\n');
    if (p) *p = '\0';
    if (is_state_controller_start(buf)) {
      read_state_controller(fp);
    }
  }
  list_print(list);
  list_free(list);
  list = NULL;
}


static bool is_state_controller_start(char* line) {
  assert(line != NULL);

  if (line[0] == '[') {
    char* p = strchr(line, ';');
    if (p) *p = '\0';
    trim(line, "\t ");
    size_t len = strlen(line);
    tolowers(line, len);
    return (strncmp(line, "[state ", 7) == 0 && line[len - 1] == ']');
  }

  return false;
}

static void read_state_controller(FILE* fp) {
  assert(fp != NULL);

  long sc_offset = ftell(fp);
  long offset = ftell(fp);
  char buf[LIMIT_CHARS];
  while (fgets(buf, LIMIT_CHARS, fp)) {
    char* p = strrchr(buf, '\n');
    if (p) *p = '\0';
    if (is_state_controller_start(buf)) {
      fseek(fp, offset, SEEK_SET);
      break;
    }
    offset = ftell(fp);

    if (is_display_to_clipboard(buf)) {
      fseek(fp, sc_offset, SEEK_SET);
      read_display_to_clipboard(fp);
      break;
    }
  }
}

static bool is_display_to_clipboard(char* line) {
  assert(line != NULL);

  line = ltrim(line, "\t ");
  if (strncmp(line, "type", 4) == 0) {
    line = ltrim(&line[4], "\t ");
    if (line[0] == '=') {
      char* p = strchr(line, ';');
      if (p) *p = '\0';
      line = trim(&line[1], "\t ");
      tolowers(line, LIMIT_CHARS);
      return (strcmp(line, "displaytoclipboard") == 0 || strcmp(line, "appendtoclipboard") == 0);
    }
  }
  return false;
}

static void read_display_to_clipboard(FILE* fp) {
  assert(fp != NULL);

  bool find_text = false;
  bool find_params = false;

  char text[LIMIT_CHARS];
  uint32_t params[MAX_PARAMS] = {0};

  long offset = ftell(fp);

  char buf[LIMIT_CHARS];
  while (fgets(buf, LIMIT_CHARS, fp)) {
    char* p = strrchr(buf, '\n');
    if (p) *p = '\0';
    if (is_state_controller_start(buf)) {
      fseek(fp, offset, SEEK_SET);
      break;
    }
    offset = ftell(fp);

    ltrim(buf, "\t ");
    if (!find_text && strncmp(buf, "text", 4) == 0) {
      char* line = ltrim(&buf[4], "\t ");
      if (line[0] == '=') {
        find_text = true;
        char* p = strchr(line, ';');
        if (p) *p = '\0';
        line = trim(&line[1], "\t ");
        size_t len = strlen(line);
        if (line[0] == '\"' && line[len-1] == '\"') {
          line[len-1] = '\0';
          strcpy(text, &line[1]);
        }
      }
    } else if (!find_params && strncmp(buf, "params", 6) == 0) {
      char* line = ltrim(&buf[6], "\t ");
      if (line[0] == '=') {
        line = ltrim(&line[1], "\t ");
        char* origin = (char*)safe_malloc(strlen(buf) + 1);
        strcpy(origin, line);
        find_params = true;
        char* p = strchr(line, ';');
        if (p) *p = '\0';
        line = rtrim(line, "\t ");
        while (!split_values(params, 6, line, ',')) {
          fprintf(output, "Failed to parse...\nOriginal params: %s\nInput correct params: ", origin);
          fgets(line, LIMIT_CHARS, stdin);
          p = strrchr(buf, '\n');
          if (p) *p = '\0';
        }
        free(origin);
        origin = NULL;
      }
    }
  }

  if (find_text && find_params) {
    fprintf(stderr, "text: %s\n", text);
    fprintf(stderr, "params: ");
    for (size_t i = 0; i < MAX_PARAMS; ++i) {
      if (i != 0) fprintf(stderr, ", ");
      fprintf(stderr, "%d", params[i]);
    }
    fprintf(stderr, "\n");
    buf[0] = '\0';
    char* fmt = text;
    char* prev = fmt;
    size_t len = 0;
    uint32_t printed = 0;
    uint32_t params_use[MAX_PARAMS] = {0};
    size_t used = 0;
    while ((fmt = get_head_of_format(FT_N, fmt, &len)) != NULL) {
      bool isHalf = has_half_format(fmt, len);
      strncpy_s(buf, LIMIT_CHARS, prev, fmt - prev);
      size_t consumed = get_number_of_formats(buf);
      for (size_t i = 0; i < consumed; ++i) {
        params_use[i] = params[used + i];
      }
      used += consumed;
      if (used >= MAX_PARAMS) {
        fprintf(stderr, "text: %s\n", text);
        show_error("It use more than max params!");
      }
      printed += get_length_of_formatted_str(buf, params_use, consumed);

      fprintf(stderr, "[0x%08X] := 0x%02X\n", params[used], printed);

      list_append_values(list, params[used], (printed&0xFF));
      list_append_values_with_weak(list, params[used] + 0x1, (printed&0xFF00) >> 8, true);
      if (!isHalf) {
        list_append_values_with_weak(list, params[used] + 0x2, (printed&0xFF0000) >> 16, true);
        list_append_values_with_weak(list, params[used] + 0x3, (printed&0xFF000000) >> 24, true);
      }

      ++used;
      fmt = &fmt[len];
      prev = fmt;
    }
    fprintf(stderr, "\n");
  }
}
