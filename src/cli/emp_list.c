#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/emp_list.h"
#include "employee/emp_tuple.h"

#define liptr(li)  ((li)->ptr_value)
#define llast(l)  liptr(list_last_cell(l))

#define list_make_ptr_cell(v) ((ListCell) {.ptr_value = (v)})

static inline ListItem* list_last_cell(const List *list) {
	return &list->items[list->length];
}

List* new_list() {
  List* l = malloc(sizeof(List));

  l->length = 0;
  l->max_length = 0;
  l->items = NULL;

  return l;
}

void free_list(List* l) {
  if (l != NULL) {
    for (int i = 0; i < l->length; i++) {
      // for now, we know the list items are always Employees
      if (((Employee*)l->items[i].ptr_value)->name != NULL) {
        emp_free((Employee*)l->items[i].ptr_value);
      }
    }
  }
}

static void enlarge_list(List* l) {
  if (l == NULL) {
    printf("ERROR: list is empty!\n");
    exit(EXIT_FAILURE);
  } else {
    if (l->items == NULL) {
      l->items = malloc(sizeof(ListItem));
    } else {
      l->items = realloc(l->items, (l->length + 1) * sizeof(ListItem));
    }

    l->max_length++;
  }
}

void list_append(List* l, Employee* emp) {
  if (l->length >= l->max_length) {
    enlarge_list(l);
  }

  // we know datum is a pointer to the Employee struct
  Employee* row = emp_copy(emp);

  llast(l) = row;

  l->length++;
}