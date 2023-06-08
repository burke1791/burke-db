#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/resultset.h"

#define liptr(li)  ((li)->ptr_value)
#define llast(l)  liptr(list_last_cell(l))

#define list_make_ptr_cell(v) ((ListCell) {.ptr_value = (v)})

static inline ListItem* list_last_cell(const List *list) {
	return &list->items[list->length];
}

List* new_list(ListType type) {
  List* l = malloc(sizeof(List));

  l->type = type;
  l->length = 0;
  l->max_length = 0;
  l->items = NULL;

  return l;
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

void list_append(List* l, void* l_item) {
  if (l->length >= l->max_length) {
    enlarge_list(l);
  }

  llast(l) = l_item;

  l->length++;
}

void free_cell(ResultCell* c) {
  if (c != NULL) {
    free(c->col);
    free(c->dat);
  }
}

void free_list(List* l) {
  switch (l->type) {
    case L_Row:
      for (int i = 0; i < l->length; i++) {
        free_list((List*)l->items[i].ptr_value);
        free(l->items[i].ptr_value);
      }
      break;
    case L_Col:
      for (int i = 0; i < l->length; i++) {
        free_cell((ResultCell*)l->items[i].ptr_value);
        free(l->items[i].ptr_value);
      }
      break;
  }

  free(l);
}