#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct
{
	sem_t sem;
	int value;
	int waits;
}sems_t;

int P(sems_t *sems, int t, int d){
	if(sems->value < t){
		sems->waits++;
		sem_wait(&(sems->sem));
	}
	sems->value -= d;
	return 0;
}

int V(sems_t *sems, int d){
	sems->value += d;
	if(sems->waits > 0){
		sems->waits--;
		sem_post(&(sems->sem));
	}
	return 0;
}

void sem_s_init(sems_t *sems, int t)
{
	sem_init(&sems->sem, 0, 0);
	sems->value = t;
	sems->waits = 0;
}