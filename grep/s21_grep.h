#ifndef SRC_DOOR_S21_GREP_H_
#define SRC_DOOR_S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option {
  int e;
  int i;
  int i_flag;
  int v;
  int c;
  int c_extra;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} option;

void parse_options(int argc, char *argv[], option *options, char ***pats,
                   char ***patf, char ***arr_of_files, int *count_files,
                   int *flag_exist, int *count_p, int *count_f);
void parse_e(char *argv[], option *options, char ***pats, int *count_p, int *i,
             int j, int *flag_nextelem, int *flag_pat);
void parse_f(char *argv[], option *options, char ***patf, int *count_f, int *i,
             int j, int *flag_nextelem, int *flag_pat);
void parse_left(char *argv[], option *options, int i, int j);
void parse_filenames(char *argv[], char ***arr_of_files, int *count_files,
                     int j);
void parse_argv1(char *argv[], int k, char ***pats, int *count_p,
                 int *flag_exist, option *options);
void parse_file(char ***pats, char ***patf, int *count_p, int count_f,
                option *options);
void check_patterns(char ***pats, char ***conf_pat, int count_p, int *amount,
                    option options, int *flag_err);
void open_file(FILE **file, char *filename, int *flag, option *options);
void output_print(FILE **file, option options, char **conf_pat, char *filename,
                  int count_files, int amount);
void output_without_v(int *count_match_c, int *i, char *filename, int number,
                      int *reg_ex, regmatch_t *matches, char **str_arr,
                      regex_t *reg_arr, int amount, option options,
                      int count_files, int *flag_number);
void output_with_v(char **conf_pat, int count_files, int *count_match_c, int *i,
                   char *filename, int number, char *str_arr, int amount,
                   option options, int *flag_number);
void output_cln(option options, int count_files, char *filename,
                int count_match_c);
void fill_arr(FILE *file, char **str, option options);

#endif  // SRC_DOOR_S21_GREP_H_
