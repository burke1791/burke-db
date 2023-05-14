#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config/config.h"

static ConfigParameter parse_config_param(char* p) {
  if (strcmp(p, "DATA_DIR") == 0) return CONF_DATA_DIR;
  if (strcmp(p, "PAGE_SIZE") == 0) return CONF_PAGE_SIZE;

  return CONF_UNRECOGNIZED;
}

static void set_config_value(Config* conf, ConfigParameter p, char* v) {
  switch (p) {
    case CONF_DATA_DIR:
      v[strcspn(v, "\r\n")] = 0; // remove the trailing newline character if it exists
      conf->dataDir = strdup(v);
      break;
    case CONF_PAGE_SIZE:
      conf->pageSize = atoi(v);
      break;
  }
}

/**
 * Reads config.conf at the root of this repository (assuming we start the program
 * from the repo root) and sets the global config values specified in the file
*/
static FILE* read_config_file() {
  FILE* fp = fopen("config.conf", "r");
  
  return fp;
}

static void close_config_file(FILE* fp) {
  fclose(fp);
}

bool set_global_config(Config* conf) {
  FILE* fp = read_config_file();
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  ConfigParameter p;

  if (fp == NULL) {
    printf("Unable to read config file: config.conf\n");
    return false;
  }

  // loops through each line in the config file and sets
  // values as it encounters them
  while ((read = getline(&line, &len, fp)) != -1) {
    // skip comment lines or empty lines
    if (strncmp(line, "#", 1) == 0 || read <= 1) continue;

    char* param = strtok(line, "=");
    char* value = strtok(NULL, "=");

    p = parse_config_param(param);

    if (p == CONF_UNRECOGNIZED) continue;

    set_config_value(conf, p, value);
  }

  if (line) free(line);

  close_config_file(fp);

  return true;
}

void free_config(Config* conf) {
  if (conf->dataDir != NULL) free(conf->dataDir);
  free(conf);
}