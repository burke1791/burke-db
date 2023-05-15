#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PD_LSN_OFFSET 0
#define PD_CHECKSUM_OFFSET 8
#define PD_FLAGS_OFFSET 10
#define PD_LOWER_OFFSET 12
#define PD_UPPER_OFFSET 14
#define PD_SPECIAL_OFFSET 16
#define PD_PAGESIZE_VERSION_OFFSET 18
#define PD_PRUNE_XID_OFFSET 20

typedef struct DataPageHeader {
  uint64_t pd_lsn;
  uint16_t pd_checksum;
  uint16_t pd_flags;
  uint32_t pd_lower;
  uint32_t pd_upper;
  uint16_t pd_special;
  uint16_t pd_pagesize_version;
  uint32_t pd_prune_xid;
} DataPageHeader;

typedef struct LinePointer {
  unsigned lp_off:15,   /* offset to the tuple from the start of the page */
      lp_flags:2,       /* state of the line pointer, currently unused */
      lp_len:15;        /* byte length of the tuple */
} LinePointer;

typedef struct DataPage {
  DataPageHeader* pg_hdr;
  LinePointer** pd_linep;
  char* user_data;          /* remaining space is taken up by the user data */
} DataPage;


void page_init(DataPage* pg);

#endif /* PAGE_H */