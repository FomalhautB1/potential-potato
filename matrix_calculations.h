#ifndef MATRIX_CALCULATIONS_H
#define MATRIX_CALCULATIONS_H

#include <math.h>
#include <string.h>
#include "parse_output.h"
#include <math.h>

typedef struct {
    double x, y, z;
} Vector;

typedef struct {
    double m[3][3];
} Matrix3;


Vector subtract_vectors(Vector v1, Vector v2);
double dot_product(Vector v1, Vector v2);
Vector cross_product(Vector v1, Vector v2);
Vector normalize(Vector v);
Matrix3 rotation_matrix_from_axis_angle(Vector axis, double theta);
Vector apply_matrix(Matrix3 R, Vector v);
void transform_coordinates(Atom atoms[], int atom_count, int atom1_index, int atom2_index, int atom3_index);
#endif