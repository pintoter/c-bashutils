#ifndef SRC_DOOR_S21_CAT_H_
#define SRC_DOOR_S21_CAT_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "locale.h"

typedef struct option {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} option;

void parse_options(int argc, char *argv[], option *options, int *filepos,
                   int *flagstruct, int *flagerropt);
void open_the_file(FILE **file, char *filename, int *flag);
void fill_arr(FILE *file, int **str, int *filesize);
void output_cat_options(FILE **file, option options, int flagstruct, int *str,
                        int *filesize);
void print_opt_s(int *str, int *i, int *flags);
void print_opt_b(int *str, int i, int *numofstr);
void print_opt_n(int *str, int i, int *numofstr);
void print_opt_e(int *str, int i);
void print_opt_t(int *str, int *i);

#endif  // SRC_DOOR_S21_CAT_H_
