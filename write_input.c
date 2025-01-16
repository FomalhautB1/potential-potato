
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
    //int writing_coordinates = 0;

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


char* MALLOC_generate_xyz(char *filename, Atom atoms[], int atom_count) {
    size_t filename_len = strlen(filename) + 5; // +5 для ".xyz" и завершающего '\0'
    char *new_filename = (char*) malloc(filename_len);
    if (new_filename == NULL) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }
    strcpy(new_filename, filename);
    // Ищем последний '.' (точку) в имени файла
    char *dot = strrchr(new_filename, '.');
    if (dot != NULL) {
        // Заменяем всё после точки на новое расширение
        strcpy(dot, ".xyz");
    } else {
        // Если точки нет, просто добавляем расширение
        strcat(new_filename, ".xyz");
    }

    FILE *file = fopen(new_filename, "w"); 
    if (file == NULL) {
        perror("Ошибка при создании файла");
        free(new_filename); // Освобождаем память перед выходом
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d\n", atom_count);
    fprintf(file, "\n");
    for (int i = 0; i < atom_count; i++) {
    fprintf(file, "%s\t%f\t%f\t%f\n", atoms[i].element, atoms[i].x, atoms[i].y, atoms[i].z);
    }
    fclose(file);

    printf("Файл '%s' успешно создан.\n", new_filename);
    return new_filename;
}