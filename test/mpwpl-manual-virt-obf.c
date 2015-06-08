#include <stdio.h>
#include <string.h>
#include <inttypes.h>

typedef int32_t int32;

int end_of_execution (const char *&ptr);
void printf_command (const char *&ptr);
void jump_command (const char *&ptr);
void if_then_else_command (const char *&ptr);
bool do_comparison (const char *&ptr, int arg0, int arg1);
int get_argument (const char *&ptr);

const int pointer_size = 8;
const int constant_size = 4;

const char *str0 = "Usage: program <command codes>\n";
const char *str1 = "a > 5, b > 5, c > 5 -- case 1\n";
const char *str2 = "a > 5, b > 5, c <= 5 -- case 2\n";
const char *str3 = "a > 5, b <= 5, c > 5 -- case 3\n";
const char *str4 = "a > 5, b <= 5, c <= 5 -- case 4\n";
const char *str5 = "a <= 5, b > 5, c > 5 -- case 5\n";
const char *str6 = "a <= 5, b > 5, c <= 5 -- case 6\n";
const char *str7 = "a <= 5, b <= 5, c > 5 -- case 7\n";
const char *str8 = "a <= 5, b <= 5, c <= 5 -- case 8\n";
const char *str9 = "exiting multi-path test program with printf-leafs\n";

#define program_text "\x01\x01\xff\xff\xff\xff\xff\xff\xff\xff\x02\x02\x00\x00\x00\x01\x02\x00\x00\x00\x00\x02\x15\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x00\x02\xff\xff\xff\xff\x00\x02\x00\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x8a\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x38\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x0f\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x09\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x32\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x0f\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x09\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x84\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x38\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x0f\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x09\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x32\x00\x00\x00\x01\x03\xff\xff\xff\xff\xff\xff\xff\xff\x02\x35\x00\x00\x00\x03\x02\x00\x00\x00\x00\x02\x0f\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x03\x02\x09\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x02\xff\xff\xff\xff\xff\xff\xff\xff\x00\x02\x00\x00\x00\x00"

/*
const char program_text[] = {
  0x01,
  0x01, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, 2, 0, 0, 0,
  0x01,
  0x02, 0, 0, 0, 0,
  0x02, 15 + 6, 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return -1;
  0x02, -1, -1, -1, -1,
  0x00, // return 0; <- dead-code
  0x02, 0, 0, 0, 0,
  0x01, // if (a>5)
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, char (138), 0, 0, 0, // else of if(a>5)
  0x01,
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, 56, 0, 0, 0, // else of if(b>5)
  0x01,
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, 15, 0, 0, 0, // else of if(c>5)
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(c>5)
  0x02, 9, 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(b>5)
  0x02, 50, 0, 0, 0,
  // else of if(b>5)
  0x01,
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, 15, 0, 0, 0, // else of if(b<=5) ... if(c>5)
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(c>5)
  0x02, 9, 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(a>5)
  0x02, char (132), 0, 0, 0,
  // else of if(a>5)

  0x01,
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, 56, 0, 0, 0, // else of if(b>5)
  0x01,
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, 15, 0, 0, 0, // else of if(c>5)
  0x02, // printf case 5
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(c>5)
  0x02, 9, 0, 0, 0,
  0x02, // printf case 6
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(b>5)
  0x02, 50, 0, 0, 0,
  // else of if(b>5)
  0x01,
  0x03, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0' + 5, 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, 15, 0, 0, 0, // else of if(b<=5) ... if(c>5)
  0x02, // printf case 7
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to end of if(c>5)
  0x02, 9, 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  // end of if(a>5)
  0x02, // final printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return 0;
  0x02, 0, 0, 0, 0,
};
*/

const char *init_program (int *argcptr, char *argv[]) {
  static_assert (sizeof (program_text) > 300, "wrong program text");
  char *program = new char[sizeof (program_text)];
  memcpy (program, program_text, sizeof (program_text));
  static_assert (sizeof (int *) <= pointer_size, "wrong pointer size");
  *((int **) (&program[2])) = argcptr;
  *((const char **) (&program[27])) = str0;
  *((const char **) (&program[49])) = &argv[1][0];
  *((const char **) (&program[75])) = &argv[1][1];
  *((const char **) (&program[101])) = &argv[1][2];
  *((const char **) (&program[126])) = str1;
  *((const char **) (&program[141])) = str2;
  *((const char **) (&program[157])) = &argv[1][2];
  *((const char **) (&program[182])) = str3;
  *((const char **) (&program[197])) = str4;
  *((const char **) (&program[213])) = &argv[1][1];
  *((const char **) (&program[239])) = &argv[1][2];
  *((const char **) (&program[264])) = str5;
  *((const char **) (&program[279])) = str6;
  *((const char **) (&program[295])) = &argv[1][2];
  *((const char **) (&program[320])) = str7;
  *((const char **) (&program[335])) = str8;
  *((const char **) (&program[344])) = str9;
  return program;
}

int main (int argc, char *argv[]) {
  const char *ptr = init_program (&argc, argv);
  for (;;) {
    switch (*ptr) {
      case 0x00:
        return end_of_execution (ptr);
      case 0x01:
        if_then_else_command (ptr);
        break;
      case 0x02:
        printf_command (ptr);
        break;
      case 0x03:
        jump_command (ptr);
        break;
    }
  }
  delete[] ptr;
  return 0;
}

int end_of_execution (const char *&ptr) {
  ++ptr;
  const int arg0 = get_argument (ptr);
  return arg0;
}

void printf_command (const char *&ptr) {
  ++ptr;
  const char **format = (const char **) ptr;
  ptr += pointer_size;
  printf (*format);
}

void jump_command (const char *&ptr) {
  ++ptr;
  const int offset = get_argument (ptr);
  ptr += offset;
}

void if_then_else_command (const char *&ptr) {
  ++ptr;
  const int arg0 = get_argument (ptr);
  const int arg1 = get_argument (ptr);
  const bool res = do_comparison (ptr, arg0, arg1);
  const int thenpart = get_argument (ptr);
  const int elsepart = get_argument (ptr);
  if (res) {
    ptr += thenpart;
  } else {
    ptr += elsepart;
  }
}

bool do_comparison (const char *&ptr, int arg0, int arg1) {
  const char type = *ptr;
  ++ptr;
  if (type == 0x1) {
    return arg0 < arg1;
  } else if (type == 0x02) {
    return arg0 <= arg1;
  } else if (type == 0x03) {
    return arg0 > arg1;
  } else if (type == 0x04) {
    return arg0 >= arg1;
  } else if (type == 0x05) {
    return arg0 == arg1;
  } else if (type == 0x06) {
    return arg0 != arg1;
  } else {
    throw "wrong operator";
  }
}

int get_argument (const char *&ptr) {
  const char type = *ptr;
  ++ptr;
  if (type == 0x01) { // pointer argument
    const int **arg0 = (const int **) ptr;
    ptr += pointer_size;
    return **arg0;
  } else if (type == 0x02) { // constant argument
    const int32 *arg0 = (const int32 *) ptr;
    ptr += constant_size;
    return *arg0;
  } else if (type == 0x03) { // pointer to char argument
    const char **arg0 = (const char **) ptr;
    ptr += pointer_size;
    return **arg0;
  } else {
    throw "wrong type";
  }
}

