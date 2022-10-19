#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	sem_t sem;
	int value;
	int wakeup;
} sems_t;

int sems_wait(sems_t *sems, int t, int d)
{
	while (sems->value < t)
	{
		sems->wakeup++;
		sem_wait(&(sems->sem));
	}
	sems->value -= d;
	return 0;
}

int sems_signal(sems_t *sems, int d)
{
	sems->value += d;
	while (sems->wakeup > 0)
	{
		sems->wakeup--;
		sem_post(&(sems->sem));
	}
	return 0;
}

void sems_init(sems_t *sems, int t)
{
	sem_init(&sems->sem, 0, 0);
	sems->value = t;
	sems->wakeup = 0;
}