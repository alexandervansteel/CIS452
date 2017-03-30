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
#include <pthread.h>    // compile with -lpthread
#include <curses.h>     // compile with -lcruses

int pop_dur;            // show duration for moles
int hide_dur;           // hide duration for moles

struct mole_settings {
  int line;             // y position on screen
  int column;           // x position on screen
};

// thread for the moles
void* mole(void *param){
  struct mole_settings *settings = param;
  int line = settings->line;
  int column = settings->column;
  move(column,line);
  printw("a");
  return 0;
}

int main(int argc, char *argv[]){
  int shmid, status;
  key_t key;
  char *shm, *s;
  int grid_width, grid_height, num_moles, max_moles, pop_dur, hide_dur;
  pthread_t *thread;
  struct mole_settings *settings = malloc(sizeof(struct mole_settings));
  char input;
  key = 9876;

  if (argc != 7){
    printf("Incorrect Arguments: ./proj3 grid_width grid_height num_moles max_moles pop_dur hide_dur \n");
    exit(1);
  } else {
    grid_width = atoi(argv[1]);
    grid_height = atoi(argv[2]);
    num_moles = atoi(argv[3]);
    max_moles = atoi(argv[4]);
    pop_dur = atoi(argv[5]);
    hide_dur = atoi(argv[6]);
  }

  thread = malloc(max_moles*sizeof(pthread_t));

  printf("grid_height: %d\n", grid_height);
  // init the game space
  initscr();
  noecho();
  move(grid_height+1,0);
  printw("Welcome to a game of Whack-a-Mole! Press ESC to exit the game.");
  refresh();

  // create game board area and spawn moles into each slot
  int i, j;
  for (i = 0; i < grid_width; i++){
    for (j = 0; j < grid_height; j++){
      settings->column = j;
      settings->line = i;
      if ((status = pthread_create(thread, NULL, mole, (void*)settings)) != 0){
        fprintf(stderr, "thread create error %d: %s\n",
        status, strerror(status));
        exit(1);
      }
    }
  }

  while ((input = getchar()) != 0x1B){
    // TODO: add game logic for main here
    move(grid_height+2,0);
    printw("Character Entered: %c", input);
    refresh();
  }

  endwin();
  return 0;
}
