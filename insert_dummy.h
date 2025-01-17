#ifndef INSERT_DUMMY_H
#define INSERT_DUMMY_H

#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include <string.h>
#include "parse_output.h"

void insert_dummy_center(Atom atoms[], int *atom_count);
void insert_dummy(Atom atoms[], int *atom_count, double x, double y, double z);

#ifdef __cplusplus
}
#endif

#endif