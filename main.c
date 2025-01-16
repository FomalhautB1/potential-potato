#include "parse_output.h"
#include "matrix_calculations.h"
#include "write_input.h"
#include <string>
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
        char* new_filename = MALLOC_generate_xyz(filename, atoms, atom_count);
        std::string xyzfilename = new_filename;
        free(new_filename);
        count_cycles(xyzfilename);
        
            // // начало анализа цикла, работает нестабильно
            // int cycle_nodes[MAX_ATOMS];
            // int cycle_size;

            // int target_cycle_size = 6; 

            // if (find_all_cycles(atoms, atom_count, cycle_nodes, &cycle_size)) {
            //     printf("Cycles found in the molecule.\n");
            //     printf("Atoms in the cycle: ");
            //     for (int i = 0; i < cycle_size; i++) {
            //         printf("%d ", cycle_nodes[i]);
            //     }
            //     printf("\n");
            // } else {
            //     printf("No cycles found in the molecule.\n");
            // }
            // //  конец анализа цикла, работает нестабильно
        printf("Введите номера трех атомов в кольце через пробел\n");
        scanf("%d %d %d", &first_atom, &second_atom, &third_atom);

        printf("%d, %d, %d \n", first_atom, second_atom, third_atom);
        transform_coordinates(atoms, atom_count, first_atom, second_atom, third_atom);
        printf("Вставить атом аустышку в центр цикла? (y/n)\n");
    
        char answer;
        scanf(" %c", &answer); // Заметьте пробел перед %c для пропуска пробелов и новой строки

        if (answer == 'y' || answer == 'Y') { // Проверяем на оба регистра
           insert_dummy_center(atoms, &atom_count);
        } else {
            printf("Операция отменена.\n");
        }

    } else {
        printf("Координаты атомов не найдены.\n");
    }        
    printf("Заменить координаты в изначальном файле (y/n)\n");   
    char answer;
    scanf(" %c", &answer); // Заметьте пробел перед %c для пропуска пробелов и новой строки

    if (answer == 'y' || answer == 'Y') { // Проверяем на оба регистра
        replaceLines(filename, atoms, atom_count);
    } else {
        printf("Операция отменена.\n");
    }
    MALLOC_generate_xyz(filename, atoms, atom_count);
    return 0;
}