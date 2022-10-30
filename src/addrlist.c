#include "addrlist.h"
#include "safemem.h"


static int compare_addr(const void* a, const void* b);


ADDR_MODIFIED_LIST* list_init(void) {
  ADDR_MODIFIED_LIST* list = (ADDR_MODIFIED_LIST*)safe_malloc(sizeof(ADDR_MODIFIED_LIST));
  list->n = 0;
  list->limit = LIST_INITIAL_LIMIT;
  list->data = (ADDR_MODIFIED_DATA*)safe_malloc(sizeof(ADDR_MODIFIED_DATA) * LIST_INITIAL_LIMIT);
  return list;
}

ADDR_MODIFIED_LIST* list_append_data(ADDR_MODIFIED_LIST* restrict list, const ADDR_MODIFIED_DATA* restrict data) {
  assert(list != NULL);
  assert(data != NULL);

  return list_append_values(list, data->addr, data->value);
}

ADDR_MODIFIED_LIST* list_append_values(ADDR_MODIFIED_LIST* restrict list, const uint32_t addr, const uint8_t value) {
  assert(list != NULL);

  if ((list->n + 1) > (list->limit)) {
    list->limit *= 2;
    list->data = (ADDR_MODIFIED_DATA*)safe_realloc(list->data, sizeof(ADDR_MODIFIED_DATA) * list->limit);
  }

  *(uint32_t*)(&(list->data[list->n].addr)) = addr;
  *(uint8_t*)(&(list->data[list->n].value)) = value;
  ++(list->n);

  return list;
}

ADDR_MODIFIED_LIST* list_sort(ADDR_MODIFIED_LIST* list) {
  assert(list != NULL);

  qsort(list->data, list->n, sizeof(ADDR_MODIFIED_DATA), compare_addr);
  return list;
}

void list_print(ADDR_MODIFIED_LIST* list) {
  assert(list != NULL);

  fprintf(output, "===== MODIFIED ADDRESSES =====\n");
  size_t n = (size_t)(list->n);
  size_t i = 0;
  while (i < n) {
    uint32_t last_addr = list_get_addr(list, i);
    fprintf(output, "\n0x%08X:\n", last_addr);
    fprintf(output, " %02hhX", list_get_value(list, i));
    ++i;
    size_t x = 1;
    while (true) {
      uint32_t addr = list_get_addr(list, i);
      if (addr == last_addr + 1) {
        fprintf(output, " %02hhX", list_get_value(list, i));
        last_addr = addr;
      } else if (addr - last_addr > 0x00 && addr - last_addr <= 0x10) {
        do {
          fprintf(output, " XX");
          ++last_addr;
          ++x;
          if (x % 16 == 0) {
            x = 0;
            fprintf(output, "\n");
          }
        } while (addr != last_addr);
        fprintf(output, " %02hhX", list_get_value(list, i));
      } else {
        fprintf(output, "\n");
        break;
      }
      ++i;
      ++x;
      if (x % 16 == 0) {
        x = 0;
        fprintf(output, "\n");
      }
    }
  }
  fprintf(output, "\n==============================\n");
}

void list_free(ADDR_MODIFIED_LIST* list) {
  free(list->data);
  list->data = NULL;
  free(list);
}


static int compare_addr(const void* a, const void* b) {
  return (((ADDR_MODIFIED_DATA*)a)->addr) - (((ADDR_MODIFIED_DATA*)b)->addr);
}
