#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  const int debug = strcmp (argv[1], "release") != 0;
  if (!debug) {
    printf ("release mode\n");
  } else {
    printf ("debug mode\n");
  }
  int ret;
  if (argc != 5) {
    printf ("Usage: program (release|debug) (U|D) (L|R) (F|B)\n");
    ret = -1;
  } else {
    ret = 0;
    const char ud = argv[2][0];
    if (ud == 'U') {
      printf ("exploring upper space\n");
    } else {
      printf ("exploring lower space\n");
    }
    if (debug) {
      const char lr = argv[3][0];
      const char fb = argv[4][0];
      if (lr == 'L') {
        printf ("exploring left area\n");
      } else {
        printf ("exploring right area\n");
      }
      if (fb == 'F') {
        printf ("moving forward\n");
      } else {
        printf ("moving backward\n");
      }
    } else {
      printf ("details are skipped\n");
    }
  }
  if (ret != 0) {
    printf ("some error occurred\n");
  } else {
    printf ("exiting normally\n");
  }
  return ret;
}

