#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/resultset.h"

static int num_digits(int64_t num) {
  if (num < 0) return num_digits(-num);
  if (num < 10) return 1;
  return 1 + num_digits(num / 10);
}

static void print_cell_with_padding(char* cell, int cellWidth, bool isRightAligned) {
  int padLen = cellWidth - strlen(cell);

  if (padLen < 0) {
    printf("\npadLen: %d\ncellWidth: %d\n valWidth: %ld\n", padLen, cellWidth, strlen(cell));
    return;
  }

  if (padLen == 0) {
    printf("%s|", cell);
  } else if (isRightAligned) {
    printf("%*s%s|", padLen, " ", cell);
  } else {
    printf("%s%*s|", cell, padLen, " ");
  }
}

static void compute_column_widths(List* res, int* widths) {
  int maxLen;
  int numCols = ((List*)res->items[0].ptr_value)->length;

  for (int col = 0; col < numCols; col++) {
    maxLen = 0;
    for (int row = 0; row < res->length; row++) {
      ResultCell* c = (ResultCell*)((List*)res->items[row].ptr_value)->items[col].ptr_value;
      switch (c->col->c_type) {
        case DT_BIGINT:
          int numDigits = num_digits(datumGetInt64(c->dat));
          if (numDigits > maxLen) {
            maxLen = numDigits;
          }
          break;
        case DT_CHAR:
          int strLen = strlen(datumGetString(c->dat));
          if (strLen > maxLen) {
            maxLen = strLen;
          }
          break;
        default:
          printf("Unknown data type!\n");
      }
    }

    widths[col] = maxLen;
  }
}

static void print_column_headers(List* res, int* widths) {
  if (res->type != L_Row) {
    printf("Resultset list is not a list of rows!\n");
    return;
  }

  printf("|");

  int totalWidth = 1;
  List* firstRow = (List*)res->items[0].ptr_value;
  for (int i = 0; i < firstRow->length; i++) {
    print_cell_with_padding(((ResultCell*)firstRow->items[i].ptr_value)->col->c_name, widths[i], false);
    totalWidth += (widths[i] + 1);
  }

  printf("\n");

  for (int k = 0; k < totalWidth; k++) {
    printf("-");
  }
  printf("\n");
}

void print_resultset(List* res) {
  printf("*** Rows: %d\n", res->length);
  printf("--------\n");

  if (res->length == 0) {
    return;
  }

  int numCols = ((List*)res->items[0].ptr_value)->length;
  int* widths = malloc(sizeof(int) * numCols);
  compute_column_widths(res, widths);

  printf("test: rs 1\n");

  print_column_headers(res, widths);

  printf("test: rs 2\n");

  for (int row = 0; row < res->length; row++) {
    printf("|");
    for (int col = 0; col < numCols; col++) {
      ResultCell* c = (ResultCell*)((List*)res->items[row].ptr_value)->items[col].ptr_value;
      switch (c->col->c_type) {
        case DT_BIGINT:
          int64_t val = datumGetInt64(c->dat);
          int numDigits = num_digits(val);

          char* cell = malloc(numDigits + 1);
          sprintf(cell, "%ld", val);
          cell[numDigits] = '\0';

          print_cell_with_padding(cell, widths[col], true);
          break;
        case DT_CHAR:
          print_cell_with_padding(datumGetString(c->dat), widths[col], false);
          break;
        default:
          printf("Unknown data type!\n");
      }
    }
    printf("\n");
  }

  printf("test: rs 3\n");

  printf("(Rows: %d)\n", res->length);
}