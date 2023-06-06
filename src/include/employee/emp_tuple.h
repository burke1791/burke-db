#ifndef EMP_TUPLE_H
#define EMP_TUPLE_H

#include <stdint.h>

/**
 * header (1 byte)
 * 2-col Null bitmap (1 byte)
 * BigInt (8-bytes)
 * Char(20) (20 bytes)
*/
#define EMP_TUPLE_SIZE 30

#define EMP_ID_OFFSET 2
#define EMP_NAME_OFFSET 10

// typedef char* EmpTuple;

// typedef struct EmpTupleHeader {
//   uint8_t t_hoff;         /* byte length of the header, including the Null bitmap */

//   /* ^ tuple header ^ */

//   uint8_t t_null_bitmap[]    /* null bitmap */

//   /* Remaining space is used by user data */
// } EmpTupleHeader;

/**
 * This struct is a convenience data type that gives us
 * an easy way to return data to the CLI
*/
typedef struct Employee {
  int64_t empId;
  char* name;
} Employee;

// EmpTuple allocate_tuple();
// int serialize_tuple(EmpTuple tup, int64_t empId, char* name);
// int insert_tuple(char* tableName, EmpTuple tup);

// EmpTuple select_tuple(int tupId);
// Employee* deserialize_tuple(EmpTuple tup);

Employee* emp_copy(Employee* emp);
void emp_free(Employee* emp);

#endif /* EMP_TUPLE_H */