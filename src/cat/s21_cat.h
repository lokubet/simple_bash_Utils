#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //2

struct struct_flagi {
  bool b;  // нумерует все не пустые строки
  bool e;  // отображает перенос строки как $
  bool n;  //нумерует все строки
  bool s;  //много пустых строк подряд сжимает в одну
  bool t;  // показывает табы
  bool v;
};

void print_file_with_settings(FILE* F, struct struct_flagi);
void process_arguments(int argc, char** argv);
void process_file(char* file_name, struct struct_flagi flagi);
int process_flags(char* flags_str, struct struct_flagi* flagi);
bool is_file(char* str);

#endif  // SRC_CAT_S21_CAT_H_
