#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX 64

void sum(char bin1[], char bin2[], char result[]);

int main(){
  pid_t pid[3];
  int ab[2], bc[2], n;
  pipe(ab), pipe(bc);
  FILE *fp;
  char* buf = (char*)malloc(sizeof(char)*MAX);

  // Child A Complementor
  pid[0] = fork();
  if (pid[0] == 0){
    // Process A writes to process B
    dup2(ab[1], STDOUT_FILENO);
    // Essential: close all other pipes and copies of pipes
    for (n=0; n<2; n++){
      close(ab[n]);
      close(bc[n]);
    }

    if ((fp = fopen("8-input_B.dat", "r")) == NULL){
      perror("ERROR: fopen in Complementor Child\n");
      exit(1);
    }
    while (fgets(buf, MAX, fp) != NULL){
      // creates the complement of the binary representation as a string
      while (*buf){
        *buf++ ^= 1;
      }
      write(STDOUT_FILENO, (const void*)buf, (size_t)strlen(buf)+1);
    }
    // quit so your child doesn't end up in the main program
    free(buf);
    fclose(fp);
    exit(0);
  }

  // Child B Incrementor
  pid[1] = fork();
  if (pid[1] == 0){
    // Process B reads from process A
    dup2(ab[0], STDIN_FILENO);
    // Process B writes to process C
    dup2(bc[1], STDOUT_FILENO);
    // Essential: close all other pipes and copies of pipes
    for (n=0; n<2; n++){
      close(ab[n]);
      close(bc[n]);
    }

    int i;
    while (fgets(buf, MAX, STDIN_FILENO) != NULL){
      // adds 1 to make 2s Complement
      for (i = MAX-1; i > -1; i--){
        if (buf[i] == '\0'){  // ignore if NULL character
          continue;
        }
        if (buf[i] == '0'){    // flips the first 0
          buf[i] = '1';        // exits for loop
          break;
        } else {
          buf[i] = '0';        // flips any 1 to simulate adding
        }
      }
      write(STDOUT_FILENO, (const void*)buf, (size_t)strlen(buf)+1);
    }
    free(buf);
    exit(0);
  }

  // Child C Adder
  pid[2] = fork();
  if (pid[2] == 0){
    // Process C reads from process B
    dup2(bc[0], STDIN_FILENO);
    // Essential: close all other pipes and copies of pipes
    for (n=0; n<2; n++){
      close(ab[n]);
      close(bc[n]);
    }

    if ((fp = fopen("8-input_A.dat", "r")) == NULL){
      perror("ERROR: fopen in Adder Child\n");
      exit(1);
    }
    char* fbuf = (char*)malloc(sizeof(char)*MAX);
    char* result = (char*)malloc(sizeof(char)*MAX);
    while (fgets(fbuf, MAX, fp) != NULL){
      sum(buf, fbuf, result);
      printf("%s + %s = %s\n", buf, fbuf, result);
    }
    fclose(fp);
    free(result);
    free(fbuf);
    free(buf);
    exit(0);
  }

  // Only the parent will be running outside of those if statements.
  // Essential: close all other pipes and copies of pipes
  for(n=0; n<2; n++){
    close(ab[n]);
    close(bc[n]);
  }

  for (n=0; n<3; n++){
    int status;
    waitpid(pid[n], &status, 0);
    printf("Child %d exited with status %d\n", n, WEXITSTATUS(status));
  }

  return 0;
}

void sum(char bin1[], char bin2[], char result[]){
  int i = strlen(bin1) - 1;
  int j = strlen(bin2) - 1;
  int carry = 0, temp, num1, num2;

  while (i > -1 && j > -1){
    num1 = bin1[i] - '0';
    num2 = bin2[j] - '0';
    temp = num1 + num2 + carry;
    if (temp / 2 == 1){
      carry = 1;
      temp %= 2;
    }
    if (i > j){
      result[i + 1] = temp + '0';
      result[strlen(bin1) + 1] = '\0';
    } else {
      result[j +1] = temp + '0';
      result[strlen(bin2) + 1] = '\0';
    }
    i--;
    j--;
  }
  while (i > -1){
    temp = bin1[i] + carry - '0';
    if (temp / 2 == 1){
      carry = 1;
      temp %= 2;
    }
    result[i + 1] = temp + '0';
    i--;
  }
  while (j > -1){
    temp = bin2[j] + carry - '0';
    if (temp / 2 == 1){
      carry = 1;
      temp %= 2;
    }
    result[j + 1] = temp + '0';
    j--;
  }
  if (carry){
    result[0] = '1';
  } else {
    result[0] = '0';
  }
}
