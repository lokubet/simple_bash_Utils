#include "s21_cat.h"

void print_file_with_settings(FILE* F, struct struct_flagi flags) {
  int c = 0, new_lines_counter = 0;
  int index_vseh_strochek = 1;
  char c_pred = '\n';
  while (EOF != (c = getc(F))) {
    new_lines_counter = (c == '\n') ? new_lines_counter + 1 : 0;
    if ((new_lines_counter > 2 && flags.s)) {
      continue;
    }
    if (c_pred == '\n' && (flags.n || flags.b)) {
      if (!(flags.b && c == '\n')) {
        printf("%6d\t", index_vseh_strochek);
        ++index_vseh_strochek;
      }
    }
    if (flags.v) {
      if (c < 32 && c != 9 && c != 10)
        if (c += 64) printf("^");
      if (c == 127) {
        c = '?';
        printf("^");
      }
    }
    if (flags.t && c == '\t') {
      printf("^I");
      // continue;
    }
    if (flags.e && c == '\n') {
      putchar('$');
    }
    if (!(flags.t && c == '\t')) {
      if (EOF == putchar(c)) {
        perror("1");
        return;
      }
    }
    c_pred = c;
  }
  if (ferror(F)) {
    perror("2");
    return;
  }
}

bool is_file(char* str) { return (str[0] != '-' && strcmp(str, "--") != 0); }

void process_file(char* file_name, struct struct_flagi flagi) {
  FILE* f = fopen(file_name, "r");
  if (!f) {
    perror("3");
    return;
  }
  print_file_with_settings(f, flagi);
  fclose(f);
}

int process_flags(char* flags_str, struct struct_flagi* flagi) {
  char* all_flags = "benstv";
  for (size_t index = 1; index < strlen(flags_str); ++index) {
    if (strchr(all_flags, flags_str[index])) {
      if (flags_str[index] == 'n') {
        flagi->n = true;
      }
      if (flags_str[index] == 'e') {
        flagi->e = true;
        flagi->v = true;
      }
      if (flags_str[index] == 'E') {
        flagi->e = true;
      }
      if (flags_str[index] == 's') {
        flagi->s = true;
      }
      if (flags_str[index] == 't') {
        flagi->t = true;
        flagi->v = true;
      }
      if (flags_str[index] == 'T') {
        flagi->t = true;
      }
      if (flags_str[index] == 'v') {
        flagi->v = true;
      }
      if (flags_str[index] == 'b') {
        flagi->b = true;
      }
    } else {
      printf("takogo flaga ne suschestvuet");
      return 1;
    }
  }
  return 0;
}

void process_arguments(int argc, char** argv) {
  struct struct_flagi flagi = {
      .b = false, .e = false, .n = false, .s = false, .t = false, .v = false};
  for (int i = 1; i < argc; ++i) {
    if (is_file(argv[i])) {
      process_file(argv[i], flagi);
    } else {
      if (process_flags(argv[i], &flagi)) {
        return;
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc == 1) return printf("you forgot to enter a file, he"), 1;
  process_arguments(argc, argv);
  return 0;
}
