#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t pizzaLock = PTHREAD_MUTEX_INITIALIZER;
int pizzaID=0;
int availablePizza=10;
int availableSlices=0;
int studentID=0;
int waitingStudentID=0;
int numStudent=0;
bool done = false;



int getStudent()
{
	pthread_mutex_lock(&lock);
	studentID++;
	pthread_mutex_unlock(&lock);		
	return studentID;
}

void *orderPizza(void *arg)
{
	printf("Seasons pizza open\n");
	while(availablePizza>0)
	{
		// check if we have any pizza slices left
		if(availableSlices==0 && waitingStudentID>0)
			{
				
				printf("Student %d ordered pizza %d\n",waitingStudentID,pizzaID+1);
				//if slices reach 0, order new pizza
				pthread_mutex_lock(&pizzaLock);
				availablePizza--;
				pizzaID++;
				if(availablePizza>=0)
				{
					availableSlices=10;
					sleep(5);
					printf("Pizza %d delivered\n", pizzaID);
					waitingStudentID=0;
					
				}	
				pthread_mutex_unlock(&pizzaLock);
			}
		sleep(1);
		//pthread_self(NULL);
		
	}
	sleep(5);
	printf("Seasons pizza closed\n");


}

void studentGetSliceOfPizza(int _studentID)
{

	while(!done && availableSlices==0 && waitingStudentID==0)
	{
		printf("Student %d waiting for pizza\n",_studentID);
		pthread_mutex_lock(&pizzaLock);
		if(waitingStudentID==0)
		{
			
			waitingStudentID=_studentID;
		}
		pthread_mutex_unlock(&pizzaLock);
		sleep(1);
	}
	
	if(availableSlices>0)
	{
		pthread_mutex_lock(&pizzaLock);
		availableSlices--;
		pthread_mutex_unlock(&pizzaLock);
		printf("Student %d eats slice  %d\n ",_studentID,10 - availableSlices);
	}
}


void *studyWhileEatingPizza(void *arg)
{
	int _studentID = getStudent();
	printf("Student %d begin studying\n", _studentID);
	
	while(!done)
	{
		sleep(1);
		studentGetSliceOfPizza(_studentID);
		// check if no more pizza is left
		pthread_mutex_lock(&lock);
		done = availablePizza == 0 && availableSlices == 0;
		pthread_mutex_unlock(&lock);		
	}
	printf("Student %d finished studying\n", _studentID);
}

int main(int argc, char *argv[])
{
	
	
	// command line argument here
	numStudent=atoi(argv[1]);
	
	// create a group of student threads equal to numStudent
	pthread_t *student_group = malloc(sizeof(pthread_t) * numStudent);

    	// start all threads to begin work
	    for (int i = 0; i < numStudent; ++i) 
		{
       			pthread_create(&student_group[i], NULL, studyWhileEatingPizza, NULL);
    		}

	// create a seasons pizza thread
	pthread_t pizza_thread;
	pthread_create(&pizza_thread,NULL, orderPizza, NULL);
	
	printf("Student study session started\n");




	
	printf("Student study session ended\n");

	pthread_join(pizza_thread,NULL);
	
    	// wait for all threads to finish
	    for (int i = 0; i < numStudent; ++i) 
	{
        	pthread_join(student_group[i], NULL);
   	}

}
