#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (strcmp (argv[1], "verbose") == 0) {
    const char lr = argv[2][1];
    if (lr == 'L') {
      printf ("left\n");
    } else {
      printf ("right\n");
      const char fb = argv[2][2];
      const char fb2 = argv[2][3];
      if (fb == 'F') {
        if (fb2 == 'F') {
          printf ("FF\n");
        }
      } else {
        if (fb2 != 'B') {
          printf ("BF\n");
        } else {
          printf ("BB\n");
        }
      }
    }
    const char ud = argv[2][0];
    if (ud == 'U') {
      printf ("up\n");
    } else {
      printf ("down\n");
    }
  } else {
    printf ("_quiet mode_\n");
  }
  const int x = argv[3][0] - '0';
  const int y = argv[3][1] - '0';
  const int z = argv[3][2] - '0';
  if (x > 8) {
    if (y > z) {
      printf ("y > z\n");
    } else {
      printf ("y <= z\n");
    }
  } else {
    printf ("x is small\n");
  }
  return 0;
}

