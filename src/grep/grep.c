#include "grep.h"

int main(int argc, char* argv[]) {
  Flags flag = {0};
  parser(argc, argv, &flag);

  while (optind < argc) {
    output(argv, &flag);
    optind++;
  }

  return 0;
}

void parser(int argc, char* argv[], Flags* flag) {
  int opt;

  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        flag->flag_e = 1;
        strcat(flag->str_regexec, optarg);
        strcat(flag->str_regexec, "|");
        break;
      case 'i':
        flag->flag_i = 1;
        break;
      case 'v':
        flag->flag_v = 1;
        break;
      case 'c':
        flag->flag_c = 1;
        break;
      case 'l':
        flag->flag_l = 1;
        break;
      case 'n':
        flag->flag_n = 1;
        break;
      case 'h':
        flag->flag_h = 1;
        break;
      case 's':
        flag->flag_s = 1;
        break;
      case 'o':
        flag->flag_o = 1;
        break;
      case 'f':
        for (int i = 1; i < argc; i++) {
          if (strcmp(argv[i], "-s") == 0) flag->flag_s = 1;
        }
        flag->flag_f = 1;
        strcpy(flag->filename, optarg);
        read_patterns_from_file(flag);
        break;
      default:
        fprintf(stderr, "Unrecognized option: %c\n", optopt);
        break;
    }

    if (flag->flag_v && flag->flag_o) {
      flag->flag_o = 0;
    }
  }

  if (!flag->flag_e && !flag->flag_f) {
    if (argc > optind) {      
      strcat(flag->str_regexec, argv[optind]);
    }
    optind++;
  }

  if (flag->flag_e || flag->flag_f) {
    flag->str_regexec[strlen(flag->str_regexec) - 1] = '\0';
  }

  if (argc - optind > 1) {
    flag->more_one_file_flag = 1;
  }
}

void output(char* argv[], Flags* flag) {
  regex_t reg;
  regmatch_t match_position;

  int regex_settings = REG_EXTENDED;
  if (flag->flag_i) {
    regex_settings = REG_EXTENDED | REG_ICASE;
  }

  regcomp(&reg, flag->str_regexec, regex_settings);

  FILE* file = fopen(argv[optind], "rb");
  if (file != NULL) {
    int str_number = 0;
    int match_counter = 0;

    while (fgets(flag->str, BUFFER, file) != NULL) {
      str_number++;

      int match = regexec(&reg, flag->str, 1, &match_position, 0);

      if (!match) {
        match_counter++;
      }

      if (flag->flag_o) {
        strcpy(flag->str_o, flag->str);
      }

      int should_output = 0;
      if (flag->flag_v) {
        should_output = match;
      } else {
        should_output = !match;
      }

      if (should_output && flag->more_one_file_flag && !flag->flag_l &&
          !flag->flag_h && !flag->flag_c && !flag->flag_o && !flag->flag_n) {
        printf("%s:", argv[optind]);
      }

      if (should_output && !flag->flag_l && !flag->flag_c && !flag->flag_n &&
          !flag->flag_o) {
        printf("%s", flag->str);
        if (flag->str[strlen(flag->str) - 1] != '\n') {
          printf("\n");
        }
      }

      if (should_output && flag->flag_n && !flag->flag_c && !flag->flag_l &&
          !flag->flag_o) {
        if (flag->more_one_file_flag && !flag->flag_h) {
          printf("%s:", argv[optind]);
        }
        printf("%d:%s", str_number, flag->str);
        if (flag->str[strlen(flag->str) - 1] != '\n') {
          printf("\n");
        }
      }

      if (should_output && flag->flag_o && !flag->flag_l && !flag->flag_c) {
        char* pointer = flag->str_o;
        int costil = 0;

        if (flag->more_one_file_flag && !flag->flag_h && !flag->flag_n) {
          printf("%s:", argv[optind]);
        }

        if (flag->flag_n) {
          if (flag->more_one_file_flag && !flag->flag_h) {
            printf("%s:", argv[optind]);
          }
          printf("%d:", str_number);
          costil = 1;
        }

        while (regexec(&reg, pointer, 1, &match_position, 0) == 0) {
          if (flag->flag_n && costil == 1) {
            costil = 0;
          } else if (flag->flag_n) {
            if (flag->more_one_file_flag && !flag->flag_h) {
              printf("%s:", argv[optind]);
            }
            printf("%d:", str_number);
          } else if (flag->more_one_file_flag && !flag->flag_h) {
            printf("%s:", argv[optind]);
          }

          int match_length = match_position.rm_eo - match_position.rm_so;
          char* match_start = pointer + match_position.rm_so;
          printf("%.*s\n", match_length, match_start);

          pointer += match_position.rm_eo;
        }
      }
    }

    if (flag->flag_l && flag->flag_c) {
      int result_count;
      int should_print_filename = 0;

      if (flag->flag_v) {
        result_count = (str_number - match_counter > 0) ? 1 : 0;
        should_print_filename = (match_counter < str_number);
      } else {
        result_count = (match_counter > 0) ? 1 : 0;
        should_print_filename = (match_counter > 0);
      }

      if (flag->more_one_file_flag && !flag->flag_h) {
        printf("%s:", argv[optind]);
      }
      printf("%d\n", result_count);

      if (should_print_filename) {
        printf("%s\n", argv[optind]);
      }
    } else if (flag->flag_l) {
      int should_print_filename = 0;

      if (flag->flag_v) {
        should_print_filename = (match_counter < str_number);
      } else {
        should_print_filename = (match_counter > 0);
      }

      if (should_print_filename) {
        printf("%s\n", argv[optind]);
      }
    } else if (flag->flag_c) {
      int count_to_print;

      if (flag->flag_v) {
        count_to_print = str_number - match_counter;
      } else {
        count_to_print = match_counter;
      }

      if (flag->more_one_file_flag && !flag->flag_h) {
        printf("%s:", argv[optind]);
      }
      printf("%d\n", count_to_print);
    }

    regfree(&reg);
    fclose(file);
  } else {
    regfree(&reg);
    if (!flag->flag_s) {
       printf("gggg\n");
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
    }
  }
}

void read_patterns_from_file(Flags* flag) {
  FILE* file = fopen(flag->filename, "rb");
  if (file != NULL) {
    char line[BUFFER];
    int is_first = 1;

    while (fgets(line, BUFFER, file)) {
      if (strlen(line) > 0) {
        if (strcmp(line, "\n") == 0) {
          strcat(flag->str_regexec, "\n");
        }

        line[strcspn(line, "\n")] = '\0';
        if (!is_first) {
          strcat(flag->str_regexec, "|");
        }
        strcat(flag->str_regexec, line);
        is_first = 0;
      }
    }
    fclose(file);
  } else {
    if (!flag->flag_s) {

      fprintf(stderr, "grep: %s: No such file or directory\n", flag->filename);
    }
  }
}