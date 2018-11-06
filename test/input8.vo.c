#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "Usage: program (U|D) (L|R) (F|B) (verbose|quiet)\n";
const char *str1 = "Up/Left/Forth\n";
const char *str2 = "Up/Left/Back\n";
const char *str3 = "Up/Right/Forth\n";
const char *str4 = "Up/Right/Back\n";
const char *str5 = "verbose";
const char *str6 = "Three coordinates indicate a 3D position\n";
const char *str7 = "We are at top of a plane right now!\n";
const char *str8 = "_quiet output_\n";
const char *str9 = "Down/Left/Forth\n";
const char *str10 = "Down/Left/Back\n";
const char *str11 = "Down/Right/Forth\n";
const char *str12 = "Down/Right/Back\n";
const char *str13 = "verbose";
const char *str14 = "Three coordinates indicate a 3D position\n";
const char *str15 = "We are exploring the downwards plane!\n";
const char *str16 = "_quiet output_\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x06), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x05'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xB7'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x97'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x77'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x08'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x09), ('\xF1'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0C), (0x09), ('\x51'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\x7D'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x55'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\xE1'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x86'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x4C'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x37'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x8F'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x46'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x31'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x58'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x46'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\xF5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x06), (0x05), (0x10), (0x09), ('\x36'), ('\xFF'), ('\xFF'), ('\xFF'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x08), ('\xD8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x1A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\xDB'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\xA5'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x4C'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x37'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\xAE'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x46'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x31'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x05), (0x07), ('\x77'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x46'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\x14'), ('\xFF'), ('\xFF'), ('\xFF'), (0x06), (0x05), (0x10), (0x09), ('\x55'), ('\xFE'), ('\xFF'), ('\xFF'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x08), ('\xF7'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x1A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[12])) = argcptr;
   *((const char **)(&program[37])) = str0;
   *((const char **)(&program[252])) = str1;
   *((const char **)(&program[268])) = str2;
   *((const char **)(&program[307])) = str3;
   *((const char **)(&program[323])) = str4;
   *((const char **)(&program[357])) = str5;
   *((const char **)(&program[389])) = str6;
   *((const char **)(&program[399])) = str7;
   *((const char **)(&program[415])) = str8;
   *((const char **)(&program[477])) = str9;
   *((const char **)(&program[493])) = str10;
   *((const char **)(&program[532])) = str11;
   *((const char **)(&program[548])) = str12;
   *((const char **)(&program[582])) = str13;
   *((const char **)(&program[614])) = str14;
   *((const char **)(&program[624])) = str15;
   *((const char **)(&program[640])) = str16;
  return program;
}
