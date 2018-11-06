#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (strcmp (argv[3], "right") == 0) {
    const int x = argv[4][0] - '0';
    const int y = argv[4][1] - '0';
    if (x < 6) {
      printf ("x < 6\n");
    } else {
      printf ("x >= 6\n");
    }
    if (y < 7) {
      printf ("y < 7\n");
    } else {
      printf ("y >= 7\n");
    }
  } else {
    const char ud = argv[4][0];
    const char lr = argv[4][1];
    if (ud == 'U') {
      if (lr == 'L') {
        printf ("up/left\n");
      } else {
        printf ("up/right\n");
      }
    } else {
      if (lr == 'L') {
        printf ("down/left\n");
      } else {
        printf ("down/right\n");
      }
    }
  }
  if (strcmp (argv[1], "verbose") == 0) {
    const char ud = argv[2][0];
    if (ud == 'U') {
      printf ("up\n");
    } else {
      printf ("down\n");
    }
  } else {
    printf ("_quiet mode_\n");
  }
  if (argc > 5) {
    const char fb = argv[5][0];
    if (fb == 'F') {
      printf ("forth\n");
    } else {
      printf ("back\n");
    }
  } else {
    printf ("2D\n");
  }
  const char sf = argv[2][1];
  if (sf == 'S') {
    printf ("slow\n");
  } else {
    printf ("fast\n");
  }
  return 0;
}

