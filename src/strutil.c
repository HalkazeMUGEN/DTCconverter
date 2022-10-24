#include "strutil.h"
#include <string.h>
#include <ctype.h>


bool split_values(uint32_t* restrict dst, size_t n, const char* restrict src, char delim) {
  assert(dst != NULL);
  assert(src != NULL);

  char tmp[12];
  const char* str = src;
  char* p;
  char* q;
  size_t i = 0;
  do {
    strncpy_s(tmp, 12, str, 11);
    p = strchr(tmp, delim);
    if (p) {
      *p = '\0';
      str += (p - tmp) + 1;
    }
    trim(tmp, "\t ");
    long val = strtol(tmp, &q, 10);
    if (*q != '\0') {
      return false;
    }
    dst[i] = (unsigned long)val;
    ++i;
  } while(i < n && p);
  return true;
}

char* trim(char* restrict str, const char* restrict seps) {
  assert(str != NULL);
  assert(seps != NULL);

  return ltrim(rtrim(str, seps), seps);
}

char* ltrim(char* restrict str, const char* restrict seps) {
  assert(str != NULL);
  assert(seps != NULL);

  size_t totrim = strspn(str, seps);
  if (totrim > 0) {
    size_t len = strlen(str);
    if (totrim == len) {
      str[0] = '\0';
    } else {
      memmove(str, str + totrim, len + 1 - totrim);
    }
  }
  return str;
}

char* rtrim(char* restrict str, const char* restrict seps) {
  assert(str != NULL);
  assert(seps != NULL);

  int i = strlen(str) - 1;
  while (i >= 0 && strchr(seps, str[i]) != NULL) {
    str[i] = '\0';
    --i;
  }
  return str;
}

char* tolowers(char* str, size_t len) {
  assert(str != NULL);

  for (size_t i = 0; i < len && str[i] != '\0'; ++i) {
    str[i] = tolower(str[i]);
  }
  return str;
}
