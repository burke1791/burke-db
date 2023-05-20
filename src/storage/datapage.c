#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

#include "config/config.h"
#include "storage/datapage.h"

extern Config* conf;

static char* get_full_filename(char* tableName) {
  int dirLen = strlen(conf->dataDir);
  int tableNameLen = strlen(tableName);

  char* filename = malloc(dirLen + tableNameLen + 5);

  strcpy(filename, conf->dataDir);
  strcat(filename, tableName);
  strcat(filename, ".dbd");

  return filename;
}

/**
 * Allocates memory for an empty data page and
 * zeros its contents
*/
DataPage* allocate_page() {
  DataPage* pg = malloc(conf->pageSize);
  memset(pg, 0, conf->pageSize);
  return pg;
}

void free_page(DataPage* pg) {
  free(pg);
}

/**
 * Allocates a new page and sets the page_no header to
 * the provided value. All other header fields are set to
 * values representative of a blank page.
*/
DataPage* page_init(uint64_t pageNo) {
  DataPage* pg = allocate_page();

  pg->pd_page_no = pageNo;

  // initialize pd_lower and pd_upper
  pg->pd_lower = PAGE_HEADER_LENGTH;
  pg->pd_upper = 0;

  return pg;
}

static int open_file(char* filename) {
  return open(filename,
                O_RDWR |    // Read/Write mode
                  O_CREAT,  // Create file if it doesn't exist
                S_IWUSR |   // User write permission
                  S_IRUSR   // User read permission
  );
}

static void close_file(int fd) {
  close(fd);
}

DataPage* read_page_from_disk(char* tableName, uint64_t pageNo) {
  char* filename = get_full_filename(tableName);

  int fd = open_file(filename);

  if (fd < 0) {
    printf("Unable to open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(fd, pageNo * conf->pageSize, SEEK_SET);

  DataPage* pg = page_init(pageNo);
  ssize_t bytes_read = read(fd, pg, conf->pageSize);

  if (bytes_read == -1) {
    printf("Error reading page from disk\n");
    free(filename);
    free_page(pg);
    return NULL;
  } else if (bytes_read != conf->pageSize) {
    printf("Incorrect number of bytes read from disk\n");
    free(filename);
    free_page(pg);
    return NULL;
  }

  free(filename);
  close_file(fd);
  return pg;
}

void write_page_to_disk(char* tableName, DataPage* pg, uint64_t pageNo) {
  char* filename = get_full_filename(tableName);

  int fd = open_file(filename);

  if (fd < 0) {
    printf("Unable to open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(fd, pageNo * conf->pageSize, SEEK_SET);

  if (offset == -1) {
    printf("Error getting file offset\n");
    free(filename);
    close_file(fd);
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = write(fd, pg, conf->pageSize);
  printf("Bytes written: %ld\n", bytes_written);

  free(filename);
  close_file(fd);
}

bool page_is_full(DataPage* pg, int requiredSpace) {
  int availableSpace;

  // when pd_upper = 0, the page is empty
  if (pg->pd_upper == 0) {
    availableSpace = conf->pageSize - PAGE_HEADER_LENGTH;
  } else {
    availableSpace = pg->pd_upper - pg->pd_lower;
  }

  if (availableSpace >= requiredSpace) return false;
  return true;
}