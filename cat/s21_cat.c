#include "s21_cat.h"

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  FILE *file = NULL;
  int filepos, flagstruct = 0, flagerropt = 0;
  option options = {0};
  parse_options(argc, argv, &options, &filepos, &flagstruct, &flagerropt);
  if (flagerropt == 0) {
    for (int i = filepos; i < argc; i++) {
      int flag = 0;
      open_the_file(&file, argv[i], &flag);
      if (flag == 0) {
        int *str;
        int filesize = 0;
        fill_arr(file, &str, &filesize);
        output_cat_options(&file, options, flagstruct, str, &filesize);
        free(str);
      }
    }
  }
  return 0;
}

void parse_options(int argc, char *argv[], option *options, int *filepos,
                   int *flagstruct, int *flagerropt) {
  for (int j = 1; j < argc; j++) {
    if (argv[j][0] == '-') {
      for (int i = 1; i < (int)strlen(argv[j]); i++) {
        if (argv[j][i] == 'b' || strstr(argv[j], "--number-nonblank") != NULL) {
          options->b = 1;
        } else if (argv[j][i] == 'e' || argv[j][i] == 'E') {
          options->e = 1;
          options->v = 1;
        } else if (argv[j][i] == 'n' || strstr(argv[j], "--number") != NULL) {
          options->n = 1;
        } else if (argv[j][i] == 's' ||
                   strstr(argv[j], "--squeeze-blank") != NULL) {
          options->s = 1;
        } else if (argv[j][i] == 't') {
          options->t = 1;
          options->v = 1;
        } else if (argv[j][i] == 'T') {
          options->t = 1;
        } else if (argv[j][i] == 'v') {
          options->v = 1;
        } else {
          printf("cat: illegal option -- %c\n", argv[j][i]);
          printf("usage: cat [-benstuv] [file ...]");
          i = (int)strlen(argv[j]);
          *flagerropt = 1;
        }
        *flagstruct = 1;
      }
    } else {
      *filepos = j;
      j = argc;
    }
  }
}

void open_the_file(FILE **file, char *filename, int *flag) {
  *file = fopen(filename, "r");
  if (*file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", filename);
    *flag = 1;
  }
}

void output_cat_options(FILE **file, option options, int flagstruct, int *str,
                        int *filesize) {
  int numofstr = 1;
  int flags = 0;
  if (flagstruct != 0) {
    for (int i = 0; i < *filesize - 1; i++) {
      if (options.s == 1) {
        print_opt_s(str, &i, &flags);
      }
      if (options.b == 1 || (options.b == 1 && options.n == 1)) {
        print_opt_b(str, i, &numofstr);
      } else if (options.n == 1) {
        print_opt_n(str, i, &numofstr);
      }
      if (options.t == 1) {
        print_opt_t(str, &i);
      }
      if (options.v == 1) {
        if (!isascii(str[i]) && !isprint(str[i])) {
          printf("M-");
          str[i] = toascii(str[i]);
        }
        if (iscntrl(str[i]) && str[i] != '\n' &&
            (str[i] != '\t' || str[i] == '\0')) {
          printf("^%c", str[i] == '\177' ? '?' : str[i] | 0100);
          continue;
        }
      }
      if (options.e == 1) {
        print_opt_e(str, i);
      }
      printf("%c", str[i]);
    }
  } else {
    for (int i = 0; i < *filesize - 1; i++) {
      printf("%c", str[i]);
    }
  }
  fclose(*file);
}

void fill_arr(FILE *file, int **str, int *filesize) {
  int i = 0;
  int n = 1;
  *str = (int *)calloc(n, sizeof(int));
  while (!feof(file)) {
    int ch = 0;
    while (ch != EOF) {
      ch = getc(file);
      if (ch == EOF) {
        (*str)[i] = '\0';
      } else {
        (*str)[i] = ch;
      }
      i++;
      if (i >= n - 1) {
        n *= 2;
        *str = (int *)realloc(*str, n * sizeof(int));
      }
      *filesize += 1;
    }
  }
}

void print_opt_s(int *str, int *i, int *flags) {
  if (*i == 0 && str[*i] == '\n' && str[*i + 1] == '\n') {
    while (str[*i + 1] == '\n') {
      *i += 1;
    }
  } else if (*i > 0 && str[*i] == '\n' && str[*i - 1] == '\n') {
    *flags = 1;
    while (*flags != 0 && str[*i + 1] == '\n') {
      *i += 1;
    }
  } else {
    *flags = 0;
  }
}

void print_opt_b(int *str, int i, int *numofstr) {
  if (str[i] != '\n' && ((i > 0 && str[i - 1] == '\n') || i == 0)) {
    printf("%6d\t", *numofstr);
    *numofstr += 1;
  }
}

void print_opt_n(int *str, int i, int *numofstr) {
  if ((i > 0 && str[i - 1] == '\n') || i == 0) {
    printf("%6d\t", *numofstr);
    *numofstr += 1;
  }
}

void print_opt_e(int *str, int i) {
  if (str[i] == '\n') {
    printf("$");
  }
}

void print_opt_t(int *str, int *i) {
  while (str[*i] == '\t') {
    printf("^I");
    *i += 1;
  }
}
