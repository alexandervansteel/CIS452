#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  pid_t pid[3];
  int ab[2], bc[2];
  pipe(ab), pipe(bc), pipe(ca);

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

    // do whatever you want
    ...
    // quit so your child doesn't end up in the main program
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

    // do whatever you want
    ...
    // quit so your child doesn't end up in the main program
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

    // do whatever you want
    ...
    // quit so your child doesn't end up in the main program
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
