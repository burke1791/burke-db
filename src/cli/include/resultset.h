#ifndef RESULTSET_OUTPUT_H
#define RESULTSET_OUTPUT_H

#include "tuple/tuple.h"

typedef struct TupleDescriptor ResultDescriptor;

typedef enum ListType {
  L_Row,
  L_Col
} ListType;

typedef struct Res {
  ListType type;
} Res;

typedef struct ListItem {
  void* ptr_value;
} ListItem;

typedef struct List {
  ListType type;
  int length;
  int max_length;
  ListItem* items;
} List;

typedef struct ResultCell {
  Column* col;
  Datum dat;
} ResultCell;

List* new_list(ListType type);
void free_list(List* l);
void list_append(List* l, void* l_item);

void print_resultset(List* res);

#endif /* RESULTSET_OUTPUT_H */