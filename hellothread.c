#include<stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

void *printHelloThread(void* p)
{

	printf("Creating thread # %d\n",pthread_self());
	sleep(2);
	printf("Hello World! I am thread # %d\n",pthread_self());

	
	return NULL;
}

int main(int argc, char *argv[])
{
	int n;

	//convert string type to integer type
	
	n=atoi(argv[1]);
	//thread id
	pthread_t threadid[n];
	
	//arrThreads[]

	//step 1: check the parameter to ensure the integer is between 1-10
	// if not it prints an error message and quits.
	if( n <1 || n > 10)
	{
		printf("Invalid\nEnter number between 1 and 10\n");
	}
	
	else
	{
		for(int i =1; i<=n; i++)
		{
		pthread_create ( &threadid[i], NULL, printHelloThread, &i);
		pthread_join(threadid,NULL);
		
		}
			
	}
        pthread_exit(NULL);

	return 0;

}



