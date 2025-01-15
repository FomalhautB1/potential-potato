#ifndef WRITE_INPUT_H
#define WRITE_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include "parse_output.h"

void replaceLines(char *filename, Atom atoms[], int atom_count);

void generate_xyz(char* filename, Atom atoms[], int atom_count);

#ifdef __cplusplus
}
#endif

#endif