#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "release";
const char *str1 = "release mode\n";
const char *str2 = "debug mode\n";
const char *str3 = "Usage: program (release|debug) (U|D) (L|R) (F|B)\n";
const char *str4 = "exploring upper space\n";
const char *str5 = "exploring lower space\n";
const char *str6 = "exploring left area\n";
const char *str7 = "exploring right area\n";
const char *str8 = "moving forward\n";
const char *str9 = "moving backward\n";
const char *str10 = "details are skipped\n";
const char *str11 = "some error occurred\n";
const char *str12 = "exiting normally\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x05), (0x0D), (0x08), ('\xF5'), ('\xFF'), ('\xFF'), ('\xFF'), (0x06), (0x06), (0x10), (0x0C), (0x09), ('\xDE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x15), (0x08), ('\xD1'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x06), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x05'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x1E'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x05), (0x0D), (0x08), ('\xD0'), ('\xFF'), ('\xFF'), ('\xFF'), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x02), (0x03), (0x02), ('\x20'), ('\x01'), ('\x00'), ('\x00'), (0x05), (0x0D), (0x08), ('\xBC'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\x3A'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\xDE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x55'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x08), ('\x06'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\xA8'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xD9'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x03), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), ('\xAB'), (0x05), (0x0D), (0x07), ('\xF8'), ('\xFF'), ('\xFF'), ('\xFF'), (0x0A), (0x0C), (0x09), ('\xB9'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x04'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x03), (0x01), (0x06), (0x05), (0x07), ('\xBE'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x4C'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x05), (0x07), ('\xAD'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x46'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x01), (0x06), (0x06), (0x08), ('\x9B'), ('\xFE'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x10'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x03), (0x02), ('\x0A'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x08), ('\x6C'), ('\xFE'), ('\xFF'), ('\xFF')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((const char **)(&program[40])) = str0;
   *((const char **)(&program[73])) = str1;
   *((const char **)(&program[89])) = str2;
   *((int **)(&program[111])) = argcptr;
   *((const char **)(&program[136])) = str3;
   *((const char **)(&program[234])) = str4;
   *((const char **)(&program[250])) = str5;
   *((const char **)(&program[363])) = str6;
   *((const char **)(&program[379])) = str7;
   *((const char **)(&program[412])) = str8;
   *((const char **)(&program[428])) = str9;
   *((const char **)(&program[444])) = str10;
   *((const char **)(&program[477])) = str11;
   *((const char **)(&program[493])) = str12;
  return program;
}
