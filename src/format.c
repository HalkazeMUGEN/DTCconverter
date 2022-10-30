#include "format.h"
#include "safemem.h"
#include <string.h>
#include <regex.h>

#define FORMAT_REGEX_NO_TYPE  "%[-+0 #]*(\\*)?[0-9]*(\\.\\*|\\.|)[0-9]*[hlF]*" // Tuned for MUGEN
#define FORMAT_REGEX          "%%|%[-+0 #]*(\\*)?[0-9]*(\\.\\*|\\.|)[0-9]*[hlF]*([BCEGSXZcdefginopsux]?)" // Tuned for MUGEN

#define testmsg %% %c % 0 0 ###*.*1lllhn %d



static int get_number_of_format(const char* restrict format, size_t* restrict plen) __attribute__((nonnull (1, 2)));


char* get_head_of_format(FORMAT_TYPES target, const char* restrict text, size_t* restrict plen) {
  assert(text != NULL);

  static regex_t regex = {0};
  if (regex.re_nsub == 0) {
    int err = regcomp(&regex, FORMAT_REGEX_NO_TYPE, REG_EXTENDED);
    if (err != 0) {
      show_error(ERRMSG_FAIL_REGCOMP);
    }
  }

  *plen = 0;
  char* p = strchr(text, '%');
  regmatch_t match;
  while (p) {
    if (regexec(&regex, p, 1, &match, 0) != REG_NOMATCH) {
      if (p[match.rm_eo] == (char)(target)) {
        *plen = match.rm_eo - match.rm_so + 1;
        return p + match.rm_so;
      }
      p += match.rm_eo - match.rm_so;
    }
    p = strchr(&p[1], '%');
  }
  return NULL;
}

int get_number_of_formats(const char* format) {
  assert(format != NULL);

  int sum = 0;
  char* p = strchr(format, '%');
  while (p) {
    size_t len;
    int cnt = get_number_of_format(p, &len);
    sum += cnt;
    p += len;
    p = strchr(p, '%');
  }
  return sum;
}

size_t get_length_of_formatted_str(const char* restrict format, const uint32_t* restrict args, size_t argc) {
  assert(format != NULL);
  assert(args != NULL);

  char* text = (char*)format;

  size_t len = 0;
  char* b = get_head_of_format('B', format, &len);
  if (b != NULL) {
    size_t textLen = strlen(format) + 75;
    text = (char*)safe_malloc(textLen);
    strcpy(text, format);
    char* buf = (char*)safe_malloc(textLen);
    b = (b - format) + text;
    do {
      if (strncmp(b, "%B", 2) == 0) {
        sprintf(buf, "%.*s%s", (size_t)(b - text), text, &text[(b - text) + len]);
        strcpy(text, buf);
      } else if (strncmp(b, "%*B", 3) == 0) {
        text[(b-text)] = '\0';
        int cnt = get_number_of_formats(text);
        sprintf(buf, "%.*s%%%d.0s%s", (size_t)(b - text), text, args[cnt], &text[(b - text) + len]);
        strcpy(text, buf);
      } else if (strncmp(b, "%*.*B", 5) == 0) {
        text[(b-text)] = '\0';
        int cnt = get_number_of_formats(text);
        sprintf(buf, "%.*s%%%d.0s%%0.0s%s", (size_t)(b - text), text, args[cnt], &text[(b - text) + len]);
        strcpy(text, buf);
      } else {
        show_error("Can't expect the behavior of %B...");
      }
      b = get_head_of_format('B', text, &len);
    } while (b);
    free(buf);
  }

  len = 0;
  switch (argc) {
  case 0:
    len = (size_t)snprintf(NULL, 0, text);
    break;
  case 1:
    len = (size_t)snprintf(NULL, 0, text, args[0]);
    break;
  case 2:
    len = (size_t)snprintf(NULL, 0, text, args[0], args[1]);
    break;
  case 3:
    len = (size_t)snprintf(NULL, 0, text, args[0], args[1], args[2]);
    break;
  case 4:
    len = (size_t)snprintf(NULL, 0, text, args[0], args[1], args[2], args[3]);
    break;
  case 5:
    len = (size_t)snprintf(NULL, 0, text, args[0], args[1], args[2], args[3], args[4]);
    break;
  default:
    show_error("Required over MAX_PARAMS!");
  }

  if (text != format) {
    free(text);
  }

  return len;
}


static int get_number_of_format(const char* restrict format, size_t* restrict plen) {
  assert(format != NULL);
  assert(plen != NULL);

  static regex_t regex = {0};
  if (regex.re_nsub == 0) {
    int err = regcomp(&regex, FORMAT_REGEX, REG_EXTENDED);
    if (err != 0) {
      show_error(ERRMSG_FAIL_REGCOMP);
    }
  }

  *plen = 0;
  int cnt = 0;
  regmatch_t match[4] = {0};
  if (regexec(&regex, format, sizeof(match) / sizeof(regmatch_t), match, 0) != REG_NOMATCH) {
    *plen = match[0].rm_eo - match[0].rm_so;
    cnt += match[1].rm_eo - match[1].rm_so;
    cnt += (match[2].rm_eo - match[2].rm_so == 2);
    if (match[3].rm_so != -1 && match[3].rm_eo != -1) {
      switch (format[match[3].rm_so]) {
      case 'c':
      case 'C':
      case 'd':
      case 'i':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
      case 'n':
      case 'p':
      case 's':
      case 'S':
      case 'Z':
        cnt++;
        break;
      case 'e':
      case 'E':
      case 'f':
      case 'g':
      case 'G':
        cnt += 2;
        break;
      default:
        break;
      }
    }
  }

  return cnt;
}

#undef FORMAT_ALL_TYPES
