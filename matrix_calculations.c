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

void transform_coordinates(Atom atoms[], int atom_count, const std::vector<int>& atom_indices) {
    if (atom_indices.size() < 3) {
        fprintf(stderr, "Для определения плоскости необходимо минимум три атома.\n");
        return;
    }

    // Собираем координаты точек из указанных индексов
    std::vector<Vector> points;
    for (int index : atom_indices) {
        if (index < 0 || index >= atom_count) {
            fprintf(stderr, "Неверный индекс атома: %d.\n", index);
            return;
        }
        points.push_back({atoms[index].x, atoms[index].y, atoms[index].z});
    }

    // Находим нормаль плоскости с помощью PCA
    Vector plane_normal = fit_plane_pca(points);

    // Целевая нормаль для оси Z
    Vector target = {0, 0, 1};

    // Если нормаль плоскости противоположна целевой нормали
    if (dot_product(plane_normal, target) == -1.0) {
        Vector rotation_axis = {1, 0, 0}; // Вращение вокруг оси X
        double theta = M_PI;
        Matrix3 R = rotation_matrix_from_axis_angle(rotation_axis, theta);

        for (int i = 0; i < atom_count; i++) {
            Vector v = {atoms[i].x, atoms[i].y, atoms[i].z};
            v = apply_matrix(R, v);
            atoms[i].x = v.x;
            atoms[i].y = v.y;
            atoms[i].z = v.z;
        }
    } else {
        // Рассчитываем ось вращения и угол
        Vector rotation_axis = cross_product(plane_normal, target);
        rotation_axis = normalize(rotation_axis);
        double theta = acos(dot_product(plane_normal, target));

        Matrix3 R = rotation_matrix_from_axis_angle(rotation_axis, theta);

        for (int i = 0; i < atom_count; i++) {
            Vector v = {atoms[i].x, atoms[i].y, atoms[i].z};
            v = apply_matrix(R, v);
            atoms[i].x = v.x;
            atoms[i].y = v.y;
            atoms[i].z = v.z;
        }
    }

    // Считаем центр масс для заданных атомов
    Vector mass_center = calculate_mass_center(atoms, atom_indices);

    // Перемещаем координаты в новую базисную систему
    move_basis(atoms, atom_count, mass_center);
    print_atoms(atoms, atom_count);
}

Vector fit_plane_pca(const std::vector<Vector>& points) {
    // Переводим точки в формат Eigen
    Eigen::MatrixXd point_matrix(points.size(), 3);
    for (size_t i = 0; i < points.size(); ++i) {
        point_matrix(i, 0) = points[i].x;
        point_matrix(i, 1) = points[i].y;
        point_matrix(i, 2) = points[i].z;
    }

    // Находим центр точек (среднее значение)
    Eigen::RowVector3d center = point_matrix.colwise().mean();
    point_matrix.rowwise() -= center;

    // Применяем сингулярное разложение для нахождения главных компонент
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(point_matrix, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Vector3d normal = svd.matrixV().col(2); // нормаль будет третьей главной компонентой

    Vector plane_normal = {normal(0), normal(1), normal(2)};
    return plane_normal;
}

Vector calculate_mass_center(Atom atoms[], const std::vector<int>& atom_indices) {
    Vector mass_center = {0, 0, 0};
    for (int index : atom_indices) {
        mass_center.x += atoms[index].x;
        mass_center.y += atoms[index].y;
        mass_center.z += atoms[index].z;
    }
    mass_center.x /= atom_indices.size();
    mass_center.y /= atom_indices.size();
    mass_center.z /= atom_indices.size();
    return mass_center;
}

std::tuple<Vector, double> fit_plane(const std::vector<Vector>& points) {
    size_t n = points.size();
    if (n < 3) {
        throw std::invalid_argument("Для определения плоскости необходимо минимум три точки.");
    }

    // Средние значения координат
    double x_mean = 0, y_mean = 0, z_mean = 0;
    for (const auto& point : points) {
        x_mean += point.x;
        y_mean += point.y;
        z_mean += point.z;
    }
    x_mean /= n;
    y_mean /= n;
    z_mean /= n;

    // Матрица ковариации
    double xx = 0, xy = 0, xz = 0, yy = 0, yz = 0;
    for (const auto& point : points) {
        double dx = point.x - x_mean;
        double dy = point.y - y_mean;
        double dz = point.z - z_mean;

        xx += dx * dx;
        xy += dx * dy;
        xz += dx * dz;
        yy += dy * dy;
        yz += dy * dz;
    }

    // Находим собственный вектор с минимальным собственным значением
    Eigen::Matrix3d cov;
    cov << xx, xy, xz,
           xy, yy, yz,
           xz, yz, -(xx + yy);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(cov);
    Eigen::Vector3d normal = solver.eigenvectors().col(0);

    Vector plane_normal = {normal(0), normal(1), normal(2)};
    double d = -(plane_normal.x * x_mean + plane_normal.y * y_mean + plane_normal.z * z_mean);

    return {plane_normal, d};
}

void move_basis(Atom atoms[], int atom_count, Vector mass_center) {
    for (int i = 0; i < atom_count; i++) {
        atoms[i].x -= mass_center.x;
        atoms[i].y -= mass_center.y;
        atoms[i].z -= mass_center.z;
    }
    return;
}


