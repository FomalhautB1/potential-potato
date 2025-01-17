#include "insert_dummy.h"

void insert_dummy_center(Atom atoms[], int *atom_count) {
    strcpy(atoms[*atom_count].element, "X");
    atoms[*atom_count].x = 0.0;
    atoms[*atom_count].y = 0.0;
    atoms[*atom_count].z = 0.0;
    (*atom_count) += 1;
    return;
}

void insert_dummy(Atom atoms[], int *atom_count, double x, double y, double z) {
    strcpy(atoms[*atom_count].element, "X");
    atoms[*atom_count].x = x;
    atoms[*atom_count].y = y;
    atoms[*atom_count].z = z;
    (*atom_count) += 1;
    return;
}