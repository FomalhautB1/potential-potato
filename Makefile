# Имя исполняемого файла
TARGET = Program

# Компилятор
CC = g++

# Флаги компиляции
CFLAGS = -Wall -Wextra -std=c++11

# Флаги линковки
LDFLAGS = -lm

# Список исходных файлов
SRCS = main.c parse_output.c matrix_calculations.c write_input.c find_cycles.c insert_dummy.c

# Объектные файлы (замена .c на .o)
OBJS = $(SRCS:.c=.o)

# Правило по умолчанию: сборка проекта
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Компиляция отдельных .c файлов в .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка промежуточных файлов
clean:
	rm -f $(OBJS) $(TARGET)
