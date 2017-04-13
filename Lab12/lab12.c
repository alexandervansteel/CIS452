#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]) {

   if (argc < 2) {
	   fprintf(stderr, "Usage: %s <pathname> <flags>\n", argv[0]);
	   exit(EXIT_FAILURE);
   }

   DIR *dirPtr;
   struct dirent *entryPtr;

	// invalid directory
   dirPtr = opendir(argv[1]);
	if (dirPtr == NULL) {
		printf("Invalid directory: %s\n", argv[1]);
	    exit(EXIT_FAILURE);

	}

   printf("Looking at directory: %s\n", argv[1]);

   while ((entryPtr = readdir(dirPtr))) {

	   struct stat sb;
	   char myFileName[1000];

	   // once we know we have valid directory
	   // get file names for directory
	   strcpy(myFileName, argv[1]);
	   strcat(myFileName, "/");
	   strcat(myFileName, entryPtr->d_name);

	   // bad file name
	   if (stat(myFileName, &sb) == -1) {
	   	   perror("stat");
	   	   exit(EXIT_FAILURE);
	   }

	   // no flags -- display only name
	   if (argc == 2) {
	   		printf("%s  ",entryPtr->d_name);
		}

	   // display only file name and size
	   else if (strcmp(argv[2], "-l") == 0) {

			printf("%s\t\t\t%lld bytes\n", entryPtr->d_name,
			   (long long) sb.st_size);
	   }

		// display only file name and inode
		else if (strcmp(argv[2], "-i")== 0) {
			printf("%s\t\t\tinode: %ld \n", entryPtr->d_name,
			   (long) sb.st_ino);
		}


		// all code obtained from stat() example on man page
		else if (strcmp(argv[2], "-all") == 0) {

		   printf("File name: %s\n",entryPtr->d_name);

		   printf("File type:                ");

		   switch (sb.st_mode & S_IFMT) {
		   case S_IFBLK:  printf("block device\n");            break;
		   case S_IFCHR:  printf("character device\n");        break;
		   case S_IFDIR:  printf("directory\n");               break;
		   case S_IFIFO:  printf("FIFO/pipe\n");               break;
		   case S_IFLNK:  printf("symlink\n");                 break;
		   case S_IFREG:  printf("regular file\n");            break;
		   case S_IFSOCK: printf("socket\n");                  break;
		   default:       printf("unknown?\n");                break;
		   }

		   printf("I-node number:            %ld\n", (long) sb.st_ino);

		   printf("Mode:                     %lo (octal)\n",
				   (unsigned long) sb.st_mode);

		   printf("Link count:               %ld\n", (long) sb.st_nlink);
		   printf("Ownership:                UID=%ld   GID=%ld\n",
				   (long) sb.st_uid, (long) sb.st_gid);

		   printf("Preferred I/O block size: %ld bytes\n",
				   (long) sb.st_blksize);
		   printf("File size:                %lld bytes\n",
				   (long long) sb.st_size);
		   printf("Blocks allocated:         %lld\n",
				   (long long) sb.st_blocks);

		   printf("Last status change:       %s", ctime(&sb.st_ctime));
		   printf("Last file access:         %s", ctime(&sb.st_atime));
		   printf("Last file modification:   %s\n", ctime(&sb.st_mtime));
		}
	}
	printf("\n");
	exit(EXIT_SUCCESS);
}
