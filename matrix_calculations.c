#include "matrix_calculations.h"
#include <math.h>
#define M_PI 3.14159265358979323846

Vector subtract_vectors(Vector v1, Vector v2) {
    Vector res;
    res.x = v1.x - v2.x;
    res.y = v1.y - v2.y;
    res.z = v1.z - v2.z;
    return res;
}

double dot_product(Vector v1, Vector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector cross_product(Vector v1, Vector v2) {
    Vector res;
    res.x = v1.y * v2.z - v1.z * v2.y;
    res.y = v1.z * v2.x - v1.x * v2.z;
    res.z = v1.x * v2.y - v1.y * v2.x;
    return res;
}

Vector normalize(Vector v) {
    double len = sqrt(dot_product(v, v));
    if (len == 0) {
        return (Vector){0, 0, 0};
    }
    return (Vector){v.x / len, v.y / len, v.z / len};
}

Matrix3 rotation_matrix_from_axis_angle(Vector axis, double theta) {
    Matrix3 R;
    double ct = cos(theta);
    double st = sin(theta);
    double vt = 1 - ct;
    axis = normalize(axis);
    double x = axis.x;
    double y = axis.y;
    double z = axis.z;
    R.m[0][0] = x * x * vt + ct;
    R.m[0][1] = x * y * vt - z * st;
    R.m[0][2] = x * z * vt + y * st;
    R.m[1][0] = y * x * vt + z * st;
    R.m[1][1] = y * y * vt + ct;
    R.m[1][2] = y * z * vt - x * st;
    R.m[2][0] = z * x * vt - y * st;
    R.m[2][1] = z * y * vt + x * st;
    R.m[2][2] = z * z * vt + ct;
    return R;
}

Vector apply_matrix(Matrix3 R, Vector v) {
    Vector res;
    res.x = R.m[0][0] * v.x + R.m[0][1] * v.y + R.m[0][2] * v.z;
    res.y = R.m[1][0] * v.x + R.m[1][1] * v.y + R.m[1][2] * v.z;
    res.z = R.m[2][0] * v.x + R.m[2][1] * v.y + R.m[2][2] * v.z;
    return res;
}

void transform_coordinates(Atom atoms[], int atom_count, int atom1_index, int atom2_index, int atom3_index) {

    if (atom1_index < 0 || atom1_index >= atom_count ||
        atom2_index < 0 || atom2_index >= atom_count ||
        atom3_index < 0 || atom3_index >= atom_count) {
        fprintf(stderr, "Invalid atom indices.\n");
        return;
    }

    Vector A = {atoms[atom1_index].x, atoms[atom1_index].y, atoms[atom1_index].z};
    Vector B = {atoms[atom2_index].x, atoms[atom2_index].y, atoms[atom2_index].z};
    Vector C = {atoms[atom3_index].x, atoms[atom3_index].y, atoms[atom3_index].z};

    Vector AB = subtract_vectors(B, A);
    Vector AC = subtract_vectors(C, A);

    Vector N = cross_product(AB, AC);
    N = normalize(N);

    Vector target = {0, 0, 1}; 

    if (dot_product(N, target) == -1.0) {

        Vector rotation_axis = {1, 0, 0}; 
        double theta = M_PI;
        Matrix3 R = rotation_matrix_from_axis_angle(rotation_axis, theta);



        for (int i = 0; i < atom_count; i++) {
            Vector v = {atoms[i].x, atoms[i].y, atoms[i].z};
            v = apply_matrix(R, v);
            atoms[i].x = v.x;
            atoms[i].y = v.y;
            atoms[i].z = v.z;
        }
        Vector Z = {atoms[atom1_index].x, atoms[atom1_index].y, atoms[atom1_index].z};
        for (int i = 0; i < atom_count; i++) {
            //atoms[i].x -= A.x;
            //atoms[i].y -= A.y;
            atoms[i].z -= Z.z;
        }

    } else {
        Vector rotation_axis = cross_product(N, target);
        rotation_axis = normalize(rotation_axis);

        double theta = acos(dot_product(N, target));

        Matrix3 R = rotation_matrix_from_axis_angle(rotation_axis, theta);



        for (int i = 0; i < atom_count; i++) {
            Vector v = {atoms[i].x, atoms[i].y, atoms[i].z};
            v = apply_matrix(R, v);
            atoms[i].x = v.x;
            atoms[i].y = v.y;
            atoms[i].z = v.z;
        }
        Vector Z = {atoms[atom1_index].x, atoms[atom1_index].y, atoms[atom1_index].z};
        for (int i = 0; i < atom_count; i++) {
            //atoms[i].x -= A.x;
            //atoms[i].y -= A.y;
            atoms[i].z -= Z.z;
        }
    
    }
    Vector mass_center = calculate_mass_center(atoms, atom1_index, atom2_index, atom3_index);
    move_basis(atoms, atom_count, mass_center);
    print_atoms(atoms,atom_count);
}
Vector calculate_mass_center(Atom atoms[], int atom1_index, int atom2_index, int atom3_index) {
    Vector mass_center = {0, 0, 0};
    mass_center.x = (atoms[atom1_index].x + atoms[atom2_index].x + atoms[atom3_index].x) / 3.0;
    mass_center.y = (atoms[atom1_index].y + atoms[atom2_index].y + atoms[atom3_index].y) / 3.0;
    mass_center.z = (atoms[atom1_index].z + atoms[atom2_index].z + atoms[atom3_index].z) / 3.0;
    return mass_center;
}

void move_basis(Atom atoms[], int atom_count, Vector mass_center) {
    for (int i = 0; i < atom_count; i++) {
        atoms[i].x -= mass_center.x;
        atoms[i].y -= mass_center.y;
        atoms[i].z -= mass_center.z;
    }
    return;
}


