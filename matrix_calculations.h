#ifndef MATRIX_CALCULATIONS_H
#define MATRIX_CALCULATIONS_H

#include <math.h>
#include <string.h>
#include "parse_output.h"


void cross_product(const double a[3], const double b[3], double result[3]);
void normalize(double v[3]);
double dot_product(const double a[3], const double b[3]);
void rotate_atoms_to_plane(Atom atoms[], int num_atoms, int idx1, int idx2, int idx4);
#endif