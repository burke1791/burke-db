#ifndef COLUMN_H
#define COLUMN_H

#include <stdbool.h>

typedef enum DataType {
  DT_BIGINT,
  DT_CHAR,
  DT_UNKNOWN
} DataType;

typedef struct Column {
  char* c_name;
  DataType c_type;
  int c_num;
  int c_len;
  bool c_not_null;
} Column;

Column* construct_column_desc(char* name, DataType type, int colnum, int len, bool isNotNull);

#endif /* COLUMN_H */