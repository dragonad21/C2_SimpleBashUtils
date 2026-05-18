#include <getopt.h>
#include <stdio.h>
#include <string.h>

struct Flags {
  int v;
  int b;
  int n;
  int s;
  int E;
  int T;
  int t;
  int e;
};

void parser(int argc, char *argv[], struct Flags *flags);
void output(char *argv[], struct Flags flags);
void process_new_line(int *skip_line, struct Flags flags, int opt,
                      int *last_was_newline, int *number_line, int *new_line);
void print_with_v(int opt, int *printed, struct Flags flags);
void print_without_v(int opt, int *printed, struct Flags flags);
