#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  if (argc != 5) {
    printf ("Usage: program (U|D) (L|R) xy zw\n");
    return -1;
  }
  const char ud = argv[1][0];
  const char lr = argv[2][0];
  if (ud == 'U') {
    if (lr == 'L') {
      const int x = argv[3][0] - '0';
      const int y = argv[3][1] - '0';
      if (x < y) {
        printf ("x < y\n");
      } else {
        printf ("x >= y\n");
      }
      if (x + y < 7) {
        printf ("x + y < 7\n");
      } else {
        printf ("x + y >= 7\n");
      }
    } else {
      printf ("Up/Right is empty\n");
    }
  } else {
    if (lr == 'L') {
      const int x = argv[3][0] - '0';
      const int y = argv[3][1] - '0';
      if (x < y + 4) {
        printf ("x < y + 4\n");
      } else {
        printf ("x >= y + 4\n");
      }
      if (x + y < 9) {
        printf ("x + y < 9\n");
      } else {
        printf ("x + y >= 9\n");
      }
    } else {
      printf ("Down/Right is empty\n");
    }
  }
  const int z = argv[4][0] - '0';
  const int w = argv[4][1] - '0';
  if (z > w) {
    printf ("z > w\n");
  } else {
    printf ("z <= w\n");
  }
  if (z * w > 12) {
    printf ("z * w > 12\n");
  } else {
    printf ("z * w <= 12\n");
  }
  return 0;
}

