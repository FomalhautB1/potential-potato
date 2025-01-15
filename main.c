#include "parse_output.h"
#include "matrix_calculations.h"
#include "write_input.h"

int main() {
    int first_atom;
    int second_atom;
    int third_atom;

    Atom atoms[MAX_ATOMS];
    char *filename = "S-NHC.out";
    int atom_count = read_coordinates(filename, atoms, MAX_ATOMS);
    if (atom_count > 0) {
        print_atoms(atoms, atom_count);
        printf("Введите номера трех атомов в кольце через пробел\n");
        scanf("%d %d %d", &first_atom, &second_atom, &third_atom);

        printf("%d, %d, %d \n", first_atom, second_atom, third_atom);
        transform_coordinates(atoms, atom_count, first_atom, second_atom, third_atom);
    } else {
        printf("Координаты атомов не найдены.\n");
    }
    filename = "S-NHC.out";
    replaceLines(filename, atoms, atom_count);
    return 0;
}