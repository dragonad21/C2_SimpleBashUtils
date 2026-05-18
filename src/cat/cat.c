#include "cat.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 0;
  }

  struct Flags flags = {0};

  parser(argc, argv, &flags);

  while (optind < argc) {
    output(argv, flags);
    optind++;
  }

  return 0;
}

void parser(int argc, char *argv[], struct Flags *flags) {
  int opt;

  struct option long_options[] = {{"number-nonblank", no_argument, 0, 'b'},
                                  {"number", no_argument, 0, 'n'},
                                  {"squeeze-blank", no_argument, 0, 's'},
                                  {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "bnsETetv", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flags->b = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'E':
        flags->E = 1;
        break;
      case 'T':
        flags->T = 1;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default: {
        break;
      }
    }
  }

  if (flags->b && flags->n) {
    flags->n = 0;
  }
}

void output(char *argv[], struct Flags flags) {
  FILE *file = fopen(argv[optind], "rb");
  if (file != NULL) {
    int opt;
    int new_line = 1;
    int skip_line = 0;
    int number_line = 1;
    int printed = 0;
    int last_was_newline = 0;

    while ((opt = fgetc(file)) != EOF) {
      printed = 0;

      if (new_line) {
        process_new_line(&skip_line, flags, opt, &last_was_newline,
                         &number_line, &new_line);
      }

      if (!skip_line && !printed) {
        if (flags.v)
          print_with_v(opt, &printed, flags);
        else
          print_without_v(opt, &printed, flags);
      }

      if (!skip_line && !printed) {
        putchar(opt);
      }

      if (opt == '\n') {
        new_line = 1;
      }
    }
    fclose(file);
  }
}

void process_new_line(int *skip_line, struct Flags flags, int opt,
                      int *last_was_newline, int *number_line, int *new_line) {
  *skip_line = 0;
  if (flags.s) {
    if (opt == '\n') {
      if (*last_was_newline) {
        *skip_line = 1;
      } else {
        *last_was_newline = 1;
      }
    } else {
      *last_was_newline = 0;
    }
  }

  if (!*skip_line) {
    if (flags.n && !flags.b) {
      printf("%6d\t", *number_line);
      (*number_line)++;
    } else if (flags.b && opt != '\n') {
      printf("%6d\t", *number_line);
      (*number_line)++;
    } else if (flags.e && opt == '\n') {
      if (!flags.b) {
        (*number_line)++;
      }
      if (flags.n || flags.b) {
        printf("      \t");
      }
    }
  }
  *new_line = 0;
}

void print_with_v(int opt, int *printed, struct Flags flags) {
  if (opt == '\t' && (flags.t || flags.T)) {
    printf("^I");
    *printed = 1;
  } else if (opt == '\n' && (flags.e || flags.E)) {
    printf("$\n");
    *printed = 1;
  } else if (opt < 32 && opt != 9 && opt != 10) {
    printf("^%c", opt + 64);
    *printed = 1;
  } else if (opt == 127) {
    printf("^?");
    *printed = 1;
  } else if (opt > 127 && opt < 160) {
    printf("M-^%c", opt - 64);
    *printed = 1;
  }
}

void print_without_v(int opt, int *printed, struct Flags flags) {
  if (opt == '\t' && (flags.t || flags.T)) {
    printf("^I");
    *printed = 1;
  } else if (opt == '\n' && (flags.e || flags.E)) {
    printf("$\n");
    *printed = 1;
  }
}