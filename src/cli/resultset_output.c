#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/resultset_output.h"

#define EMP_ID_WIDTH 13
#define EMP_NAME_WIDTH 20

static void print_cell_with_padding(char* cell, int padLen, bool isRightAligned) {
  if (padLen == 0) {
    printf("%s|", cell);
  } else if (isRightAligned) {
    printf("%*s%s|", padLen, " ", cell);
  } else {
    printf("%s%*s|", cell, padLen, " ");
  }
}

static void print_column_headers() {
  printf("|");
  print_cell_with_padding("employee_id", EMP_ID_WIDTH - 11, false);
  print_cell_with_padding("name", EMP_NAME_WIDTH - 4, false);
  printf("\n");

  int totalWidth = 1 + EMP_ID_WIDTH + 1 + EMP_NAME_WIDTH + 1;
  for (int k = 0; k < totalWidth; k++) {
    printf("-");
  }
  printf("\n");
}

static int num_digits(int64_t num) {
  if (num < 0) return num_digits(-num);
  if (num < 10) return 1;
  return 1 + num_digits(num / 10);
}

static void print_empId(int64_t empId) {
  int numDigits = num_digits(empId);

  char* cell = malloc(numDigits + 1);
  memset(cell, 0, numDigits);
  sprintf(cell, "%ld", empId);
  cell[numDigits] = '\0';

  print_cell_with_padding(cell, EMP_ID_WIDTH - numDigits, true);
}

static void print_name(char* name) {
  print_cell_with_padding(name, EMP_NAME_WIDTH - strlen(name), false);
}

void print_resultset(List* res) {
  printf("*** Rows: %d\n", res->length);
  printf("--------\n");

  print_column_headers();

  for (int i = 0; i < res->length; i++) {
    Employee* emp = ((Employee*)(res->items[i].ptr_value));

    printf("|");
    print_empId(emp->empId);
    print_name(emp->name);
    printf("\n");
  }

  printf("\n");
}