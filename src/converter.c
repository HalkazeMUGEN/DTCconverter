#include "converter.h"
#include "encode.h"
#include "decode.h"
#include <getopt.h>


#define OPT_STRING  "deo:h"


FILE* input = NULL;
FILE* output = NULL;


static CONVERT_MODE mode = MODE_ENCODE;

static const struct option longopts[] = {
  {"decode", no_argument, 0, 'd'},
  {"encode", no_argument, 0, 'e'},
  {"output", required_argument, 0, 'o'},
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'}
};


static void parse_args(int argc, char** argv);
static _Noreturn void print_usage(const char* name) __attribute__((nonnull (1)));
static _Noreturn void print_version(void);
static void convert(FILE* fp, CONVERT_MODE mode) __attribute__((nonnull (1)));


int main(int argc, char** argv) {
  parse_args(argc, argv);

  convert(input, mode);

  fclose(input);
  input = NULL;

  fclose(output);
  output = NULL;

  return 0;
}


static void parse_args(int argc, char** argv) {
  int c;
  int longindex;
  opterr = 0;

  while ((c = getopt_long(argc, argv, OPT_STRING, longopts, &longindex)) != -1) {
    switch (c) {
    case 'd':
      mode = MODE_DECODE;
      break;
    case 'e':
      mode = MODE_ENCODE;
      break;
    case 'o':
      output = fopen(optarg, "w");
      if (output == NULL) {
        show_error(ERRMSG_FAIL_OPEN_FILE);
      }
      break;
    case 'h':
      print_usage(argv[0]);
      break;
    case 'v':
      print_version();
      break;
    default:
      break;
    }
  }

  if (argc <= optind) {
    fprintf(stderr, ERRMSG_NO_INPUT);
    exit(1);
  }

  input = fopen(argv[optind], "r");
  if (input == NULL) {
    show_error(ERRMSG_FAIL_OPEN_FILE);
  }

  if (output == NULL) {
    output = stdout;
  }
}

static _Noreturn void print_usage(const char* name) {
  assert(name != NULL);

  printf("Usage: %s [options] <file>\n", name);
  puts("Options:");
  puts("  -d, --decode                HEX -> DTC");
  puts("  -e, --encode                DTC -> HEX (default)");
  puts("  -o <file>, --output <file>  Place the output into <file>.");
  puts("  -h, --help                  Give this help list.");
  puts("      --version               Print program version.");
  exit(0);
}

static _Noreturn void print_version(void) {
  puts("DTCconverter 1.0.0");
  puts("Copyright (C) 2022 Halkaze");
  exit(0);
}


static void convert(FILE* fp, CONVERT_MODE mode) {
  assert(fp != NULL);

  switch (mode) {
  case MODE_ENCODE:
    encode(fp);
    break;
  case MODE_DECODE:
    decode(fp);
    break;
  default:
    break;
  }
}
