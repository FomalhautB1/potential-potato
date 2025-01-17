# Имя исполняемого файла
TARGET = Program

# Компилятор
CC = g++

# Флаги компиляции
CFLAGS = -Wall -Wextra -std=c++11 -fpermissive -I/usr/include/openbabel3 -I/usr/include/eigen3 -Wno-deprecated-declarations -Wno-unused-parameter

# Флаги линковки
LDFLAGS = -lm -lopenbabel

# Список исходных файлов
SRCS = main.c parse_output.c matrix_calculations.c write_input.c find_cycles.c insert_dummy.c

# Объектные файлы (замена .c на .o)
OBJS = $(SRCS:.c=.o)

# Правило по умолчанию: сборка проекта
all: check_dependencies $(TARGET)

# Проверка зависимостей
check_dependencies:
	@echo "Проверка наличия зависимостей..."
	@which g++ > /dev/null || (echo "Компилятор g++ не найден!" && exit 1)
	@pkg-config --exists eigen3 || (echo "Библиотека Eigen3 не найдена! Установите её с помощью 'sudo apt-get install libeigen3-dev'." && exit 1)
	@pkg-config --exists openbabel-3 || (echo "Библиотека OpenBabel 3 не найдена! Установите её с помощью 'sudo apt-get install libopenbabel-dev'." && exit 1)
	@echo "Все зависимости установлены."

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Компиляция отдельных .c файлов в .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка промежуточных файлов
clean:
	rm -f $(OBJS) $(TARGET)