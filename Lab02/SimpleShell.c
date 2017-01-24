#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_LEN 1024

int main() {
  char line[BUFF_LEN];  // command line
  char* argv[100];      // user command
  char filepath[20];    // full file path
  char* path = "/bin/"; // set path to bin
  int argc;             // arg count

  while (1) {
    printf("SimpleShell>> ");

    if (!fgets(line, BUFF_LEN, stdin))
      break;

    size_t len = strlen(line);
    if (line[len - 1] == '\n')
      line[len - 1] = '\0';
    if (strcmp(line, "exit") == 0)
      break;

    char *token;
    token - strtok(line, ' ');
    int i = 0;
    while (token != NULL) {
      argv[i] = token;
      token = strtok(NULL, ' ');
      i++;
    }
    argv[i] = NULL;

    argc = i;
    for (i = 0; i < argc; i++) {
      printf("%s\n", argc[i]);
    }
    strcpy(filepath, path);
    strcat(filepath, argv[0]);

    for (i = 0; i < strlen(filepath); i++) {
      if (filepath[i] == '\n')
        filepath[i] = '\0';
    }

    int pid = fork();

    if (pid == 0) {
      execvp( filepath, argv);
      fprintf(stderr, "Child process could not execvp\n");
    } else {
      wait(NULL);
      printf("Child exited\n");
    }
  }
};
