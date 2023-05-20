#ifndef EMP_TUPLE_H
#define EMP_TUPLE_H

#include <stdint.h>

#define EMP_TUPLE_SIZE 51  /* header (23 bytes) + BigInt (8 bytes) + Char(20) (20 bytes) */
#define EMP_ID_OFFSET 23
#define EMP_NAME_OFFSET 31

typedef struct EmpTuple {
  int32_t t_xmin;         /* currently unused - int instead of uint
                             because this value can be negative */
  int32_t t_xmax;         /* currently unused - int instead of uint
                             because this value can be negative */
  uint32_t t_cid;         /* currently unused */
  uint32_t t_ctid;        /* currently unused */
  uint16_t t_infomask2;   /* currently unused */
  uint16_t t_infomask;    /* currently unused */
  uint8_t t_hoff;         /* byte length of the header, including the Null bitmap */

  /* ^ 23 byte tuple header ^ */

  /* Remaining space is used by the Null bitmap and user data */
} EmpTuple;

EmpTuple* allocate_tuple();
int serialize_tuple(EmpTuple* tup, int64_t empId, char* name);
int insert_tuple(char* tableName, EmpTuple* tup);

#endif /* EMP_TUPLE_H */