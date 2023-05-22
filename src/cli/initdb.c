#include <stdio.h>

#include "config/config.h"
#include "storage/datapage.h"

extern Config* conf;

/**
 * Runs once on "server" startup. It first checks if a file exists
 * for the db table. If not, it will allocate a new page and write the
 * blank page to disk.
*/
void initdb() {
  char* tableName = conf->tableName;

  DataPage pg = read_page_from_disk(tableName, 0);

  if (pg == NULL) {
    printf("Initializing %s table\n", tableName);
    pg = page_init(0);
    write_page_to_disk(tableName, pg, 0);
  }

  free_page(pg);
}