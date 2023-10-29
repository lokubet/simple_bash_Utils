#include "s21_grep.h"

int main(int argc, char **argv) {
  flags flag = {0};
  parser(argc, argv, &flag);
  reader(argc, argv, &flag);
  return 0;
}

void parser(int argc, char **argv, flags *flag) {
  int f, flagE_count = 0;
  while ((f = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1) {
    switch (f) {
      case 'e':
        flag->e = 1;
        flagE_count++;
        flagE(flagE_count);
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        flagF(flagE_count);
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      default:
        exit(EXIT_FAILURE);
    }
    if (flag->v || flag->c) {
      flag->o = 0;
    }
  }
}

void reader(int argc, char **argv, flags *flag) {
  size_t pattern_flag = 0;
  int files_count = argc - optind - 1;

  while (optind < argc) {
    if (!flag->e && !flag->f) {
      if (pattern_flag == 0) {
        strcat(pattern, argv[optind]);
        pattern_flag = 1;
        optind++;
      }
    }
    FILE *file = fopen(argv[optind], "r");
    if (file != NULL) {
      flags_output(argv, file, files_count, flag);
      fclose(file);
    } else {
      if (!flag->s) {
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
      }
    }
    optind++;
  }
}

void flagE(int flagE_count) {
  if (flagE_count > 1) {
    strcat(pattern, "|");
  }
  strcat(pattern, optarg);
}

void flagF(int flagE_count) {
  FILE *file = fopen(optarg, "r");
  char buffer[BUFFER_SIZE];
  if (file != NULL) {
    size_t linesC = 0;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
      if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = 0;
      }
      if (linesC > 0 || flagE_count > 0) {
        strcat(pattern, "|");
      }
      if (*buffer == '\0') {
        strcat(pattern, ".");
      } else {
        strcat(pattern, buffer);
      }
      linesC++;
    }
    fclose(file);
  } else {
    fprintf(stderr, "File not found\n");
  }
}

void flagO(int status, char *buffer, regex_t re, size_t nmatch,
           regmatch_t pmatch[nmatch]) {
  char *ptr = buffer;
  while (!status) {
    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           ptr + pmatch[0].rm_so);
    ptr += pmatch[0].rm_eo;
    status = regexec(&re, ptr, 1, pmatch, REG_NOTBOL);
  }
}

void flagC(int files_count, flags *flag, size_t match_count, char **argv) {
  if (files_count > 1 && !flag->h) printf("%s:", argv[optind]);
  if (flag->l && match_count) {
    printf("1\n");
  } else {
    printf("%zu\n", match_count);
  }
}

void flags_output(char **argv, FILE *file, int files_count, flags *flag) {
  regex_t re;
  size_t nmatch = 1;
  regmatch_t pmatch[nmatch];
  int cflags = REG_EXTENDED;

  if (flag->e) {
    files_count++;
  }
  if (flag->i) cflags = REG_EXTENDED | REG_ICASE;
  if (regcomp(&re, pattern, cflags) == 0) {
    size_t match_count = 0, line_number = 1, is_new_line = 1;
    while (!feof(file)) {
      char buffer[BUFFER_SIZE];
      if (fgets(buffer, BUFFER_SIZE, file)) {
        int status = regexec(&re, buffer, nmatch, pmatch, 0);
        if (flag->v) status = !status;
        if (status != REG_NOMATCH) {
          if (!flag->c && !flag->l) {
            if (files_count > 1 && !flag->h) printf("%s:", argv[optind]);
            if (flag->n) printf("%zu:", line_number);
            if (flag->o) {
              is_new_line = 0;
              flagO(status, buffer, re, nmatch, pmatch);
            } else {
              printf("%s", buffer);
            }
            if (buffer[strlen(buffer) - 1] != '\n' && is_new_line) printf("\n");
          }
          match_count++;
        }
        line_number++;
      }
    }
    if (flag->c) {
      flagC(files_count, flag, match_count, argv);
    }
    if (flag->l && match_count) {
      printf("%s\n", argv[optind]);
    }
  }
  regfree(&re);
}
