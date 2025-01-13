#include "parse_output.h"

int main() {
    Atom atoms[MAX_ATOMS];
    const char *filename = "S-NHC.out";

    int atom_count = read_coordinates(filename, atoms, MAX_ATOMS);
    if (atom_count > 0) {
        print_atoms(atoms, atom_count);
    } else {
        printf("Координаты атомов не найдены.\n");
    }

    return 0;
}