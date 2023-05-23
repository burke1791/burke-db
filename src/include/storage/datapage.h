#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PAGE_HEADER_LENGTH 8

#define PD_PAGE_NO_OFFSET 0
#define PD_LOWER_OFFSET 4
#define PD_UPPER_OFFSET 6

typedef char* DataPage;

typedef struct LinePointer {
  unsigned lp_off:15,   /* offset to the tuple from the start of the page */
      lp_flags:2,       /* state of the line pointer, currently unused */
      lp_len:15;        /* byte length of the tuple */
} LinePointer;

typedef struct DataPageHeader {
  /* Page Header Fields */
  uint32_t pd_page_no;
  uint16_t pd_lower;
  uint16_t pd_upper;

  LinePointer** pd_linep;

  /* remaining space is taken up by the user data */
} DataPageHeader;

DataPage allocate_page();
DataPage page_init(uint64_t pageNo);
void free_page(DataPage pg);

DataPage read_page_from_disk(char* tableName, uint64_t pageNo);
void write_page_to_disk(char* tableName, DataPage pg, uint64_t pageNo);

bool page_is_full(DataPage pg, int requiredSpace);

#endif /* PAGE_H */