#include <string.h>

#include "tuple/column.h"

static Column* allocate_column() {
  Column* c = malloc(sizeof(Column));
  return c;
}

Column* construct_column_desc(char* name, DataType type, int colnum, int len, bool isNotNull) {
  Column* c = allocate_column();

  c->c_name = strdup(name);
  c->c_type = type;
  c->c_num = colnum;
  c->c_len = len;
  c->c_not_null = isNotNull;

  return c;
}