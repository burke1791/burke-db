#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PAGE_HEADER_LENGTH 24

#define PD_PAGE_NO_OFFSET 0
#define PD_CHECKSUM_OFFSET 8
#define PD_FLAGS_OFFSET 10
#define PD_LOWER_OFFSET 12
#define PD_UPPER_OFFSET 14
#define PD_SPECIAL_OFFSET 16
#define PD_PAGESIZE_VERSION_OFFSET 18
#define PD_PRUNE_XID_OFFSET 20

typedef struct LinePointer {
  unsigned lp_off:15,   /* offset to the tuple from the start of the page */
      lp_flags:2,       /* state of the line pointer, currently unused */
      lp_len:15;        /* byte length of the tuple */
} LinePointer;

typedef struct DataPage {
  /* Page Header Fields */
  uint64_t pd_page_no;
  uint16_t pd_checksum;
  uint16_t pd_flags;
  uint32_t pd_lower;
  uint32_t pd_upper;
  uint16_t pd_special;
  uint16_t pd_pagesize_version;
  uint32_t pd_prune_xid;

  LinePointer** pd_linep;

  /* remaining space is taken up by the user data */
} DataPage;

DataPage* allocate_page();
DataPage* page_init(uint64_t pageNo);
void free_page(DataPage* pg);

DataPage* read_page_from_disk(char* tableName, uint64_t pageNo);
void write_page_to_disk(char* tableName, DataPage* pg, uint64_t pageNo);

bool page_is_full(DataPage* pg, int requiredSpace);

#endif /* PAGE_H */