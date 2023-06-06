#ifndef TUPLE_H
#define TUPLE_H

#include <stdint.h>
#include <stdbool.h>

#include "datum.h"

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

typedef struct TupleDescriptor {
	int	natts;			/* number of attributes in the tuple */
	bool has_null;
	Column* cols[];
}	TupleDescriptor;

typedef char* Tuple;

typedef struct TupleHeader {
  uint8_t t_hoff;

  uint8_t t_null_bitmap[];

  /* User data takes up the remaining space */
} TupleHeader;

typedef struct TupleContainer {
  uint32_t t_len;
  void* t_self;
  TupleHeader* t_data; /* tuple header and user data */
} TupleContainer;


TupleDescriptor* construct_tuple_desc(char* tablename);
Column* get_column_desc(char* tablename, char* colname);

uint16_t compute_tuple_size(TupleDescriptor* td, Datum* values, bool* isnull);

void fill_tuple(TupleDescriptor* td, char* data, Datum* values, bool* isnull, uint8_t* bitmap);

#endif /* TUPLE_H */