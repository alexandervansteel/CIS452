#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void sig_handler(int);

void sig_handler(int sigNum){
  if (sigNum == SIGUSR1) {
    printf("received a SIGUSR1 signal.\n");
  }
  else if (sigNum == SIGUSR2) {
    printf("received a SIGUSR2 signal.\n");
  }
  else if (sigNum == SIGINT) {
    printf(" received.\nThat's it, I'm shutting you down... gracefully!\n");
    if (kill(getpid(), SIGKILL) < 0) {
      perror("failed to kill child\n");
      exit(1);
    }
    exit(0);
  }
}

int main() {
  pid_t pid;
  signal(SIGUSR1, sig_handler);
  signal(SIGUSR2, sig_handler);

  if ((pid = fork()) < 0) {
    perror("fork failed\n");
    exit(1);
  }

  if (pid == 0) { // child
    pid_t ppid = getppid();
    while (1) {
      sleep(rand()%5+1);
      if (rand() % 2 != 0) {
        if (kill(ppid, SIGUSR1) < 0) {
          perror("kill SIGUSR1 failed\n");
          exit(1);
        }
      } else {
        if (kill(ppid, SIGUSR2) < 0) {
          perror("kill SIGUSR2 failed\n");
          exit(1);
        }
      }
    }
  } else {    // parent
    printf("Spawned child PID# %d\n", pid);
    signal(SIGINT, sig_handler);
    while (1) {
      printf("waiting...\t");
      fflush(stdout);
      pause();
    }
  }
  return 0;
}
