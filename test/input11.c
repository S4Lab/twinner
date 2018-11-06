#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  const int x = argv[1][0] - '0';
  const int y = argv[1][1] - '0';
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
  const int z = argv[2][0] - '0';
  const int w = argv[2][1] - '0';
  if (z < 4) {
    if (w < 7) {
      printf ("z < 4 | w < 7\n");
    } else {
      printf ("z < 4 | w >= 7\n");
    }
  } else {
    if (w < 7) {
      printf ("z >= 4 | w < 7\n");
    } else {
      printf ("z >= 4 | w >= 7\n");
    }
  }
  const int u = argv[3][0] - '0';
  if (u == 3) {
    printf ("u == 3\n");
  } else {
    printf ("u != 3\n");
  }
  return 0;
}

