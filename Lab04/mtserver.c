#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MAXBUF 255         // max buffer size
#define MAXTHREADS 4000    // max number of threads

int f_num = 0;             // number of files accessed
int t_time = 0;              // total time
pthread_mutex_t mutex_time; // access to time

void* dispatcher_exec(void* arg);
void* worker_exec(void* arg);
void print_status(int);

int main(){
  srand(time(NULL));
  pthread_mutex_init(&mutex_time, NULL);

  int status;

  pthread_t dispatch;

  if((status = pthread_create(&dispatch, NULL, dispatcher_exec, NULL)) != 0){
    printf("Error dispatching pthread.\n");
    exit(1);
  }

  if ((status = pthread_join(dispatch, NULL)) != 0){
    printf("Error joining pthread.\n");
    exit(1);
  }

  pthread_mutex_destroy(&mutex_time);

  return 0;
}

void* dispatcher_exec(void* arg){
  // signal handler for ^C
  signal(SIGINT,print_status);

  int buf_index = 0;

  while(1){
    if (buf_index == MAXTHREADS){
      buf_index = 0;
    }

    char* buf = malloc(sizeof(char)*MAXBUF);
    printf("Enter file name: ");
    if (fgets(buf, MAXBUF, stdin) == NULL){
      perror("Error reading input\n");
      exit(1);
    }

    buf = realloc(buf, sizeof(char)*strlen(buf));

    // create worker thread
    int status;
    pthread_t worker;
    if ((status = pthread_create(&worker,NULL,worker_exec,(void*)buf)) != 0){
      fprintf(stderr, "Error creating worker pthread %d: %s\n", status, strerror(status));
      exit(1);
    }

    // detach worker thread and continue dispatch
    if ((status = pthread_detach(worker)) != 0){
      fprintf(stderr, "Error detaching worker pthread %d: %s\n", status, strerror(status));
      exit(1);
    }

    f_num++;
    buf_index++;
  }
  return arg;
}

void* worker_exec(void* arg){
  char* f_name = (char*) arg;
  int w_time = 0;

  // 20% probability
  if ((rand()%5) == 0){
    w_time = rand()%4+7;
    sleep(w_time);
    printf("\nFile %s retrieved:\t%d\n", f_name, w_time);
  } else {
    w_time = 1;
    sleep(w_time);
    printf("\nFile %s retrieved:\t%d\n", f_name, w_time);
  }
  free(f_name);

  // lock time to ensure only one thread modifies it at a time
  pthread_mutex_lock(&mutex_time);
  t_time += w_time;
  pthread_mutex_unlock(&mutex_time);

  return NULL;
}

void print_status(int sig_num){
  printf("\nFiles Found:\t%d\n", f_num);
  float avg_time = (float)t_time/(float)f_num;
  printf("Average Time:\t%f sec\n", avg_time);
  exit(0);
}
