#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int values;
    int wakeup;
    int wait;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}sem_s;

void sem_s_init(sem_s *sem)
{
    pthread_mutex_init(&(sem->mutex),NULL);
    pthread_cond_init(&(sem->cond),NULL);
}

void P(sem_s *sem,int t,int d)
{
    pthread_mutex_lock(&(sem->mutex));
    // sem->values -= d;
    // if (sem->values<t)
    // {
    //     do
    //     {
    //         pthread_cond_wait(&(sem->cond), &(sem->mutex));
    //     } while (sem->wakeup==0);
    //     sem->wakeup -=d;
    // }
    while (sem->values-d<t)
    {
        sem->wakeup++;
        pthread_cond_wait(&(sem->cond),&(sem->mutex));
    }
    
    pthread_mutex_unlock(&(sem->mutex));
}

void Ssignal(sem_s *sem,int d)
{
    pthread_mutex_lock(&(sem->mutex));
    sem->values += d;
    if (sem->wait>0)
    {
        pthread_cond_broadcast(&(sem->cond));
        sem->wakeup=0;
    }
    pthread_mutex_unlock(&(sem->mutex));
}