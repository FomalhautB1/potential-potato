#ifndef MATRIX_CALCULATIONS_H
#define MATRIX_CALCULATIONS_H


#include <math.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <Eigen/Dense>

#include "parse_output.h"

#ifdef __cplusplus
extern "C" {
#endif

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
void transform_coordinates(Atom atoms[], int atom_count, const std::vector<int>& atom_indices);
Vector fit_plane_pca(const std::vector<Vector>& points);
Vector calculate_mass_center(Atom atoms[], const std::vector<int>& atom_indices);
std::tuple<Vector, double> fit_plane(const std::vector<Vector>& points);
void move_basis(Atom atoms[], int atom_count, Vector mass_center);

#ifdef __cplusplus
}
#endif


#endif