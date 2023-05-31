#ifndef DATUM_H
#define DATUM_H

#include <stdbool.h>

typedef enum DatumTag {
  DAT_
}

typedef struct Datum {
  DataType d_type;
  char* tablename;
  int col_num;
  int d_len; /* -1 for variable length; -2 for fixed length cols */
} Datum;

Datum* construct_datum(char* tablename, int col_num, void* val);
void serialize_data(Datum* values, bool* isnull);

#endif /* DATUM_H */