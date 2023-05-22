#ifndef EMP_LIST_H
#define EMP_LIST_H

#include "employee/emp_tuple.h"

typedef struct ListItem {
  void* ptr_value;
} ListItem;

typedef struct List {
  int length;
  int max_length;
  ListItem* items;
} List;

List* new_list();
void free_list(List* l);
void list_append(List* l, Employee* emp);

#endif /* EMP_LIST_H */