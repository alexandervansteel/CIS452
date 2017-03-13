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

// ^C signal handler
void exit_program(int);

int main (){

  int shmId;
  char *shmPtr;

  char *buf = malloc(BUFSIZE * sizeof(char));

  // generate shared key
  key_t key = ftok("/tmp", 7);

  // install control c signal handler
  signal(SIGINT, exit_program);

  // create shared M segment
  if ((shmId = shmget(key, SEGSIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
    perror("ERROR: shmget\n");
    exit(1);
  }

  // attach shared M segment to data space
  if ((shmPtr = shmat(shmId, 0, 0)) == (void*) -1){
    perror ("ERROR: shmat\n");
    exit(1);
  }

  char *ptr1, *ptr2, *m_ptr;
  int mem_addr[2], mem1, mem2;
  while (1){

    // put input into shared M
    printf("Enter shared message: ");
    fgets(buf, BUFSIZE, stdin);

    // move pointer into shared data
    m_ptr = shmPtr + sizeof(mem_addr);

    // initialize M address size
    mem_addr[0] = 32;

    // return buffer string where pointer
    sprintf(m_ptr, "%s", buf);

    // move pointer M Address size
    m_ptr += mem_addr[0];

    // save off first pointer
    ptr1 = m_ptr;

    // designate next M Address, with blank string
    mem_addr[1] = sprintf(m_ptr, "1") + 1;

    // move pointer for next M Address
    m_ptr += mem_addr[1];

    // save off second pointer
    ptr2 = m_ptr;

    // set pointer back to blank string
    sprintf(m_ptr, "1");

    // return shared memory pointer
    memcpy(shmPtr, &mem_addr, sizeof(mem_addr));

    // get input from user for first time
    sscanf(ptr1, "%d", &mem1);
    sscanf(ptr2, "%d", &mem2);

    // continue to get input from user
    while (mem1 && mem2){
      sscanf(ptr1, "%d", &mem1);
      sscanf(ptr2, "%d", &mem2);
    }

  }

  // detach shared M segment
  if (shmdt(shmPtr) < 0){
    perror("ERROR: shmdt\n");
    exit(1);
  }

  // remove shared M segment
  if (shmctl(shmId, IPC_RMID, 0) < 0){
    perror("ERROR: shmctl\n");
    exit(1);
  }

  exit(0);
}

void exit_program(int sigNum){
  printf("Closing writer process.\n");
  exit(0);
}
