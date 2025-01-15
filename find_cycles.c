#include "find_cycles.h"

//#define MAX_ATOMS 100  // Максимальное количество атомов
#define BOND_THRESHOLD 1.8 // Порог расстояния для связи (в ангстремах) 1.6

// Функция для вычисления расстояния между двумя атомами
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

// Поиск циклов с использованием DFS
void find_cycles_dfs(int node, int parent, int *visited, int *stack, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS], int num_atoms, int *cycle_found, int *cycle_nodes, int *cycle_size) {
    visited[node] = 1;
    stack[node] = 1;

    for (int neighbor = 0; neighbor < num_atoms; neighbor++) {
        if (adjacency_matrix[node][neighbor]) {  // Если существует связь
            if (!visited[neighbor]) {
                find_cycles_dfs(neighbor, node, visited, stack, adjacency_matrix, num_atoms, cycle_found, cycle_nodes, cycle_size);
            } else if (stack[neighbor] && neighbor != parent) {
                *cycle_found = 1;  // Найден цикл
                // Добавляем текущую вершину в список узлов цикла
                if (*cycle_size < MAX_ATOMS) {
                    cycle_nodes[*cycle_size] = node;
                    (*cycle_size)++;
                }
            }
        }
    }

    stack[node] = 0;
}

// Основная функция для поиска циклов
int find_cycles(Atom *atoms, int num_atoms, int *cycle_nodes, int *cycle_size) {
    int adjacency_matrix[MAX_ATOMS][MAX_ATOMS];
    build_adjacency_matrix(atoms, num_atoms, adjacency_matrix);

    int visited[MAX_ATOMS] = {0};
    int stack[MAX_ATOMS] = {0};
    int cycle_found = 0;

    *cycle_size = 0;  // Инициализация размера цикла

    for (int i = 0; i < num_atoms; i++) {
        if (!visited[i]) {
            find_cycles_dfs(i, -1, visited, stack, adjacency_matrix, num_atoms, &cycle_found, cycle_nodes, cycle_size);
        }
    }

    return cycle_found;
}
