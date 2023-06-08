#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "include/cli.h"
#include "config/config.h"
#include "storage/datapage.h"
#include "include/resultset.h"
#include "tuple/tuple.h"

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
  List* res = new_list(L_Row);

  TupleDescriptor* td = construct_tuple_desc(conf->tableName);

  LinePointer* lp = malloc(sizeof(LinePointer));

  while (true) {
    DataPage pg = read_page_from_disk(conf->tableName, pageNo);
    if (pg == NULL) {
      break;
    }

    if (((DataPageHeader*)pg)->pd_upper != 0) {
      tupId = 0;
      numTuples = (((DataPageHeader*)pg)->pd_lower - PAGE_HEADER_LENGTH) / 4;
      printf("test: 1\n");
      while (tupId < numTuples) {
        memcpy(lp, pg + PAGE_HEADER_LENGTH + (tupId * sizeof(LinePointer)), sizeof(LinePointer));
        Datum* values = get_tuple(td, pg, tupId);
        List* col = new_list(L_Col);

        for (int i = 0; i < td->natts; i++) {
          ResultCell* c = malloc(sizeof(ResultCell));
          c->col = td->cols[i];
          printf("test: 2 | col: %d\n", i);
          c->dat = values[i];

          printf("test: 3 | col: %d\n", i);

          list_append(col, c);
        }

        list_append(res, col);

        tupId++;
      }
    }

    pageNo++;
  }

  print_resultset(res);
  free_list(res);
  free(lp);
}

static void execute_insert(char* val) {
  char* cmd = strtok(val, " ");

  int64_t empId = atol(strtok(NULL, " "));
  char* name = strtok(NULL, " ");

  TupleDescriptor* td = construct_tuple_desc(conf->tableName);
  Datum* values = malloc(sizeof(Datum) * td->natts);
  bool* isnull = malloc(sizeof(bool) * td->natts);

  // construct values and isnull arrays
  for (int i = 0; i < td->natts; i++) {
    switch (td->cols[i]->c_type) {
      case DT_BIGINT:
        values[i] = int64GetDatum(empId); // hard-coded for now
        isnull[i] = false; // empId cannot be null
        break;
      case DT_CHAR:
        if (strcmp(name, "NULL") == 0) {
          values[i] = (Datum)NULL;
          isnull[i] = true;
        } else {
          char* strVal = malloc(21);
          strcpy(strVal, name);
          values[i] = charGetDatum(strVal);
          isnull[i] = false;
        }
        break;
      default:
        printf("Unknown data type!\n");
    }
  }

  insert_tuple(td, values, isnull);
  
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