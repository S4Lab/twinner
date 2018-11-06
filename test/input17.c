#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (strcmp (argv[1], "verbose") == 0) {
    const char ud1 = argv[2][0];
    const char ud2 = argv[2][1];
    const char ud3 = argv[2][2];
    if (ud1 == 'U') {
      printf ("up\n");
    } else {
      printf ("down\n");
    }
    if (argv[3][0] == 'R') {
      if (ud2 == 'U') {
        printf ("up\n");
      } else {
        printf ("down\n");
      }
    } else {
      if (ud3 == 'U') {
        printf ("up\n");
      } else {
        printf ("down\n");
      }
      const char x = argv[2][4] - '0';
      if (x > 2) {
        printf ("x > 2\n");
      } else {
        printf ("x <= 2\n");
      }
    }
  } else {
    printf ("_quiet mode_\n");
  }
  if (strcmp (argv[4], "magicvalue") == 0) {
    const char sf = argv[5][0];
    if (sf == 'S') {
      printf ("slow\n");
    } else {
      printf ("fast\n");
    }
  } else {
    printf ("magic code is wrong\n");
  }
  const char sf2 = argv[5][1];
  if (sf2 == 'S') {
    printf ("S2\n");
  } else {
    printf ("F2\n");
  }
  const char sf3 = argv[5][2];
  if (sf3 != 'F') {
    printf ("S3\n");
  } else {
    printf ("F3\n");
  }
  return 0;
}

