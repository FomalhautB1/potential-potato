
#include "write_input.h"

void replaceLines(char *filename, Atom atoms[], int atom_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка при открытии исходного файла");
        return;
    }

    // Создаем временный файл
    FILE *temp_file = fopen("temp.out", "w");
    if (temp_file == NULL) {
        perror("Ошибка при создании временного файла");
        fclose(file);
        return;
    }

    char line[LINE_LENGTH];
    int writing_coordinates = 0;

    while (fgets(line, sizeof(line), file)) {
        // Проверяем начало секции координат
        if (strstr(line, "CARTESIAN COORDINATES (ANGSTROEM)")) {
            fprintf(temp_file, "%s", line); // Переписываем заголовок
            fgets(line, sizeof(line), file); // Пропускаем пустую строку
            fprintf(temp_file, "%s", line);

            // Записываем новые координаты
            for (int i = 0; i < atom_count; i++) {
                fprintf(temp_file, "  %-2s%12.6f%12.6f%12.6f\n",
                    atoms[i].element,
                    atoms[i].x,
                    atoms[i].y,
                    atoms[i].z);
            }
            
            fprintf(temp_file, "\n");

            // Пропускаем старую секцию координат
            while (fgets(line, sizeof(line), file)) {
                if (line[0] == '\n') {
                    break;
                }
            }

            // Продолжаем обработку файла после координат
            continue;
        }

        // Переписываем остальные строки
        fprintf(temp_file, "%s", line);
    }

    fclose(file);
    fclose(temp_file);

    // Заменяем исходный файл новым
    remove(filename);
    rename("temp.out", filename);
}