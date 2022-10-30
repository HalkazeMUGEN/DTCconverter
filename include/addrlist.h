#ifndef _ADDR_LIST_H_
#define _ADDR_LIST_H_


#include "DTCconverter.h"


#define LIST_INITIAL_LIMIT 128


typedef struct _ADDR_MODIFIED_DATA {
  const uint32_t addr;
  const uint8_t value;
  const bool weak;
} ADDR_MODIFIED_DATA, *PADDR_MODIFIED_DATA;

typedef struct _ADDR_MODIFIED_LIST {
  int n;
  int limit;
  ADDR_MODIFIED_DATA* data;
} ADDR_MODIFIED_LIST, *PADDR_MODIFIED_LIST;


#define list_get(list, x)       ((list)->data[(x)])
#define list_get_addr(list, x)  (((list)->data[(x)]).addr)
#define list_get_value(list, x) (((list)->data[(x)]).value)


ADDR_MODIFIED_LIST* list_init(void);
ADDR_MODIFIED_LIST* list_append_data(ADDR_MODIFIED_LIST* restrict list, const ADDR_MODIFIED_DATA* restrict data) __attribute__((nonnull (1, 2)));
ADDR_MODIFIED_LIST* list_append_values(ADDR_MODIFIED_LIST* list, const uint32_t addr, const uint8_t value) __attribute__((nonnull (1)));
ADDR_MODIFIED_LIST* list_append_values_with_weak(ADDR_MODIFIED_LIST* list, const uint32_t addr, const uint8_t value, const bool weak) __attribute__((nonnull (1)));
ADDR_MODIFIED_DATA* list_search_weak_addr(ADDR_MODIFIED_LIST* list, const uint32_t addr) __attribute__((nonnull (1)));
ADDR_MODIFIED_LIST* list_sort(ADDR_MODIFIED_LIST* list) __attribute__((nonnull (1)));
void list_print(ADDR_MODIFIED_LIST* list) __attribute__((nonnull (1)));
void list_free(ADDR_MODIFIED_LIST* list);


#endif  // _ADDR_LIST_H_
