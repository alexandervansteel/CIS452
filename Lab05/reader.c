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

  int shmId, *p, p1;
  char *shmPtr, *m_ptr, *reader[2];

  // install control c signal handler
  signal(SIGINT, exit_program);

  // generate shared key
  key_t key = ftok("/tmp", 7);

  // create shared M segment
  if ((shmId = shmget(key, SEGSIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
    perror("ERROR: shmget\n");
    exit(1);
  }

  // attach shared M segment to data space
  if ((shmPtr = shmat(shmId, 0, 0)) == (void*) -1){
    perror("ERROR: shmat\n");
    exit(1);
  }


  while (1){

    // get copy of pointer to shared M space
    p = (int*)shmPtr;

    // get next place in shared M space
    // writer should be pointing here
    m_ptr = shmPtr + sizeof(int) * 2;

    // save off this pointer
    reader[0] = m_ptr;

    // print output from where writer was pointing
    printf("Message Received: %s\n", reader[0]);

    // pointer arithmetic to move to next spot in M
    m_ptr += *p++;

    // save off this pointer
    reader[1] = m_ptr;

    //
    sprintf(m_ptr, "0");

    // must get from writer first time
    sscanf(m_ptr, "%d", &p1);

    // as long shared M still exists and writer still wants to go
    while (!p1){
      sscanf(m_ptr, "%d", &p1);
    }

  }

  // detach shared M segment
  if (shmdt(shmPtr) < 0){
    perror ("ERROR: shmdt\n");
    exit (1);
  }

  // remove shared M segment
  if (shmctl(shmId, IPC_RMID, 0) < 0){
    perror ("ERROR: shmctl\n");
    exit(1);
  }

  exit(0);
}

void exit_program(int sigNum){
  printf("Closing reader process.\n");
  exit(0);
}
