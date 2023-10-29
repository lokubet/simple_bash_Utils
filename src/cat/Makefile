CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -std=c11 #-pedantic -fsanitize=address
SOURCE = s21_cat.c
DEST = s21_cat

all: $(DEST)

s21_cat:
	$(CC) $(CFLAGS) -o $(DEST) $(SOURCE)

leaks:
	leaks -atExit -- ./$(DEST)

valgrind:
	valgrind --leak-check=full ./$(DEST)

clang:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -rf .clang-format

clean:
	@rm -rf $(DEST)

rebuild:
	$(MAKE) clean
	$(MAKE) all

