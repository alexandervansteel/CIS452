#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define BUFF_LEN 1024

int main() {
  char line[BUFF_LEN];  // command line
  char* argv[100];      // user command
  char filepath[20];    // full file path
  int argc;             // arg count

  while (1) {
    printf("SimpleShell>> ");

    if (!fgets(line, BUFF_LEN, stdin))
      break;

    size_t len = strlen(line);
    if (line[len - 1] == "\n")
      line[len - 1] = "\0";

    if (strcmp(line, "exit") == 0)
      break;

  }
};
