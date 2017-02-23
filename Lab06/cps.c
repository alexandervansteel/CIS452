#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16
int main (int argc, char *argv[]){
  int status;
  long int i, loop, temp, *shmPtr;
  int shmId;
  pid_t pid;

  // get value of loop variable (from command-line argument)
  if (argc < 2){
    printf("Usage: program loopVar");
  } else {
    loop = atoi(argv[1]);
  }

  if ((shmId = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
    perror("i can't get no..\n");
    exit(1);
  }
  if ((shmPtr = shmat(shmId, 0, 0)) == (void*) -1){
    perror("can't attach\n");
    exit(1);
  }

  shmPtr[0] = 0;
  shmPtr[1] = 1;

  int semId;

  // create semaphore
  if ((semId = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
    perror("Uh oh...something went wrong creating semaphore");
    exit(1);
  }

  // intialize semaphore to 1
  if (semctl(semId, 0, SETVAL, 1) < 0){
    perror("Uh oh...something went wrong initializing semaphore");
    exit(1);
  }

  // "wait-for-zero": signals process is entering critical section
  struct sembuf wait_operation;
  wait_operation.sem_num = 0;
  // wait operation decrements semval to 0
  wait_operation.sem_op = -1;

  // signals process has exited critical section
  struct sembuf signal_operation;
  signal_operation.sem_num = 0;
  // signal operation increments semval back to 1
  signal_operation.sem_op = 1;


  if (!(pid = fork())){
    for (i=0; i<loop; i++){

      // entering critical section
      semop(semId, &wait_operation, 1);

      // swap the contents of shmPtr[0] and shmPtr[1]
      temp = shmPtr[0];
      shmPtr[0] = shmPtr[1];
      shmPtr[1] = temp;

      // exiting critical section
      semop(semId, &signal_operation, 1);

    }
    if (shmdt(shmPtr) < 0){
      perror("just can't let go\n");
      exit(1);
    }
    exit(0);
  } else {
    for (i=0; i<loop; i++){

      // entering critical section
      semop(semId, &wait_operation, 1);

      // swap the contents of shmPtr[1] and shmPtr[0]
      temp = shmPtr[0];
      shmPtr[0] = shmPtr[1];
      shmPtr[1] = temp;

      // exiting critical section
      semop(semId, &signal_operation, 1);
    }
  }

  wait(&status);
  printf("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

  if (shmdt(shmPtr) < 0) {
    perror("just can't let go\n");
    exit(1);
  }
  if (shmctl(shmId, IPC_RMID, 0) < 0) {
    perror ("can't deallocate\n");
    exit(1);
  }

  //  remove the semaphore referenced by semId
  semctl(semId, 0, IPC_RMID);

  return 0;
}
