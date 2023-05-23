#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "config/config.h"
#include "employee/emp_tuple.h"
#include "storage/datapage.h"

extern Config* conf;

EmpTuple allocate_tuple() {
  EmpTuple tup = malloc(EMP_TUPLE_SIZE);
  memset(tup, 0, EMP_TUPLE_SIZE);
  return tup;
}

int serialize_tuple(EmpTuple tup, int64_t empId, char* name) {
  if (strlen(name) > 20) {
    printf("name: '%s' exceeds allowable length\n", name);
    return -1;
  }
  
  /**
   * set t_hoff to the header size of this tuple.
  */
  ((EmpTupleHeader*)tup)->t_hoff = 2;

  // if (strcmp(name, "NULL") == 0) {
  //   // ((EmpTupleHeader*)tup)->t_null_bitmap[1] = 1;
  // }
  

  /* set our column values in the user data space of the tuple */
  memcpy(tup + EMP_ID_OFFSET, &empId, 8);
  memcpy(tup + EMP_NAME_OFFSET, name, strlen(name));

  return EMP_TUPLE_SIZE;
}

int insert_tuple(char* tableName, EmpTuple tup) {
  DataPage pg = read_page_from_disk(conf->tableName, 0);

  if (pg == NULL) {
    printf("Unable to insert tuple\n");
    return -1;
  }

  if (page_is_full(pg, EMP_TUPLE_SIZE + 4)) {
    printf("Page is full. Need to implement adding pages\n");
    return -1;
  }

  // copy tuple data to the correct spot on the page
  uint16_t tupOffset;

  // an empty page has pd_upper = 0
  if (((DataPageHeader*)pg)->pd_upper == 0) {
    tupOffset = conf->pageSize - EMP_TUPLE_SIZE;
  } else {
    tupOffset = ((DataPageHeader*)pg)->pd_upper - EMP_TUPLE_SIZE;
  }

  memcpy(pg + tupOffset, tup, EMP_TUPLE_SIZE);

  // update pd_upper
  ((DataPageHeader*)pg)->pd_upper = tupOffset;

  LinePointer* lp = malloc(4);
  memset(lp, 0, 4);
  lp->lp_off = tupOffset;
  lp->lp_len = EMP_TUPLE_SIZE;

  // copy line pointer data to the end of the line pointer array
  int lpOffset = ((DataPageHeader*)pg)->pd_lower;
  memcpy(pg + lpOffset, lp, 4);

  // update pd_lower
  ((DataPageHeader*)pg)->pd_lower = lpOffset + 4;

  write_page_to_disk(conf->tableName, pg, ((DataPageHeader*)pg)->pd_page_no);

  free(tup);
  free_page(pg);

  return 1;
}

Employee* emp_copy(Employee* emp) {
  Employee* empCp = malloc(sizeof(Employee));

  empCp->empId = emp->empId;
  empCp->name = strdup(emp->name);

  return empCp;
}

void emp_free(Employee* emp) {
  if (emp->name != NULL) free(emp->name);
  free(emp);
}