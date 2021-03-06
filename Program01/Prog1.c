#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

//#define _BSD_SOURCE

int main () {
  struct passwd *pw;
  pw = getpwnam(getlogin());
  if (pw == NULL) {
    printf("\nERROR: Failed to get current user name.\n");
    exit(1);
  }

  printf("\nUser is: %s\n", pw->pw_name);
  printf("UID is: %d\n", pw->pw_uid);
  printf("GID is: %d\n", pw->pw_gid);

  char host[1024];
  int result = gethostname(host, 1024);
  if (result != 0) {
    printf("\nERROR: Failed to get host name.\n");
    exit(1);
  }
  printf("Host is: %s\n", host);

  struct passwd* p = NULL;
  setpwent();
  int i;
  for (i = 0; i < 5; i++) {
    if ((p = getpwent()) != NULL) {
      printf("entry %d: %s\n", i, p->pw_name);
    }
  }
  endpwent();

  char env_var[1024];
  printf("\nEnvironment variable desired: ");
  scanf("%s", env_var);
  printf("\nValue is: %s\n\n", getenv(env_var));

  return 1;
}
