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

        
    char answer;
    std::string xyzfilename;

    if (atom_count > 0) {

        char* new_filename = MALLOC_generate_xyz(filename, atoms, atom_count);
        xyzfilename = new_filename;
        free(new_filename);

        count_cycles(xyzfilename);

        printf("Введите номера трех атомов в кольце через пробел\n");
        scanf("%d %d %d", &first_atom, &second_atom, &third_atom);
        printf("%d, %d, %d \n", first_atom, second_atom, third_atom);
        first_atom -= 1;
        second_atom -= 1;
        third_atom -= 1;
        transform_coordinates(atoms, atom_count, first_atom, second_atom, third_atom);

        char answer2;

        printf("Вставить атом-пустышку между указанными атомами? (y/n)\n");
        scanf(" %c", &answer2); 
        if (answer2 == 'y' || answer2 == 'Y') { // Проверяем на оба регистра
           insert_dummy_center(atoms, &atom_count);
        } else {
            printf("Операция отменена.\n");
        }

    } else {
        printf("Координаты атомов не найдены.\n");
    }   


    if (argc > 2 && strcmp(argv[2], "-r") == 0) {
        printf("Заменить координаты в изначальном файле (y/n)\n");


        scanf(" %c", &answer); 

        if (answer == 'y' || answer == 'Y') { // Проверяем на оба регистра
            replaceLines(filename, atoms, atom_count);
        } else {
            printf("Операция отменена.\n");
        }
    }

    MALLOC_generate_xyz(filename, atoms, atom_count);

    printf("Сохранить %s?(y/n)\n", xyzfilename.c_str());   
    scanf(" %c", &answer); 

    if (answer == 'y' || answer == 'Y') { // Проверяем на оба регистра
        printf("**not implemented yet***.\n");
    } else {
        printf("Операция отменена.\n");
    }


    std::string output_file_path = "orca.inp";

    generate_orca_input(xyzfilename.c_str(), output_file_path.c_str(), atoms, atom_count);

    return 0;
}