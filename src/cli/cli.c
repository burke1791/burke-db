#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "include/cli.h"

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
  } else if (strcmp(cmd, "select") == 0) {
    return CLI_SELECT;
  } else if (strcmp(cmd, "insert") == 0) {
    return CLI_INSERT;
  }

  return CLI_UNRECOGNIZED;
}

bool execute_cli_command(CLI_Command cmd) {
  switch (cmd) {
    case CLI_UNRECOGNIZED:
      printf("Unrecognized command. Please enter 'select', 'insert', or '.exit'\n");
      return true;
    case CLI_SELECT:
      printf("Select: not implemented yet\n");
      return true;
    case CLI_INSERT:
      printf("Insert: not implemented yet\n");
      return true;
    case CLI_EXIT:
      printf("Program shutting down...\n");
      return false;
    default:
      printf("Unrecognized CLI_COMMAND\n");
      return false;
  }
}