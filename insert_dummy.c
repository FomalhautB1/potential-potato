#include "insert_dummy.h"

void insert_dummy_center(Atom atoms[], int *atom_count) {
    strcpy(atoms[*atom_count].element, "X");
    atoms[*atom_count].x = 0.0;
    atoms[*atom_count].y = 0.0;
    atoms[*atom_count].z = 0.0;
    (*atom_count) += 1;
    return;
}