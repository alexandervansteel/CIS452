#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//#define MAX 64

void sum(char bin1[], char bin2[], char result[]);
void exit_program(int);


int main(){
  pid_t pid[3];
  int ab[2], bc[2], n;
  pipe(ab), pipe(bc);
  
  // handle ^C
  signal(SIGINT, exit_program);
  
  int MAX;
  printf("Enter the byte size: ");
  scanf("%d", &MAX);
  MAX++;
  
  
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
    
    FILE* fp;
    char* buf = malloc(sizeof(char)*MAX);
    
    if ((fp = fopen("8-input_B.dat", "r")) == NULL){
      perror("ERROR: fopen in Complementor Child\n");
      exit(1);
    }
    while (fgets(buf, MAX, fp) != NULL){
      if (strlen(buf) == MAX-1){
        strtok(buf, "\n");    // remove trailing \n from fgets
        //fprintf(stderr, "From file in child a: %s\n", buf);
        
        // creates the complement of the binary representation as a string
        char* s = buf;
        while (*s){
          *s++ ^= 1;
        }
        
        //fprintf(stderr, "Sending from child a: %s\n", buf);
        
        write(STDOUT_FILENO, (const void*)buf, (size_t)strlen(buf));
      }
      memset(buf, '\0', strlen(buf));
    }
    free(buf);
    fclose(fp);
    close(ab[1]);
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
    
    char* buf = (char*)malloc(sizeof(char)*MAX);
    
    int i;
    while (fgets(buf, MAX, stdin) != NULL){
      //strtok(buf, "\n");    // remove trailing \n from fgets
      if (strlen(buf) == MAX-1){
        strtok(buf, "\n");
        //fprintf(stderr, "Comming into child b: %s\n", buf);
        
        // adds 1 to make 2s Complement
        for (i = strlen(buf); i >= 0; i--){
          if (buf[i] == '\0'){  // ignore if NULL character
            continue;
          }
          if (buf[i] == '0'){
            buf[i] = '1';        // flips first 0
            break;               // exits for loop
          } else {
            buf[i] = '0';        // flips any 1 to simulate adding
          }
        }
        
        //fprintf(stderr, "Sending to child c: %s\n", buf);
        
        write(STDOUT_FILENO, (const void*)buf, (size_t)strlen(buf));
      }
      memset(buf, '\0', MAX);
    }
    free(buf);
    close(ab[0]);
    close(bc[1]);
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
    
    int carry, temp, j, num1, num2;
    FILE* fp;
    if ((fp = fopen("8-input_A.dat", "r")) == NULL){
      perror("ERROR: fopen in Adder Child\n");
      exit(1);
    }
    char* buf = (char*)malloc(sizeof(char)*MAX);
    char* fbuf = (char*)malloc(sizeof(char)*MAX);
    char* line = (char*)malloc(sizeof(char)*MAX + 2);
    memset(line, '-', MAX+2);
    //char* result = (char*)malloc(sizeof(char)*MAX);
    while (fgets(buf, MAX, stdin) != NULL){
      if (strlen(buf) == MAX-1){
      strtok(buf, "\n");
      fprintf(stderr, "  %s\n", buf);
      
      // load number in from vector table
      while (strlen(fbuf) != MAX){
        if (fgets(fbuf, MAX+1, fp) == NULL){
          break;
        }
      }
      strtok(fbuf, "\n");
      fprintf(stderr, "+ %s\n", fbuf);
      
      carry = 0;
      for(j = MAX -1; j >= 0; j--){
        // Convert 1 char from each array to int
        num1 = fbuf[j] - '0';
        num2 = buf[j] - '0';
        
        // Add A + B + Carry
        temp = num1 + num2 + carry;
        
        // if sum is 3 or 2 there is a carry
        // else there is no carry
        if (temp == 3 || temp == 2){
          carry = 1;
          
          // temp mod 2 to get the result for this place
          // 1 when temp is 3, 0 when temp is 2
          temp %= 2;
        } else if(temp == 1 || temp == 0){
          carry = 0;
        }
        
        // convert temp back to the proper char and set result[j]
        buf[j] = temp + '0';
      }
        strtok(buf, "\0");
      fprintf(stdout, "%s\n  %s\n\n", line, buf);
    }
      memset(buf, '\0', MAX);
      memset(fbuf, '\0', MAX);
    }
    fclose(fp);
    free(line);
    free(fbuf);
    free(buf);
    close(bc[0]);
    exit(0);
  }
  
  // Parent
  // close all other pipes and copies of pipes
  for(n=0; n<2; n++){
    close(ab[n]);
    close(bc[n]);
  }
  
  // report close status
  for (n=0; n<3; n++){
    int status;
    waitpid(pid[n], &status, 0);
    //printf("Child %d exited with status %d\n", n, WEXITSTATUS(status));
  }
  
  return 0;
}

void exit_program(sig_num){

}
