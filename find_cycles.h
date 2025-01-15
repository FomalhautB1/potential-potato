#ifndef FIND_CYCLES_H
#define FIND_CYCLES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "parse_output.h"

double calculate_distance(Atom a1, Atom a2);
void build_adjacency_matrix(Atom *atoms, int num_atoms, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS]);
void find_cycles_dfs(int node, int parent, int *visited, int *stack, int adjacency_matrix[MAX_ATOMS][MAX_ATOMS], int num_atoms, int *cycle_found, int *cycle_nodes, int *cycle_size);
int find_cycles(Atom *atoms, int num_atoms, int *cycle_nodes, int *cycle_size);

#endif