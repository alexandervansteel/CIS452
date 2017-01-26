/*
 * Alexander Vansteel
 * Winter 2017
 * CIS 452 Lab 02
 * Prof Hans Dulimarta
 * SimpleShell
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/wait.h>
 #include <sys/resource.h>
 #include <sys/time.h>
 #include <sys/resource.h>

 #define MAXBUF 1024
 #define MAXARGS 256

 int main() {
   char buf[MAXBUF];
   char* args[MAXARGS];
   char* token;
   pid_t pid;
   int status = 0;

   while (1) {
     printf("SimpleShell>> ");
     if (fgets(buf, MAXBUF, stdin) == NULL) {
       printf("ERROR: Failed to get command.\n");
     }
     buf[strcspn(buf, "\n")] = 0;
     if (strcmp(buf, "quit") == 0) {
       exit(0);
     }

     token = strtok(buf, " ");
     int i = 0;
     while (token != NULL) {
       char* arg = malloc(sizeof(*arg) * (strlen(token) + 1));
       strcpy(arg, token);
       args[i] = arg;
       token = strtok(NULL, " ");
       i++;
     }
     args[i] = NULL;

     pid = fork();
     if (pid < 0) {
       printf("ERROR: Failed to fork.\n");
     }

     else if (pid) {
       struct rusage res;
       wait3(&status, 0, &res);

       struct timeval uTime = res.ru_utime;
       time_t uTimeSec = uTime.tv_sec;
       long int uTimeMicroSec = uTime.tv_usec;
       printf("User CPU Time Usage: %ld %ldus\n", uTimeSec, uTimeMicroSec);

       long int ics = res.ru_nivcsw;
       printf("Involuntary Context Switches: %ls\n", ics);
     }

     else {
      if (execvp(args[0], args) < 0) {
        printf("ERROR: Failed to create child.\n");
      }
      exit(status);
     }
  }
  return 0;
}
