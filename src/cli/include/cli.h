#ifndef CLI_H
#define CLI_H

#include <unistd.h>

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum CLI_Command {
  CLI_EXIT,
  CLI_SELECT,
  CLI_INSERT,
  CLI_UNRECOGNIZED
} CLI_Command;

InputBuffer* new_input_buffer();
void free_input_buffer(InputBuffer* buf);

void print_prompt();
bool read_input(InputBuffer* buf);

CLI_Command parse_command(char* cmd);
bool execute_cli_command(CLI_Command cmd);

void print_config();

#endif /* CLI_H */