#ifndef _DTC_CONVERTER_H_
#define _DTC_CONVERTER_H_


#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#define LIMIT_CHARS           256

#define ERRMSG_FAIL_OPEN_FILE "Can't open file."
#define ERRMSG_FAIL_ALLOC     "Can't get memory."
#define ERRMSG_FAIL_REGCOMP   "Can't compile regex."


static inline _Noreturn void show_error(const char* errmsg) __attribute__((nonnull (1)));


static inline _Noreturn void show_error(const char* errmsg) {
  assert(errmsg != NULL);

  fputs(errmsg, stderr);
  exit(1);
}


#endif  // _DTC_CONVERTER_H_
