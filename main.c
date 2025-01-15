#include "parse_output.h"
#include "matrix_calculations.h"
#include "write_input.h"
#include "find_cycles.h"
#include "insert_dummy.h"

int main(int argc, char *argv[]) {
    int first_atom;
    int second_atom;
    int third_atom;

    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    Atom atoms[MAX_ATOMS];
    int atom_count = read_coordinates(filename, atoms, MAX_ATOMS);
    if (atom_count > 0) {
            // начало анализа цикла, работает нестабильно
            int cycle_nodes[MAX_ATOMS];
            int cycle_size;

            if (find_cycles(atoms, atom_count, cycle_nodes, &cycle_size)) {
                printf("Cycles found in the molecule.\n");
                printf("Atoms in the cycle: ");
                for (int i = 0; i < cycle_size; i++) {
                    printf("%d ", cycle_nodes[i]);
                }
                printf("\n");
            } else {
                printf("No cycles found in the molecule.\n");
            }
            //  конец анализа цикла, работает нестабильно
        printf("Введите номера трех атомов в кольце через пробел\n");
        scanf("%d %d %d", &first_atom, &second_atom, &third_atom);

        printf("%d, %d, %d \n", first_atom, second_atom, third_atom);
        transform_coordinates(atoms, atom_count, first_atom, second_atom, third_atom);
        insert_dummy_center(atoms, &atom_count);
    } else {
        printf("Координаты атомов не найдены.\n");
    }
    replaceLines(filename, atoms, atom_count);
    return 0;
}