#include "find_cycles.h"

//#define MAX_ATOMS 100  // Максимальное количество атомов
#define BOND_THRESHOLD 1.6 // Порог расстояния для связи (в ангстремах) 1.6

double calculate_distance(Atom a1, Atom a2) {
    return sqrt(pow(a1.x - a2.x, 2) + pow(a1.y - a2.y, 2) + pow(a1.z - a2.z, 2));
}

// Построение матрицы смежности для графа молекулы
void build_adjacency_matrix(Atom *atoms, int num_atoms, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS]) {
    for (int i = 0; i < num_atoms; i++) {
        for (int j = 0; j < num_atoms; j++) {
            if (i != j && calculate_distance(atoms[i], atoms[j]) <= BOND_THRESHOLD) {
                adjacency_matrix[i][j] = 1;
            } else {
                adjacency_matrix[i][j] = 0;
            }
        }
    }
}

// Нормализация порядка цикла для предотвращения дублирования
void normalize_cycle(int *cycle, int size) {
    int min_index = 0;
    for (int i = 1; i < size; i++) {
        if (cycle[i] < cycle[min_index]) {
            min_index = i;
        }
    }
    int normalized[MAX_ATOMS];
    for (int i = 0; i < size; i++) {
        normalized[i] = cycle[(min_index + i) % size];
    }
    memcpy(cycle, normalized, size * sizeof(int));
}

// Проверка уникальности цикла
int is_unique_cycle(int (*cycles)[MAX_ATOMS], int cycle_count, int *cycle, int size) {
    for (int i = 0; i < cycle_count; i++) {
        int match = 1;
        for (int j = 0; j < size; j++) {
            if (cycles[i][j] != cycle[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            return 0;
        }
    }
    return 1;
}

// Рекурсивный поиск циклов DFS
void find_cycles_dfs(int node, int start, int depth, int *visited, int *path, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS], int num_atoms, int target_cycle_size, int (*cycles)[MAX_ATOMS], int *cycle_count) {
    visited[node] = 1;
    path[depth] = node;

    for (int neighbor = 0; neighbor < num_atoms; neighbor++) {
        if (adjacency_matrix[node][neighbor]) {  // Если существует связь
            if (!visited[neighbor]) {
                // Продолжаем DFS
                find_cycles_dfs(neighbor, start, depth + 1, visited, path, adjacency_matrix, num_atoms, target_cycle_size, cycles, cycle_count);
            } else if (neighbor == start && depth + 1 == target_cycle_size) {
                // Найден цикл нужной длины
                int temp_cycle[MAX_ATOMS];
                for (int i = 0; i < target_cycle_size; i++) {
                    temp_cycle[i] = path[i];
                }
                normalize_cycle(temp_cycle, target_cycle_size);
                if (is_unique_cycle(cycles, *cycle_count, temp_cycle, target_cycle_size)) {
                    memcpy(cycles[*cycle_count], temp_cycle, target_cycle_size * sizeof(int));
                    (*cycle_count)++;
                }
                return;
            }
        }
    }

    visited[node] = 0;  // Возвращаем состояние узла
}

// Основная функция для поиска всех циклов заданного размера
int find_all_cycles(Atom *atoms, int num_atoms, int target_cycle_size, int (*cycles)[MAX_ATOMS]) {
    int adjacency_matrix[MAX_ATOMS][MAX_ATOMS];
    build_adjacency_matrix(atoms, num_atoms, adjacency_matrix);

    int visited[MAX_ATOMS] = {0};
    int path[MAX_ATOMS];
    int cycle_count = 0;

    for (int i = 0; i < num_atoms; i++) {
        find_cycles_dfs(i, i, 0, visited, path, adjacency_matrix, num_atoms, target_cycle_size, cycles, &cycle_count);
    }

    return cycle_count;
}