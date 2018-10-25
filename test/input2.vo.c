#include <stdio.h>
#include <string.h>
#include <string.h> 
const char *str0 = "Usage: program <one-option> <its-value>\n";
const char *str1 = "--option";
const char *str2 = "unknown option!\n";
const char *str3 = "optvalue";
const char *str4 = "invalid value!\n";
const char *str5 = "correct option/value pair is given!\n";
const char program_text[] = {('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), ('\xAB'), (0x01), (0x06), (0x01), (0x01), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x03'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x06), (0x10), (0x0C), (0x09), ('\xC2'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x01'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x01), (0x06), (0x06), (0x10), (0x0C), (0x09), ('\x8A'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00'), (0x02), ('\x11'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x05), (0x02), ('\x02'), ('\x00'), ('\x00'), ('\x00'), (0x0F), (0x11), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), ('\xFF'), (0x00), (0x02), ('\x00'), ('\x00'), ('\x00'), ('\x00')};
char program[sizeof(program_text)];

const char *init_program(int *argcptr,char *argv[])
{
  memcpy(program,program_text,sizeof(program_text));
   *((void **)(&program[0])) = argv;
   *((int **)(&program[12])) = argcptr;
   *((const char **)(&program[37])) = str0;
   *((const char **)(&program[68])) = str1;
   *((const char **)(&program[93])) = str2;
   *((const char **)(&program[124])) = str3;
   *((const char **)(&program[149])) = str4;
   *((const char **)(&program[166])) = str5;
  return program;
}
