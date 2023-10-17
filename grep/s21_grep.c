#include "s21_grep.h"

int main(int argc, char *argv[]) {
  FILE *file = NULL;
  option options = {0};
  char **pats, **patf, **arr_of_files, **conf_pat;
  int count_files = 0, flag_exist = 0, count_p = 0, count_f = 0, i = 0, k,
      amount = 0, flag_err = 0;
  if (argc > 2) {
    parse_options(argc, argv, &options, &pats, &patf, &arr_of_files,
                  &count_files, &flag_exist, &count_p, &count_f);
    if (flag_exist != 0) {
      i++;
    }
    if (options.f == 1) {
      parse_file(&pats, &patf, &count_p, count_f, &options);
    }
    check_patterns(&pats, &conf_pat, count_p, &amount, options, &flag_err);
    if (flag_err == 0) {
      for (; i < count_files; i++) {
        int flag = 0;
        open_file(&file, arr_of_files[i], &flag, &options);
        if (flag == 0) {
          output_print(&file, options, conf_pat, arr_of_files[i],
                       count_files - flag_exist, amount);
          fclose(file);
        }
      }
    }
    for (k = 0; k < count_files; k++) {
      free(arr_of_files[k]);
    }
    free(arr_of_files);
    if (amount != 0) {
      for (k = 0; k < amount; k++) {
        free(conf_pat[k]);
      }
      free(conf_pat);
    }
  }
  return 0;
}

void parse_options(int argc, char *argv[], option *options, char ***pats,
                   char ***patf, char ***arr_of_files, int *count_files,
                   int *flag_exist, int *count_p, int *count_f) {
  char *av_opt = "eivclnhsfo";
  int flag_pat = 0;
  for (int j = 1; j < argc; j++) {
    int next_elem = 0;
    if (argv[j][0] == '-') {
      if (strchr(argv[j], 'e') != NULL || strchr(argv[j], 'f') != NULL) {
        for (int i = 1; i < (int)strlen(argv[j]); i++) {
          if (argv[j][i] == 'e')
            parse_e(argv, options, pats, count_p, &i, j, &next_elem, &flag_pat);
          else if (argv[j][i] == 'f')
            parse_f(argv, options, patf, count_f, &i, j, &next_elem, &flag_pat);
          else if (strchr(av_opt, argv[j][i]) != NULL)
            parse_left(argv, options, i, j);
          else {
            if (options->s == 0)
              fprintf(stderr, "grep: invalid option -- %c\n", argv[j][i]);
            i = (int)strlen(argv[j]);
          }
        }
      } else {
        for (int i = 1; i < (int)strlen(argv[j]); i++) {
          if (strchr(av_opt, argv[j][i]) != NULL)
            parse_left(argv, options, i, j);
          else if (options->s == 0)
            fprintf(stderr, "grep: invalid option -- %c\n", argv[j][i]);
        }
      }
    } else {
      parse_filenames(argv, arr_of_files, count_files, j);
    }
    if (next_elem == 1) j++;
  }
  for (int k = 1; k < argc; k++) {
    if (flag_pat == 0 && argv[k][0] != '-') {
      parse_argv1(argv, k, pats, count_p, flag_exist, options);
      break;
    }
  }
}

void parse_e(char *argv[], option *options, char ***pats, int *count_p, int *i,
             int j, int *next_elem, int *flag_pat) {
  options->e = 1;
  if (*i == (int)strlen(argv[j]) - 1) {
    if (*count_p == 0) {
      *pats = (char **)calloc((*count_p) + 1, sizeof(char *));
    } else {
      *pats = (char **)realloc(*pats, ((*count_p) + 1) * sizeof(char *));
    }
    (*pats)[*count_p] = (char *)calloc(strlen(argv[j + 1]) + 1, sizeof(char));
    if (argv[j + 1] != NULL) strcpy((*pats)[*count_p], argv[j + 1]);
    *next_elem = 1;
    if (strcmp(".", argv[j + 1]) == 0) {
      options->c_extra = 1;
    }
  } else {
    (*i)++;
    if ((*count_p) == 0) {
      *pats = (char **)calloc((*count_p) + 1, sizeof(char *));
      (*pats)[*count_p] = (char *)calloc(1, sizeof(char));
    } else {
      *pats = (char **)realloc(*pats, ((*count_p) + 1) * sizeof(char *));
    }
    int k = 0;
    while (argv[j][*i] != '\0') {
      (*pats)[*count_p] =
          (char *)realloc((*pats)[*count_p], (k + 2) * sizeof(char));
      (*pats)[*count_p][k] = argv[j][*i];
      (*i)++;
      k++;
    }
    (*pats)[*count_p][k] = '\0';
  }
  (*count_p)++;
  *i = (int)strlen(argv[j]);
  *flag_pat = 1;
}

