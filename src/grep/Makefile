CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = s21_grep

all: $(TARGET)

$(TARGET): grep.c grep.h
	$(CC) $(CFLAGS) -o $(TARGET) grep.c

clean:
	rm -rf grep

rebuild: clean all

.PHONY: all clean rebuild