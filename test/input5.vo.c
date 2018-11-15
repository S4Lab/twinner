#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "Usage: program --opt <val> <digit1><digit2>\n";
const char *str1 = "--opt";
const char *str2 = "--opt z is given\n";
const char *str3 = "unknown value is given\n";
const char *str4 = "--opt is missing\n";
const char *str5 = "digit1 is more than six\n";
const char *str6 = "digit1 is NOT more than six\n";
const char *str7 = "some error happened\n";
const char *str8 = "exiting normally\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x06), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x1E'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x05), (0x0D), (0x08), ('\xD0'), ('\xFF'), ('\xFF'), ('\xFF'), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), (0x03), (0x02), ('\xDB'), ('\x00'), ('\x00'), ('\x00'), (0x05), (0x0D), (0x08), ('\xBC'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), (0x01), (0x06), (0x05), (0x10), (0x0C), (0x09), ('\x9E'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x43'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x0A), (0x0C), (0x09), ('\x77'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x7A'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\xF5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0E), (0x0A), (0x0C), (0x09), ('\x1B'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x30'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x03), (0x08), ('\xD5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x06'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x06), (0x08), ('\xE0'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x08), ('\xB1'), ('\xFE'), ('\xFF'), ('\xFF')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[22])) = argcptr;
   *((const char **)(&program[47])) = str0;
   *((const char **)(&program[104])) = str1;
   *((const char **)(&program[164])) = str2;
   *((const char **)(&program[180])) = str3;
   *((const char **)(&program[196])) = str4;
   *((const char **)(&program[270])) = str5;
   *((const char **)(&program[286])) = str6;
   *((const char **)(&program[319])) = str7;
   *((const char **)(&program[335])) = str8;
  return program;
}