void parse_f(char *argv[], option *options, char ***patf, int *count_f, int *i,
             int j, int *next_elem, int *flag_pat) {
  options->f = 1;
  if (*i == (int)strlen(argv[j]) - 1) {
    if (*count_f == 0) {
      *patf = (char **)calloc((*count_f) + 1, sizeof(char *));
    } else {
      *patf = (char **)realloc(*patf, ((*count_f) + 1) * sizeof(char *));
    }
    (*patf)[*count_f] =
        (char *)calloc((int)strlen(argv[j + 1]) + 1, sizeof(char));
    if (argv[j + 1] != NULL) strcpy((*patf)[*count_f], argv[j + 1]);
    *next_elem = 1;
  } else {
    (*i)++;
    if (*count_f == 0) {
      *patf = (char **)calloc((*count_f) + 1, sizeof(char *));
      (*patf)[*count_f] = (char *)calloc(1, sizeof(char));
    } else {
      *patf = (char **)realloc(*patf, ((*count_f) + 1) * sizeof(char *));
    }
    int k = 0;
    while (argv[j][*i] != '\0') {
      (*patf)[*count_f] =
          (char *)realloc((*patf)[*count_f], (k + 2) * sizeof(char));
      (*patf)[*count_f][k] = argv[j][*i];
      (*i)++;
      k++;
    }
    (*patf)[*count_f][k] = '\0';
  }
  (*count_f)++;
  *i = (int)strlen(argv[j]);
  *flag_pat = 1;
}

void parse_left(char *argv[], option *options, int i, int j) {
  if (argv[j][i] == 'i') {
    options->i = 1;
    options->i_flag = 2;
  } else if (argv[j][i] == 'v') {
    options->v = 1;
  } else if (argv[j][i] == 'c') {
    options->c = 1;
  } else if (argv[j][i] == 'l') {
    options->l = 1;
  } else if (argv[j][i] == 'n') {
    options->n = 1;
  } else if (argv[j][i] == 'h') {
    options->h = 1;
  } else if (argv[j][i] == 's') {
    options->s = 1;
  } else if (argv[j][i] == 'o') {
    options->o = 1;
  }
}

void parse_filenames(char *argv[], char ***arr_of_files, int *count_files,
                     int j) {
  if (*count_files == 0) {
    *arr_of_files = (char **)calloc((*count_files) + 1, sizeof(char *));
  } else {
    *arr_of_files =
        (char **)realloc(*arr_of_files, ((*count_files) + 1) * sizeof(char *));
  }
  (*arr_of_files)[*count_files] =
      (char *)calloc((int)strlen(argv[j]) + 1, sizeof(char));
  if (argv[j] != NULL) strcpy((*arr_of_files)[*count_files], argv[j]);
  (*count_files)++;
}

void parse_argv1(char *argv[], int k, char ***pats, int *count_p,
                 int *flag_exist, option *options) {
  options->e = 1;
  *pats = (char **)calloc(1, sizeof(char *));
  (*pats)[*count_p] = (char *)calloc((int)strlen(argv[k]) + 1, sizeof(char));
  strcpy((*pats)[*count_p], argv[k]);
  *flag_exist = 1;
  *count_p = 1;
}

