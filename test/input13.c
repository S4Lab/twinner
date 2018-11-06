#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  const char lr = argv[1][0];
  const char ud = argv[1][1];
  const char fb = argv[1][2];
  if (lr == 'L') {
    if (ud == 'U') {
      if (fb == 'F') {
        printf ("L/U/F\n");
      } else {
        printf ("L/U/B\n");
      }
    } else {
      if (fb == 'F') {
        printf ("L/D/F\n");
      } else {
        printf ("L/D/B\n");
      }
      const int x = argv[1][3] - '0';
      if (x > 5) {
        printf ("x > 5\n");
      } else {
        printf ("x <= 5\n");
      }
    }
    if (strcmp (argv[2], "left") == 0) {
      printf ("left-path\n");
    } else {
      printf ("right-path\n");
    }
  } else {
    if (ud == 'U') {
      if (fb == 'F') {
        printf ("R/U/F\n");
      } else {
        printf ("R/U/B\n");
      }
    } else {
      if (fb == 'F') {
        printf ("R/D/F\n");
      } else {
        printf ("R/D/B\n");
      }
    }
    if (strcmp (argv[2], "left") == 0) {
      const int y = argv[1][4] - '0';
      if (y < 4) {
        printf ("y < 4\n");
      } else {
        printf ("y >= 4\n");
      }
    } else {
      const int y = argv[1][4] - '0';
      if (y < 8) {
        printf ("y < 8\n");
      } else {
        printf ("y >= 8\n");
      }
      if (y > 2) {
        printf ("y > 2\n");
      } else {
        printf ("y <= 2\n");
      }
    }
  }
  if (argc > 3) {
    printf ("there were unused arguments\n");
  } else {
    printf ("not too many args\n");
  }
  return 0;
}

