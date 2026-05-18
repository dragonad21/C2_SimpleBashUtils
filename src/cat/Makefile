CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = s21_cat

all: $(TARGET)

$(TARGET): cat.c cat.h
	$(CC) $(CFLAGS) -o $(TARGET) cat.c

clean:
	rm -rf cat

rebuild: clean all

.PHONY: all clean rebuild