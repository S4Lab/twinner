#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  const char x = argv[1][0] - '0';
  const char y = argv[1][1] - '0';
  const char z = argv[1][2] - '0';
  if (x < y) {
    if (z > 5) {
      printf ("small x | z > 5\n");
    } else {
      printf ("small x | z <= 5\n");
    }
  } else {
    if (x > z) {
      if (x < 7) {
        printf ("z is min | x < 7\n");
      } else {
        printf ("z is min | x >= 7\n");
      }
    } else {
      if (x + z > 8) {
        printf ("x is min | x+z > 8\n");
      } else {
        printf ("x is min | x+z <= 8\n");
      }
    }
  }
  if (strcmp (argv[2], "slow") == 0) {
    const char ud = argv[3][0];
    if (ud == 'U') {
      printf ("slow up\n");
    } else {
      printf ("slow down\n");
    }
  } else {
    const char ud = argv[3][0];
    if (ud == 'U') {
      printf ("quick up\n");
    } else {
      printf ("quick down\n");
    }
  }
  return 0;
}

