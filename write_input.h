#ifndef WRITE_INPUT_H
#define WRITE_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <string.h>
#include "parse_output.h"

void replaceLines(char *filename, Atom atoms[], int atom_count);

char* MALLOC_generate_xyz(char* filename, Atom atoms[], int atom_count);

void generate_orca_input(const char *xyz_file_path, const char *output_file_path, Atom atoms[], int atom_count);


#ifdef __cplusplus
}
#endif

#endif