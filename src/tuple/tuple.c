#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tuple/tuple.h"

bool att_isnull(int attnum, uint8_t* bitmap) {
  return !(bitmap[attnum >> 3] & (1 << (attnum & 0x07)));
}

static TupleDescriptor* allocate_tuple_desc(int numCols) {
  TupleDescriptor* td = malloc(sizeof(TupleDescriptor) + (numCols * sizeof(Column)));
  return td;
}

TupleDescriptor* construct_tuple_desc(char* tablename) {
  // get the number of columns from the system table
  TupleDescriptor* td = allocate_tuple_desc(2);
  td->natts = 2;
  td->has_null = false;

  // look up columns in system table
  // loop through columns and build Column array in TupleDescriptor
  for (int i = 0; i < 2; i++) {
    if (i == 0) {
      td->cols[i] = get_column_desc("employee", "employee_id");
    } else if (i == 1) {
      td->cols[i] = get_column_desc("employee", "name");
      td->has_null = true;
    }
  }
}

static Column* allocate_column() {
  Column* c = malloc(sizeof(Column));
  return c;
}

static Column* construct_column_desc(char* name, DataType type, int colnum, int len, bool isNotNull) {
  Column* c = allocate_column();

  c->c_name = strdup(name);
  c->c_type = type;
  c->c_num = colnum;
  c->c_len = len;
  c->c_not_null = isNotNull;

  return c;
}

Column* get_column_desc(char* tablename, char* colname) {
  // look up column details in the system table
  if (strcmp(tablename, "employee") != 0) return NULL;

  if (strcmp(colname, "employee_id") == 0) {
    return construct_column_desc(colname, DT_BIGINT, 0, 8, true);
  } else if (strcmp(colname, "name") == 0) {
    return construct_column_desc(colname, DT_CHAR, 0, 20, false);
  }

  return NULL;
}

uint16_t compute_tuple_size(TupleDescriptor* td, Datum* values, bool* isnull) {
  // hard-coded for now
  /*
    t_hoff: 1 byte
    t_null_bitmap: 1 byte
    employee_id: 8 bytes
    name: 20 bytes
  */
  return 30;
}

static void fill_val(Column* col, uint8_t** bit, int* bitmask, char** dataP, Datum datum, bool isnull) {
  int16_t dataLen;
  char* data = *dataP;

  if (bit != NULL) {
    if (*bitmask != 0x80) {
      *bitmask <<= 1;
    } else {
      *bit += 1;
      **bit = 0x0;
      *bitmask = 1;
    }
  }

  **bit |= *bitmask;

  if (!isnull) {
    switch (col->c_type) {
      case DT_BIGINT:
        dataLen = 8;
        int64_t valBigInt = datumGetInt64(datum);
        memcpy(data, &valBigInt, dataLen);
        break;
      case DT_CHAR:
        dataLen = col->c_len;
        char* str = datumGetString(datum);
        memcpy(data, str, strlen(str));
        break;
      case DT_UNKNOWN:
        printf("Unknown data type! (fill_val)\n");
        break;
    }

    data += dataLen;
    *dataP = data;
  }
}

void fill_tuple(TupleDescriptor* td, char* data, Datum* values, bool* isnull, uint8_t* bitmap) {
  uint8_t* bitP;
  int bitmask;

  if (bitmap != NULL) {
    bitP = &bitmap[-1];
    bitmask = 0x80;
  } else {
    bitP = NULL;
    bitmask = 0;
  }

  for (int i = 0; i < td->natts; i++) {
    fill_val(
      td->cols[i],
      bitP ? &bitP : NULL,
      &bitmask,
      &data,
      values[i],
      isnull[i]
    );
  }
}

int calculate_att_size(Column* col, Tuple tup, int offset) {
  switch (col->c_type) {
    case DT_BIGINT:
      return 8;
    case DT_CHAR:
      return col->c_len;
    default:
      printf("Unknown data type! (calculate_att_size)\n");
  }
}

Datum* get_tuple_att(Column* col, Tuple tup, int offset, int attSize) {
  Datum* d = malloc(attSize);
  memset(d, 0, attSize);
  memcpy(d, tup + offset, attSize);
}