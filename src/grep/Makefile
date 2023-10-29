CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 #-pedantic -fsanitize=address

.PHONY: all clean rebuild clang leaks

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c

clang:
	@cp ../../materials/linters/.clang-format .
	clang-format -n s21_grep.c s21_grep.h
	@rm .clang-format

clean:
	rm -rf s21_grep *.o

rebuild:
	$(MAKE) clean
	$(MAKE) all