void parse_file(char ***pats, char ***patf, int *count_p, int count_f,
                option *options) {
  FILE *file_f = NULL;
  for (int i = 0; i < count_f; i++) {
    file_f = fopen((*patf)[i], "r");
    if (file_f != NULL) {
      fseek(file_f, 0, SEEK_END);
      long size = ftell(file_f);
      if (0 == size)
        continue;
      else {
        fseek(file_f, 0, SEEK_SET);
        while (!feof(file_f)) {
          if ((*count_p) == 0)
            *pats = (char **)calloc((*count_p) + 1, sizeof(char *));
          else
            *pats = (char **)realloc(*pats, ((*count_p) + 1) * sizeof(char *));
          char ch = 0;
          int j = 0;
          while (ch != '\n' && ch != EOF) {
            ch = (char)getc(file_f);
            if (j == 0)
              (*pats)[*count_p] = (char *)calloc(j + 2, sizeof(char));
            else
              (*pats)[*count_p] =
                  (char *)realloc((*pats)[*count_p], (j + 2) * sizeof(char));
            if (ch == EOF || ch == '\n') {
              if (j == 0 && ch == '\n') {
                (*pats)[*count_p][j] = '\n';
                (*pats)[*count_p][j + 1] = '\0';
              } else
                (*pats)[*count_p][j] = '\0';
            } else
              (*pats)[*count_p][j] = ch;
            j++;
          }
          (*count_p)++;
        }
      }
    } else {
      if (options->s == 0)
        fprintf(stderr, "grep: %s: No such file or directory\n", (*patf)[i]);
    }
    fclose(file_f);
    free((*patf)[i]);
  }
  free(*patf);
}

void check_patterns(char ***pats, char ***conf_pat, int count_p, int *amount,
                    option options, int *flag_err) {
  int count_failed = 0;
  for (int i = 0; i < count_p; i++) {
    regex_t reg_arr;
    if (regcomp(&reg_arr, (*pats)[i], options.i_flag) == 0) {
      if (*amount == 0) {
        *conf_pat = (char **)calloc((*amount) + 1, sizeof(char *));
      } else {
        *conf_pat =
            (char **)realloc(*conf_pat, ((*amount) + 1) * sizeof(char *));
      }
      (*conf_pat)[*amount] =
          (char *)calloc((int)strlen((*pats)[i]) + 1, sizeof(char));
      strcpy((*conf_pat)[*amount], (*pats)[i]);
      (*amount)++;
    } else {
      if (options.s == 0)
        fprintf(stderr, "Program can't compile the pattern: %s\n", (*pats)[i]);
      count_failed++;
    }
    regfree(&reg_arr);
    free((*pats)[i]);
  }
  free(*pats);
  if (count_failed == count_p) *flag_err = 1;
}

void open_file(FILE **file, char *filename, int *flag, option *options) {
  *file = fopen(filename, "r");
  if (*file == NULL) {
    if (options->s == 0)
      fprintf(stderr, "grep: %s: No such file or directory\n", filename);
    *flag = 1;
  }
}

void output_print(FILE **file, option options, char **conf_pat, char *filename,
                  int count_files, int amount) {
  int count_match_c = 0, number = 0;
  while (!feof(*file)) {
    number++;
    char *str_arr = NULL;
    fill_arr(*file, &str_arr, options);
    if ((int)strlen(str_arr) > 0) {
      int flag_number = 0;
      char *copy_str = str_arr;
      for (int i = 0; i < amount; i++) {
        regex_t reg_arr;
        regmatch_t matches[1];
        regcomp(&reg_arr, conf_pat[i], options.i_flag);
        int reg_ex;
        if (options.o == 0)
          reg_ex = regexec(&reg_arr, copy_str, 0, NULL, 0);
        else
          reg_ex = regexec(&reg_arr, copy_str, 1, matches, 0);
        if (reg_ex == 0 && options.v == 0) {
          output_without_v(&count_match_c, &i, filename, number, &reg_ex,
                           &matches[0], &copy_str, &reg_arr, amount, options,
                           count_files, &flag_number);
        } else if (reg_ex == 1 && options.v == 1) {
          output_with_v(conf_pat, count_files, &count_match_c, &i, filename,
                        number, copy_str, amount, options, &flag_number);
        } else if (reg_ex == 0 && options.v == 1) {
          i = amount;
        }
        regfree(&reg_arr);
      }
    }
    free(str_arr);
  }
  output_cln(options, count_files, filename, count_match_c);
}

