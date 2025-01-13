#include "parse_output.h"


// Функция для чтения координат атомов
int read_coordinates(const char *filename, Atom atoms[], int max_atoms) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        return -1;
    }

    char line[LINE_LENGTH];
    int atom_count = 0;
    int reading_coordinates = 0;
    int optimized_geometry = 0;
    while (fgets(line, sizeof(line), file)) {
        // Проверка начала секции с координатами
        if ((strstr(line, "--- Optimized Parameters ---")) && !optimized_geometry) {
            optimized_geometry = 1;
        }
            // Проверка начала секции с координатами
        if (optimized_geometry) {
            if (strstr(line, "CARTESIAN COORDINATES (ANGSTROEM)")) {
                reading_coordinates = 1;
                // Пропускаем разделительную строку
                fgets(line, sizeof(line), file);
                continue;
            }

            // Конец координат
            if (reading_coordinates && line[0] == '\n') {
                break;
            }

            // Чтение строки с координатами
            if (reading_coordinates && atom_count < max_atoms) {
                Atom atom;
                if (sscanf(line, "%2s %lf %lf %lf", atom.element, &atom.x, &atom.y, &atom.z) == 4) {
                    atoms[atom_count++] = atom;
                }
            }
        }
    }
    fclose(file);
    return atom_count;
}

void print_atoms(Atom atoms[], int atom_count) {
    printf("Прочитано %d атомов:\n", atom_count);
    for (int i = 0; i < atom_count; i++) {
        printf("%s\t%lf\t%lf\t%lf\n", atoms[i].element, atoms[i].x, atoms[i].y, atoms[i].z);
    }
}