#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (strcmp (argv[1], "left") == 0) {
    const int x = argv[2][0] - '0';
    const int y = argv[2][1] - '0';
    if (x < 4) {
      if (y < 7) {
        printf ("x < 4 | y < 7\n");
      } else {
        printf ("x < 4 | y >= 7\n");
      }
    } else {
      if (y < 7) {
        printf ("x >= 4 | y < 7\n");
      } else {
        printf ("x >= 4 | y >= 7\n");
      }
    }
    if (x + y > 5) {
      printf ("x + y > 5\n");
    } else {
      printf ("x + y <= 5\n");
    }
  } else {
    printf ("right\n");
    if (strcmp (argv[3], "enter") == 0) {
      printf ("entering the room\n");
    } else {
      printf ("leaving the room\n");
    }
    const int x = argv[2][0] - '0';
    const int y = argv[2][1] - '0';
    if (x < 3) {
      if (y < 4) {
        printf ("x < 3 | y < 4\n");
      } else {
        printf ("x < 3 | y >= 4\n");
      }
    } else {
      printf ("x >= 3 | y = ?\n");
    }
    const char z = argv[4][0];
    const char w = argv[4][1];
    if (z == 'R') {
      printf ("z = R\n");
    } else {
      printf ("z = L\n");
    }
    if (w == 'R') {
      printf ("w = R\n");
    } else {
      printf ("w = L\n");
    }
  }
  if (argc > 5) {
    printf ("there were unused arguments\n");
  } else {
    printf ("not too many args\n");
  }
  return 0;
}

