/*
* Alexander Vansteel
* CIS 452
* Project 3
* Whack-a-Mole
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/icp.h>
#include <sys/shm.h>
#include <pthread.h>

#define SHSIZE 100;     // size of shared memory

// thread for the moles
void *mole(void *param){

}

int main(int argc, char *argv[]){
  int shmid;
  key_t key;
  char *shm, *s;
  int grid_width, grid_height, num_moles, max_moles, pop_dur, hide_dur;
  pthread_t thread;

  key = 9876;

  if (argc != 7){
    printf("Incorrect Arguments: ./proj3 grid_width grid_height num_moles max_moles pop_dur hide_dur \n", );
    exit(1);
  } else {
    grid_width = atoi(argv[1]);
    grid_height = atoi(argv[2]);
    num_moles = atoi(argv[3]);
    max_moles = atoi(argv[4]);
    pop_dur = atoi(argv[5]);
    hide_dur = atoi(argv[6]);
  }
  // create shared memory space
  if ((shmid = shmget(key, SHSIZE, IPC_CREATE | 0666)) < 0){
    perror("shmget\n");
    exit(1);
  }

  // attatch shared memory
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1){
    perror("shmat\n");
    exit(1);
  }

  // create the moles
  int i;
  for (i = 0; i < max_moles; i++){
    if ((status = pthread_create(&thread, NULL, mole, NULL)) != 0) {
        fprintf(stderr, "thread create error %d: %s\n",
          status, strerror(status));
        exit(1);
      }
  }


  // detach shared M segment
  if (shmdt(shm) < 0){
    perror("shmdt\n");
    exit(1);
  }

  // remove shared M segment
  if (shmctl(shmid, IPC_RMID, 0) < 0){
    perror("shmctl\n");
    exit(1);
  }
  return 0;
}