void output_without_v(int *count_match_c, int *i, char *filename, int number,
                      int *reg_ex, regmatch_t *matches, char **str_arr,
                      regex_t *reg_arr, int amount, option options,
                      int count_files, int *flag_number) {
  if (options.c == 1 || options.l == 1) {
    (*count_match_c)++;
    *i = amount;
  } else {
    if (options.h == 0 && count_files > 1) printf("%s:", filename);
    if (options.n == 1 && *flag_number == 0) {
      printf("%d:", number);
      *flag_number = 1;
    }
    if (options.o == 1) {
      while (*reg_ex == 0) {
        for (int k = (int)matches[0].rm_so; k < (int)matches[0].rm_eo; k++) {
          printf("%c", (*str_arr)[k]);
        }
        printf("\n");
        *str_arr = *str_arr + matches[0].rm_eo;
        *reg_ex = regexec(reg_arr, *str_arr, 1, matches, 0);
      }
    } else {
      printf("%s", *str_arr);
      *i = amount;
    }
  }
}

void output_with_v(char **conf_pat, int count_files, int *count_match_c, int *i,
                   char *filename, int number, char *str_arr, int amount,
                   option options, int *flag_number) {
  int flag_vv = 0;
  regex_t reg_arr2;
  while (*i < amount && flag_vv == 0) {
    regcomp(&reg_arr2, conf_pat[*i], options.i_flag);
    int reg_ex = regexec(&reg_arr2, str_arr, 0, NULL, 0);
    if (reg_ex == 0) {
      flag_vv = 1;
    }
    regfree(&reg_arr2);
    (*i)++;
  }
  if (flag_vv != 1) {
    if (options.c == 1 || options.l == 1) {
      (*count_match_c)++;
    } else {
      if (options.h == 0 && count_files > 1) {
        printf("%s:", filename);
      }
      if (options.n == 1 && *flag_number == 0) {
        printf("%d:", number);
        *flag_number = 1;
      }
      printf("%s", str_arr);
    }
  }
  *i = amount;
}

void output_cln(option options, int count_files, char *filename,
                int count_match_c) {
  if (options.c == 1 && options.l == 1) {
    if (options.h == 0 && count_files > 1) {
      printf("%s:", filename);
    }
    if (count_match_c > 0) {
      printf("%d\n", 1);
      printf("%s\n", filename);
    } else {
      printf("%d\n", 0);
    }
  } else if (options.c == 1) {
    if (options.h == 0 && count_files > 1) {
      printf("%s:", filename);
    }
    printf("%d\n", count_match_c);
  } else if (options.l == 1) {
    if (count_match_c > 0) {
      printf("%s\n", filename);
    }
  }
}

void fill_arr(FILE *file, char **str, option options) {
  int i = 0;
  int n = 1;
  *str = (char *)calloc(n + 2, sizeof(char));
  char ch = 0;
  while (ch != '\n' && ch != EOF) {
    ch = (char)getc(file);
    if (i == 0 && ch == EOF) {
      break;
    } else if (options.c_extra == 1 && i == 0 && ch == '\n') {
      (*str)[i] = '\0';
    } else if (ch == EOF) {
      if ((*str)[i - 1] != '\n') {
        (*str)[i] = '\n';
        (*str)[i + 1] = '\0';
      }
    } else if (ch == '\n') {
      (*str)[i] = '\n';
      (*str)[i + 1] = '\0';
    } else {
      (*str)[i] = ch;
    }
    i++;
    if (i >= n - 1) {
      n += 10;
      *str = (char *)realloc(*str, (n + 1) * sizeof(char));
    }
  }
}