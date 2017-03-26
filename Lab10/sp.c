#include <stdio.h>
#include <stdlib.h>

#define KB 1024
#define LOOP 200

int main()
{
    int count, *intPtr;

    long int i, j, dim = 4 * KB;

    printf("size of int: %d\n", sizeof(int));
    printf("size of malloc1: %d\n", dim*dim*sizeof(int));
    printf("size of malloc2: %d\n", dim*dim*sizeof(int)/1024);

    if ((intPtr = malloc(dim * dim * sizeof(int))) == 0) {
	    perror("totally out of space");
	    exit(1);
    }
    for (count = 1; count <= LOOP; count++)
	    for (i = 0; i < dim; i++)
	        for (j = 0; j < dim; j++)
		        intPtr[i * dim + j] = (i + j) % count;

    free(intPtr);
    return 0;
}
