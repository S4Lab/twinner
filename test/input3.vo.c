#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <iostream>
#include <iomanip>

typedef int32_t int32;

int aux; // holding intermediate values such as function ret value

int end_of_execution (const char *&ptr);
void printf_command (const char *&ptr);
void strcmp_command (const char *&ptr);
void add_and_set_aux_command (const char *&ptr);
void jump_command (const char *&ptr);
void if_then_else_command (const char *&ptr);
bool do_comparison (const char *&ptr, int arg0, int arg1);
int get_argument (const char *&ptr);

const int pointer_size = 8;
const int constant_size = 4;

const char *str0 = "Usage: program --left <num1> --right <num2>\n";
const char *str1 = "--left";
const char *str2 = "left option is missing!\n";
const char *str3 = "invalid left value!\n";
const char *str4 = "--right";
const char *str5 = "right option is missing!\n";
const char *str6 = "invalid right value!\n";
const char *str7 = "num1 is smaller than num2\n";
const char *str8 = "num1 is NOT smaller than num2\n";
const char *str9 = "sum of two numbers is more than 15\n";
const char *str10 = "sum of two numbers is NOT more than 15\n";
const char *str11 = "last line!\n";

//#define program_text "\x01\x01\xff\xff\xff\xff\xff\xff\xff\xff\x02\x03\x00\x00\x00\x01\x02\x00\x00\x00\x00\x02\x15\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x00\x02\xff\xff\xff\xff\x00\x02\x00\x00\x00\x00\x04\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01\x04\x02\x00\x00\x00\x00\x06\x02\x00\x00\x00\x00\x02\x0f\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x00\x02\xff\xff\xff\xfe\x04\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01\x04\x02\x00\x00\x00\x00\x06\x02\x00\x00\x00\x00\x02\x0f\x00\x00\x00\x02\xff\xff\xff\xff\xff\xff\xff\xff\x00\x02\xff\xff\xff\xfe\x02\xff\xff\xff\xff\xff\xff\xff\xff\x00\x02\x00\x00\x00\x00"


const char program_text[] = {
  0x01,
  0x01, '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, 5, 0, 0, 0,
  0x01,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return -1;
  0x02, -1, -1, -1, -1,
  0x00, // return 0; <- dead-code
  0x02, 0, 0, 0, 0,
  0x04, // aux = strcmp (argv[1], ...
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x01, // if (aux != 0 ...
  0x04, // aux
  0x02, 0, 0, 0, 0,
  0x06,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return -2;
  0x02, -1, -1, -1, -2,
  0x01, // if (argv[2][1] != 0
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, 0, 0, 0, 0,
  0x06,
  0x02, char ((1 + 9 + 5 + 6 + 5) * 2), 0, 0, 0,
  0x02, 0, 0, 0, 0,
  0x01, // || argv[2][0] < '0'
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0', 0, 0, 0,
  0x01,
  0x02, char (1 + 9 + 5 + 6 + 5), 0, 0, 0,
  0x02, 0, 0, 0, 0,
  0x01, // || argv[2][0] > '9'
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '9', 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return -3;
  0x02, -1, -1, -1, -3,
  0x04, // aux = strcmp (argv[3], ...
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x01, // if (aux != 0 ...
  0x04, // aux
  0x02, 0, 0, 0, 0,
  0x06,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return -4;
  0x02, -1, -1, -1, -4,
  0x01, // if (argv[4][1] != 0
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, 0, 0, 0, 0,
  0x06,
  0x02, char ((1 + 9 + 5 + 6 + 5) * 2), 0, 0, 0,
  0x02, 0, 0, 0, 0,
  0x01, // || argv[4][0] < '0'
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '0', 0, 0, 0,
  0x01,
  0x02, char (1 + 9 + 5 + 6 + 5), 0, 0, 0,
  0x02, 0, 0, 0, 0,
  0x01, // || argv[4][0] > '9'
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, '9', 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return -5;
  0x02, -1, -1, -1, -5,
  0x01, // argv[2][0] - '0' < argv[4][0] - '0'
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x01,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to after if(num1 < num2) { ... } else { ... }
  0x02, char (9), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x05, // aux = argv[2][0] + argv[4][0]
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03,
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x01, // if (num1 + num2 > 15) {
  0x04, // aux
  0x02, char (15 + '0' * 2), 0, 0, 0,
  0x03,
  0x02, 0, 0, 0, 0,
  0x02, char (9 + 6), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x03, // jump to after if(num1 < num2) { ... } else { ... }
  0x02, char (9), 0, 0, 0,
  0x02, // printf
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x02, // printf last line
  '\xFF', '\xFF','\xFF','\xFF', '\xFF', '\xFF','\xFF','\xFF',
  0x00, // return 0;
  0x02, 0, 0, 0, 0,
};


char program[sizeof (program_text)];

const char *init_program (int *argcptr, char *argv[]) {
  static_assert (sizeof (program_text) > 50, "wrong program text");
  memcpy (program, program_text, sizeof (program_text));
  static_assert (sizeof (int *) <= pointer_size, "wrong pointer size");
  /*
  for (char c : program_text) {
    std::cout << "\\x" << std::hex
      << std::setw(2) << std::setfill('0') << uint32_t (uint8_t (c));
  }
  std::cout << std::endl;
  exit (0);
  */

  *((int **) (&program[2])) = argcptr;
  *((const char **) (&program[27])) = str0;
  *((const char **) (&program[48])) = argv[1];
  *((const char **) (&program[56])) = str1;
  *((const char **) (&program[83])) = str2;
  *((const char **) (&program[99])) = &argv[2][1];
  *((const char **) (&program[125])) = &argv[2][0];
  *((const char **) (&program[151])) = &argv[2][0];
  *((const char **) (&program[176])) = str3;
  *((const char **) (&program[191])) = argv[3];
  *((const char **) (&program[199])) = str4;
  *((const char **) (&program[226])) = str5;
  *((const char **) (&program[242])) = &argv[4][1];
  *((const char **) (&program[268])) = &argv[4][0];
  *((const char **) (&program[294])) = &argv[4][0];
  *((const char **) (&program[319])) = str6;
  *((const char **) (&program[335])) = &argv[2][0];
  *((const char **) (&program[344])) = &argv[4][0];
  *((const char **) (&program[364])) = str7;
  *((const char **) (&program[379])) = str8;
  *((const char **) (&program[389])) = &argv[2][0];
  *((const char **) (&program[398])) = &argv[4][0];
  *((const char **) (&program[425])) = str9;
  *((const char **) (&program[440])) = str10;
  *((const char **) (&program[449])) = str11;
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
      case 0x04:
        strcmp_command (ptr);
        break;
      case 0x05:
        add_and_set_aux_command (ptr);
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

void strcmp_command (const char *&ptr) {
  ++ptr;
  const char **arg0 = (const char **) ptr;
  ptr += pointer_size;
  const char **arg1 = (const char **) ptr;
  ptr += pointer_size;
  aux = strcmp (*arg0, *arg1);
}

void add_and_set_aux_command (const char *&ptr) {
  ++ptr;
  const int arg0 = get_argument (ptr);
  const int arg1 = get_argument (ptr);
  aux = arg0 + arg1;
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
  } else if (type == 0x04) { // aux value
    return aux;
  } else {
    throw "wrong type";
  }
}

