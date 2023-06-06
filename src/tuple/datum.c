
#include "tuple/datum.h"

Datum int64GetDatum(int64_t i) {
  return (Datum) i;
}

Datum charGetDatum(char* c) {
  return (Datum) c;
}

int64_t datumGetInt64(Datum d) {
  return (int64_t) d;
}

char* datumGetString(Datum d) {
  return (char*) d;
}