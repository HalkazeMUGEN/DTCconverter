#include "safemem.h"


void* safe_malloc(size_t x) {
  void* ret = malloc(x);
  if (ret == NULL) {
    show_error(ERRMSG_FAIL_ALLOC);
  }
  return ret;
}

void* safe_calloc(size_t n, size_t size) {
  void* ret = calloc(n, size);
  if (ret == NULL) {
    show_error(ERRMSG_FAIL_ALLOC);
  }
  return ret;
}

void* safe_realloc(void* p, size_t x) {
  void* ret = realloc(p, x);
  if (ret == NULL) {
    show_error(ERRMSG_FAIL_ALLOC);
  }
  return ret;
}
