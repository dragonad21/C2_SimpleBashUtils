#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 4096

typedef struct {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_o;
  int flag_f;
  int more_one_file_flag;

  char str_regexec[BUFFER];
  char filename[BUFFER];
  char str[BUFFER];
  char str_o[BUFFER];
} Flags;

void parser(int argc, char* argv[], Flags* flag);
void output(char* argv[], Flags* flag);
void read_patterns_from_file(Flags* flag);

#endif