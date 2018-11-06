#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  const char ud = argv[2][0];
  const char lr = argv[2][1];
  const char fb = argv[2][2];
  if (strcmp (argv[1], "multiplied") == 0) {
    if (ud == 'U') {
      if (lr == 'L') {
        printf ("UL\n");
      } else {
        printf ("UR\n");
      }
    } else {
      if (lr == 'L') {
        printf ("DL\n");
      } else {
        printf ("DR\n");
      }
    }
  } else {
    if (ud == 'U') {
      printf ("sequential: U\n");
    } else {
      printf ("sequential: D\n");
    }
    if (lr == 'L') {
      printf ("sequential: L\n");
    } else {
      printf ("sequential: R\n");
    }
    if (fb == 'F') {
      printf ("sequential: F\n");
    } else {
      printf ("sequential: B\n");
    }
  }
  if (strcmp (argv[3], "slow") == 0) {
    printf ("slow\n");
  } else {
    printf ("fast\n");
  }
  return 0;
}

