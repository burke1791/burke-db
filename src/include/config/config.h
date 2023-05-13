#ifndef CONFIG_H
#define CONFIG_H

typedef enum ConfigParameter {
  CONF_DATA_DIR,
  CONF_PAGE_SIZE,
  CONF_UNRECOGNIZED
} ConfigParameter;

/**
 * Stores global configuration information for the DB server
*/
typedef struct {
  char* dataDir;
  int pageSize;
} Config;

bool set_global_config(Config* conf);

#endif /* PAGE_H */