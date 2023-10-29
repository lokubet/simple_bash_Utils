#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048

char pattern[BUFFER_SIZE];

typedef struct {
  int e, i, v, c, l, n, h, s, f, o;
} flags;

void parser(int argc, char **argv, flags *flag);
void reader(int argc, char **argv, flags *flag);
void flags_output(char **argv, FILE *file, int files_count, flags *flag);
void flagE(int flagE_count);
void flagF(int flagE_count);
void flagO(int status, char *buffer, regex_t re, size_t nmatch,
           regmatch_t pmatch[nmatch]);
void flagC(int files_count, flags *flag, size_t match_count, char **argv);

#endif  // SRC_GREP_S21_GREP_H_
