#ifndef PARSE_OUTPUT_H
#define PARSE_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 256
#define MAX_ATOMS 500

typedef struct {
    char element[3]; // Тип атома (например, "C", "N", "H")
    double x;        // Координата X
    double y;        // Координата Y
    double z;        // Координата Z
} Atom;

int read_coordinates(char *filename, Atom atoms[], int max_atoms);
void print_atoms(Atom atoms[], int atom_count);
#endif