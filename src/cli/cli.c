#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "include/cli.h"
#include "config/config.h"
#include "employee/emp_tuple.h"
#include "include/emp_list.h"
#include "storage/datapage.h"
#include "include/resultset_output.h"

extern Config* conf;

InputBuffer* new_input_buffer() {
  InputBuffer* buf = malloc(sizeof(InputBuffer));

  buf->buffer = NULL;
  buf->buffer_length = 0;
  buf->input_length = 0;

  return buf;
}

void free_input_buffer(InputBuffer* buf) {
  if (buf->buffer != NULL) free(buf->buffer);
  free(buf);
}

void print_prompt() {
  printf("db > ");
}

bool read_input(InputBuffer* buf) {
  ssize_t bytes_read = getline(&(buf->buffer), &(buf->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input\n");
    return false;
  }

  // Ignore trailing newline character
  buf->input_length = bytes_read - 1;
  buf->buffer[bytes_read - 1] = 0;

  return true;
}

CLI_Command parse_command(char* cmd) {
  if (strcmp(cmd, ".exit") == 0) {
    return CLI_EXIT;
  } else if (strncmp(cmd, "select", 6) == 0) {
    return CLI_SELECT;
  } else if (strncmp(cmd, "insert", 6) == 0) {
    return CLI_INSERT;
  }

  return CLI_UNRECOGNIZED;
}

static void execute_select() {
  bool hasData = true;
  uint64_t pageNo = 0;
  int tupId = 0;
  int numTuples;
  List* res = new_list();

  LinePointer* lp = malloc(sizeof(LinePointer));
  EmpTuple tup = malloc(EMP_TUPLE_SIZE);
  Employee* emp = malloc(sizeof(Employee));
  emp->name = malloc(20);

  while (true) {
    DataPage pg = read_page_from_disk(conf->tableName, pageNo);
    if (pg == NULL) {
      break;
    }

    memset(lp, 0, 4);
    memset(tup, 0, EMP_TUPLE_SIZE);

    if (((DataPageHeader*)pg)->pd_upper != 0) {
      tupId = 0;
      numTuples = (((DataPageHeader*)pg)->pd_lower - PAGE_HEADER_LENGTH) / 4;
      while (tupId < numTuples) {
        memcpy(lp, pg + PAGE_HEADER_LENGTH + (tupId * sizeof(LinePointer)), sizeof(LinePointer));
        printf("lp_len: %d\n", lp->lp_len);
        memcpy(tup, pg + lp->lp_off, lp->lp_len);

        memcpy(&(emp->empId), tup + EMP_ID_OFFSET, 8);
        memcpy(emp->name, tup + EMP_NAME_OFFSET, 20);

        list_append(res, emp);

        tupId++;
      }
    }

    pageNo++;
  }

  print_resultset(res);
  free_list(res);
  free(lp);
  free(tup);
  emp_free(emp);
}

static void execute_insert(char* val) {
  char* cmd = strtok(val, " ");

  int64_t empId = atol(strtok(NULL, " "));
  char* name = strtok(NULL, " ");

  EmpTuple tup = allocate_tuple();

  if (serialize_tuple(tup, empId, name) < 0) {
    return;
  }

  if (insert_tuple(conf->tableName, tup) < 0) {
    return;
  }

  printf("Tuple inserted\n");
}

bool execute_cli_command(CLI_Command cmd, char* val) {
  switch (cmd) {
    case CLI_UNRECOGNIZED:
      printf("Unrecognized command. Please enter 'select', 'insert', or '.exit'\n");
      return true;
    case CLI_SELECT:
      execute_select();
      return true;
    case CLI_INSERT:
      execute_insert(val);
      return true;
    case CLI_EXIT:
      printf("Program shutting down...\n");
      return false;
    default:
      printf("Unrecognized CLI_COMMAND\n");
      return false;
  }
}

void print_config() {
  printf("DATA_DIR: %s\n", conf->dataDir);
  printf("PAGE_SIZE: %d\n", conf->pageSize);
  printf("TABLE_NAME: %s\n", conf->tableName);
}