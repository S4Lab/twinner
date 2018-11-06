#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  const char ud = argv[1][0];
  const char lr = argv[1][1];
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
  const char sf = argv[1][2];
  if (sf == 'S') {
    printf ("slow\n");
  } else {
    printf ("fast\n");
  }
  if (argv[2][0] == 'V') {
    const char ud1 = argv[3][0];
    if (ud1 == 'U') {
      printf ("up\n");
    } else {
      printf ("down\n");
    }
    const char ud2 = argv[3][1];
    if (ud2 == 'U') {
      printf ("up\n");
    } else {
      printf ("down\n");
    }
    const char ud3 = argv[3][2];
    const char lr3 = argv[3][3];
    if (ud3 == 'U') {
      if (lr3 == 'L') {
        printf ("UL\n");
      } else {
        printf ("UR\n");
      }
    } else {
      if (lr3 == 'L') {
        printf ("DL\n");
      } else {
        printf ("DR\n");
      }
    }
  } else {
    printf ("_quiet mode_\n");
  }
  return 0;
}

