CFLAGS=-Wall -Werror -Wextra

all: s21_cat

rebuild: clean all

s21_cat: s21_cat.h s21_cat.c
	gcc $(CFLAGS) s21_cat.c -o s21_cat

tests: tests.sh
	./tests.sh

clean:
	rm -rf s21_cat