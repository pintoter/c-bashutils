CFLAGS=-Wall -Werror -Wextra

all: s21_grep

rebuild: clean all

s21_grep: s21_grep.h s21_grep.c
	gcc $(CFLAGS) s21_grep.c -o s21_grep

tests: tests.sh
	./tests.sh

clean:
	rm -rf s21_grep