#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc != 5) {
    printf ("Usage: program (U|D) (L|R) (F|B) (verbose|quiet)\n");
    return -1;
  }
  const char ud = argv[1][0];
  const char lr = argv[2][0];
  const char fb = argv[3][0];
  const char *verbose_str = argv[4];
  if (ud == 'U') {
    if (lr == 'L') {
      if (fb == 'F') {
        printf ("Up/Left/Forth\n");
      } else {
        printf ("Up/Left/Back\n");
      }
    } else {
      if (fb == 'F') {
        printf ("Up/Right/Forth\n");
      } else {
        printf ("Up/Right/Back\n");
      }
    }
    const int verbose = (strcmp (verbose_str, "verbose") == 0);
    if (verbose) {
      printf ("Three coordinates indicate a 3D position\n");
      printf ("We are at top of a plane right now!\n");
    } else {
      printf ("_quiet output_\n");
    }
  } else {
    if (lr == 'L') {
      if (fb == 'F') {
        printf ("Down/Left/Forth\n");
      } else {
        printf ("Down/Left/Back\n");
      }
    } else {
      if (fb == 'F') {
        printf ("Down/Right/Forth\n");
      } else {
        printf ("Down/Right/Back\n");
      }
    }
    const int verbose = (strcmp (verbose_str, "verbose") == 0);
    if (verbose) {
      printf ("Three coordinates indicate a 3D position\n");
      printf ("We are exploring the downwards plane!\n");
    } else {
      printf ("_quiet output_\n");
    }
  }
  return 0;
}

