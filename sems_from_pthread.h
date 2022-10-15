#include <pthread.h>

typedef struct
{
    int values;
    int wakeup;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sems_t;

void sems_init(sems_t *sem, int v)
{
    sem->values = v;
    sem->wakeup = 0;
    pthread_mutex_init(&(sem->mutex), NULL);
    pthread_cond_init(&(sem->cond), NULL);
}

void sems_wait(sems_t *sem, int t, int d)
{
    pthread_mutex_lock(&(sem->mutex));
    while (sem->values - d < t)
    {
        sem->wakeup++;
        pthread_cond_wait(&(sem->cond), &(sem->mutex));
    }
    sem->values -= d;
    pthread_mutex_unlock(&(sem->mutex));
}

void sems_signal(sems_t *sem, int d)
{
    pthread_mutex_lock(&(sem->mutex));
    sem->values += d;
    if (sem->wakeup > 0)
    {
        pthread_cond_broadcast(&(sem->cond));
        sem->wakeup = 0;
    }
    pthread_mutex_unlock(&(sem->mutex));
}
