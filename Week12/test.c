/*
  (C) Nguyen Ba Ngoc
  A minimal test runner tool
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *cases[] = {
  "./prog p1k.txt p1k-o10.txt",
  "./prog p10k.txt p10k-o100.txt",
  "./prog p100k.txt p100k-o1k.txt",
  "./prog p1m.txt p1m-o10k.txt",
  "valgrind ./prog p1k.txt p1k-o10.txt",
  "valgrind ./prog p10k.txt p10k-o100.txt",
  "valgrind ./prog p100k.txt p100k-o1k.txt",
  "valgrind ./prog p1m.txt p1m-o10k.txt"
};

char *real_command(const char *cmd) {
  static char buff[1024];
  strcpy(buff, "/usr/bin/time -f \"\\tTime: %E real,\\tMemory: %M Kb\" ");
  strcat(buff, cmd);
  return buff;
}

int main() {
  int n = sizeof(cases)/sizeof(cases[0]);
  for (int i = 0; i < n; ++i) {
    const char *command = real_command(cases[i]);
    printf("case %d: %s\n", i + 1, cases[i]);
    system(command);
    printf("Press Enter to continue...");
    while (getchar() != '\n') ;
  }
  return 0;
}