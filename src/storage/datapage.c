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
DataPage allocate_page() {
  DataPage pg = malloc(conf->pageSize);
  memset(pg, 0, conf->pageSize);
  return pg;
}

void free_page(DataPage pg) {
  free(pg);
}

/**
 * Allocates a new page and sets the page_no header to
 * the provided value. All other header fields are set to
 * values representative of a blank page.
*/
DataPage page_init(uint64_t pageNo) {
  DataPage pg = allocate_page();

  ((DataPageHeader*)pg)->pd_page_no = pageNo;

  // initialize pd_lower and pd_upper
  ((DataPageHeader*)pg)->pd_lower = PAGE_HEADER_LENGTH;
  ((DataPageHeader*)pg)->pd_upper = 0;

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

DataPage read_page_from_disk(char* tableName, uint64_t pageNo) {
  char* filename = get_full_filename(tableName);

  int fd = open_file(filename);

  if (fd < 0) {
    printf("Unable to open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(fd, pageNo * conf->pageSize, SEEK_SET);

  DataPage pg = page_init(pageNo);
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

void write_page_to_disk(char* tableName, DataPage pg, uint64_t pageNo) {
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

bool page_is_full(DataPage pg, int requiredSpace) {
  int availableSpace;

  // when pd_upper = 0, the page is empty
  if (((DataPageHeader*)pg)->pd_upper == 0) {
    availableSpace = conf->pageSize - PAGE_HEADER_LENGTH;
  } else {
    availableSpace = ((DataPageHeader*)pg)->pd_upper - ((DataPageHeader*)pg)->pd_lower;
  }

  if (availableSpace >= requiredSpace) return false;
  return true;
}

void insert_tuple(TupleDescriptor* td, Datum* values, bool* isnull) {
  int16_t dataLen = compute_tuple_size(td, values, isnull);
  Tuple tup = malloc(dataLen);
  memset(tup, 0, dataLen);

  ((TupleHeader*)tup)->t_hoff = 2;
  int16_t offset = ((TupleHeader*)tup)->t_hoff;

  fill_tuple(td, tup + offset, values, isnull, ((TupleHeader*)tup)->t_null_bitmap);

  DataPage pg = read_page_from_disk(conf->tableName, 0);

  if (pg == NULL) {
    printf("Unable to insert tuple\n");
    return;
  }

  if (page_is_full(pg, dataLen + 4)) {
    printf("Page is full. Need to implement adding pages\n");
    return;
  }

  // copy tuple data to the correct spot on the page
  uint16_t tupOffset;

  // an empty page has pd_upper = 0
  if (((DataPageHeader*)pg)->pd_upper == 0) {
    tupOffset = conf->pageSize - dataLen;
  } else {
    tupOffset = ((DataPageHeader*)pg)->pd_upper - dataLen;
  }

  memcpy(pg + tupOffset, tup, dataLen);

  // update pd_upper
  ((DataPageHeader*)pg)->pd_upper = tupOffset;

  LinePointer* lp = malloc(4);
  memset(lp, 0, 4);
  lp->lp_off = tupOffset;
  lp->lp_len = dataLen;

  // copy line pointer data to the end of the line pointer array
  int lpOffset = ((DataPageHeader*)pg)->pd_lower;
  memcpy(pg + lpOffset, lp, 4);

  // update pd_lower
  ((DataPageHeader*)pg)->pd_lower = lpOffset + 4;

  write_page_to_disk(conf->tableName, pg, ((DataPageHeader*)pg)->pd_page_no);

  free(tup);
  free_page(pg);
}