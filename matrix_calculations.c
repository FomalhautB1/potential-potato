#include "matrix_calculations.h"
#include "parse_output.h"

void cross_product(const double a[3], const double b[3], double result[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

// Нормализация вектора
void normalize(double v[3]) {
    double magnitude = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (magnitude > 0) {
        v[0] /= magnitude;
        v[1] /= magnitude;
        v[2] /= magnitude;
    }
}

double dot_product(const double a[3], const double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void rotate_atoms_to_plane(Atom atoms[], int atom_count, int idx1, int idx2, int idx3) {
    double p1[3] = {atoms[idx1].x, atoms[idx1].y, atoms[idx1].z};
    double p2[3] = {atoms[idx2].x, atoms[idx2].y, atoms[idx2].z};
    double p4[3] = {atoms[idx3].x, atoms[idx3].y, atoms[idx3].z};

    // Векторы, лежащие в плоскости
    double v1[3], v2[3];
    for (int i = 0; i < 3; i++) {
        v1[i] = p2[i] - p1[i];
        v2[i] = p4[i] - p1[i];
    }

    // Нормаль к плоскости
    double normal[3];
    cross_product(v1, v2, normal);
    normalize(normal);

    // Новые оси
    double x_axis[3];
    for (int i = 0; i < 3; i++) {
        x_axis[i] = v1[i];
    }
    normalize(x_axis);

    double y_axis[3];
    cross_product(normal, x_axis, y_axis);
    normalize(y_axis);

    // Матрица поворота
    double rotation_matrix[3][3] = {
        {x_axis[0], y_axis[0], normal[0]},
        {x_axis[1], y_axis[1], normal[1]},
        {x_axis[2], y_axis[2], normal[2]}
    };

    // Поворот всех атомов
    for (int i = 0; i < atom_count; i++) {
        double atom[3] = {atoms[i].x, atoms[i].y, atoms[i].z};
        double rotated[3] = {0.0, 0.0, 0.0};

        // Умножение матрицы поворота на координаты атома
        for (int j = 0; j < 3; j++) {
            rotated[0] += rotation_matrix[0][j] * atom[j];
            rotated[1] += rotation_matrix[1][j] * atom[j];
            rotated[2] += rotation_matrix[2][j] * atom[j];
        }

        // Обновление координат атома после поворота
        atoms[i].x = rotated[0];
        atoms[i].y = rotated[1];
        atoms[i].z = rotated[2];
    }

    // Смещение плоскости на z = 0
    double z_shift = atoms[idx1].z;
    // Смещаем все атомы так, чтобы плоскость была на z = 0
    for (int i = 0; i < atom_count; i++) {
        atoms[i].z -= z_shift;
    }
    print_atoms(atoms, atom_count);
}