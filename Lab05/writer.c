#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define BUFSIZE 256
#define SEGSIZE 4096

// control c signal handler
void exitProgram(int);

int main () {

  int shmId;
  char *shmPtr;

  char *buf = malloc(BUFSIZE * sizeof(char));

  // generate shared key
  key_t key = ftok("/tmp", 7);

  // install control c signal handler
  signal(SIGINT, exitProgram);

  // create shared M segment
  if ((shmId = shmget(key, SEGSIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
    perror ("i can't get no..\n");
    exit (1);
  }

  // attach shared M segment to data space
  if ((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
    perror ("can't attach\n");
    exit (1);
  }

  char *pointer1, *pointer2, *mainPointer;
  int memAddress[2], mem1, mem2;
  while(1) {

    // put input into shared M
    printf("Enter shared message: ");
    fgets(buf, BUFSIZE, stdin);

    // move pointer into shared data
    mainPointer = shmPtr + sizeof(memAddress);

    // initialize M address size
    memAddress[0] = 32;

    // return buffer string where pointer
    sprintf(mainPointer, buf);

    // move pointer M Address size
    mainPointer += memAddress[0];

    // save off first pointer
    pointer1 = mainPointer;

    // designate next M Address, with blank string
    memAddress[1] = sprintf(mainPointer, "1") + 1;

    // move pointer for next M Address
    mainPointer += memAddress[1];

    // save off second pointer
    pointer2 = mainPointer;

    // set pointer back to blank string
    sprintf(mainPointer, "1");

    // return shared memory pointer
    memcpy(shmPtr, &memAddress, sizeof(memAddress));

    // get input from user for first time
    sscanf(pointer1, "%d", &mem1);
    sscanf(pointer2, "%d", &mem2);

    // continue to get input from user
    while (mem1 && mem2) {
      sscanf(pointer1, "%d", &mem1);
      sscanf(pointer2, "%d", &mem2);
    }

  }

  // detach shared M segment
  if (shmdt (shmPtr) < 0) {
    perror ("just can't let go\n");
    exit (1);
  }

  // remove shared M segment
  if (shmctl (shmId, IPC_RMID, 0) < 0) {
    perror ("can't deallocate\n");
    exit(1);
  }

  exit(0);
}

void exitProgram(int sigNum) {
  printf("...writer all done for today.\n");
  exit(0);
}
