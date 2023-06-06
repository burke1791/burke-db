#ifndef DATUM_H
#define DATUM_H

#include <stdint.h>

typedef unsigned long Datum;

Datum int64GetDatum(int64_t i);
Datum charGetDatum(char* c);

int64_t datumGetInt64(Datum d);
char* datumGetString(Datum d);

#endif /* DATUM_H */