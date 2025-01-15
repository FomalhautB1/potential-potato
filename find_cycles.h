#ifndef FIND_CYCLES_H
#define FIND_CYCLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "parse_output.h"

double calculate_distance(Atom a1, Atom a2);
void build_adjacency_matrix(Atom *atoms, int num_atoms, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS]);
void find_cycles_dfs(int node, int start, int depth, int *visited, int *path, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS], int num_atoms, int target_cycle_size, int (*cycles)[MAX_ATOMS], int *cycle_count);
int find_all_cycles(Atom *atoms, int num_atoms, int target_cycle_size, int (*cycles)[MAX_ATOMS]);

#ifdef __cplusplus
}
#endif

#endif