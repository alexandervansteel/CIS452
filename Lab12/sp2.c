#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
   DIR *dirPtr;
   struct dirent *entryPtr;
   struct stat st;
   int size;

   dirPtr = opendir (".");

   while ((entryPtr = readdir (dirPtr)))
   {
      stat(entryPtr->d_name, &st);
      size = st.st_size;
      printf ("%s\t%d\n", entryPtr->d_name, size);
   }
   closedir (dirPtr);
   return 0;
}
