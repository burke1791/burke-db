#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "include/cli.h"
#include "config/config.h"

Config* conf;

int main(int argc, char* argv[]) {
  conf = malloc(sizeof(Config));
  // tells the loop when it's time to quit
  bool cliState = true;
  // allocate memory for storing user input
  InputBuffer* buf = new_input_buffer();

  if (!set_global_config(conf)) {
    return EXIT_FAILURE;
  }

  print_config();

  // main control loop
  while (cliState) {
    print_prompt();
    cliState = read_input(buf);

    // we want to exit early if there was an issue reading the user input
    if (!cliState) break;

    CLI_Command cmd = parse_command(buf->buffer);
    cliState = execute_cli_command(cmd);
  }

  free_input_buffer(buf);
  free_config(conf);
  return EXIT_SUCCESS;
}