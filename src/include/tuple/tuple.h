#ifndef TUPLE_H
#define TUPLE_H

#include <stdint.h>
#include <stdbool.h>

#include "column.h"

#define TUPLE_SIZE 2

typedef struct TupleDescriptor {
	int	natts;			/* number of attributes in the tuple */
	bool has_null;
	Column* cols[];
}	TupleDescriptor;

typedef unsigned long int Datum;
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

TupleDescriptor* get_tup_descriptor(char* tablename);
void construct_datum(void* values, )
void calc_tup_len(TupleDescriptor* td, )
TupleContainer* form_tuple(Datum* values, bool* isnull);


#endif /* TUPLE_H */