/*
 * Lab 01(09) #7
 * by Andrew Burns and Alexander Vansteel
 * 3/23/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>

#define SIZE 16

int main()
{
    mtrace();
	char *data1, *data2;
	int k;
	do {
    	data1 = malloc(SIZE);
    	printf ("Please input your EOS username: ");
    	scanf ("%s", data1);
    	if (! strcmp (data1, "quit")){
   		 free(data1);    //when user wants to quit, data1 needs to be freed
        	break;}
    	data2 = malloc(SIZE);
    	for (k = 0; k < SIZE; k++)
        	data2[k] = data1[k];
    	free (data1);
    	printf ("data2 :%s:\n", data2);
   	 free (data2);    //data2 was never freed in the original code
	} while(1);
    muntrace();
	return 0;
}
