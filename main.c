#include "parse_output.h"
#include "matrix_calculations.h"
#include "write_input.h"
#include <string>
#include "find_cycles.h"
#include "insert_dummy.h"


int main(int argc, char *argv[]) {

    //int first_atom;
    //int second_atom;
    //int third_atom;

    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    Atom atoms[MAX_ATOMS];

    int atom_count = read_coordinates(filename, atoms, MAX_ATOMS); //читаем координаты оптимизированной структуры из .out ORCA

        
    char answer;
    std::string xyzfilename;

    if (atom_count > 0) {

        char* new_filename = MALLOC_generate_xyz(filename, atoms, atom_count); //генерируем .xyz файл с координатами атомов
        xyzfilename = new_filename;
        free(new_filename);

        count_cycles(xyzfilename); //анализируем молкулу на циклы

        std::string input;
        std::vector<int> atom_indices;
        std::cout << "Введите номера атомов через пробел: ";
        std::getline(std::cin, input);  //читаем всю строку
        std::stringstream ss(input); //используем stringstream для разделения строки на числа
        int index;
        while (ss >> index) {
            atom_indices.push_back(index - 1);  //читаем числа из строки и добавляем их в вектор, уменьшая каждый на 1
        }

        
        std::cout << "Введенные индексы атомов: "; // Выводим введенные индексы
        for (int i : atom_indices) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        transform_coordinates(atoms, atom_count, atom_indices); // поворачиваем молкулу таким образм, что ось z перпендикулярна плоскости цикла

        char answer2;

        printf("\nВставить атом-пустышку между указанными атомами? (y/n)\n");
        scanf(" %c", &answer2); 
        if (answer2 == 'y' || answer2 == 'Y') { // Проверяем на оба регистра
            insert_dummy_center(atoms, &atom_count);
        } else {
            printf("Операция отменена.\n");
        }
        while (true) {
            printf("Добавить атом пустышку по координатам? (y/n)\n");
            scanf(" %c", &answer2);

            double x;
            double y;
            double z;

            if (answer2 == 'y' || answer2 == 'Y') { // Проверяем на оба регистра
                    // Запрос на ввод координат
                std::cout << "Введите координаты через пробел x, y, z (начало координат в центре кольца): ";
                // Чтение значений из консоли
                std::cin >> x >> y >> z;
                // Вывод введенных данных
                std::cout << "Введенные координаты: " << std::endl;
                std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
                insert_dummy(atoms, &atom_count, x, y, z);
            } else {
                printf("Операция отменена.\n");
                break;
            }
        }


    } else {
        printf("Координаты атомов не найдены.\n"); //если не получилось считать координаты атомов из .out файла
        return 1;
    }   

/// заменяет координаты в изначальном логе орки, не используется
/*
    if (argc > 2 && strcmp(argv[2], "-r") == 0) {
        printf("Заменить координаты в изначальном файле (y/n)\n");


        scanf(" %c", &answer); 

        if (answer == 'y' || answer == 'Y') { // Проверяем на оба регистра
            replaceLines(filename, atoms, atom_count);
        } else {
            printf("Операция отменена.\n");
        }
    }
*/
    MALLOC_generate_xyz(filename, atoms, atom_count); // генерируем новый .xyz файл, так как координаты молкул изменились
    if (argc > 2 && strcmp(argv[2], "-xyz") == 0) {
        printf("Сохранить %s?(y/n)\n", xyzfilename.c_str());   
        scanf(" %c", &answer); 

        if (answer == 'y' || answer == 'Y') { // Проверяем на оба регистра
            printf("**not implemented yet***.\n");
        } else {
            printf("Операция отменена.\n");
        }
    }

    std::string output_file_path = "orca.inp";

    generate_orca_input(xyzfilename.c_str(), output_file_path.c_str(), atoms, atom_count);

    return 0;
}