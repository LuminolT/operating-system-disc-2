#include "sems_from_pthread.h"
//#include "sems_from_semaphore.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <processthreadsapi.h>
const int RN = 3;
sems_t L;
sems_t mx;

int a = 1;

void *reader(void *arg)
{
    while (1)
    {
        printf("thread %d is waiting to read\n", GetCurrentThreadId());
        sems_wait(&L, 1, 1);
        sems_wait(&mx, 1, 0);

        printf("thread %d reading\n", GetCurrentThreadId());
        sleep(5);
        sems_signal(&L, 1);
        sleep(1);
    }
    return NULL;
}

void *writer(void *arg)
{
    while (1)
    {

        printf("thread %d is trying to write\n", GetCurrentThreadId());
        sems_wait(&L, RN, 0);
        sems_wait(&mx, 1, 1);

        printf("thread %d begin to write.\n", GetCurrentThreadId());
        sleep(1);
        printf("thread %d end writing.\n", GetCurrentThreadId());

        sems_signal(&mx, 1);
        sleep(1);
    }
    return NULL;
}

void test()
{

    sems_init(&L, RN);
    sems_init(&mx, 1);
    pthread_t write, read, read1, read2;
    // pthread_create(&write, NULL, writer, NULL);
    // pthread_create(&read, NULL, reader, NULL);
    // pthread_create(&read1, NULL, reader, NULL);
    // pthread_create(&read2, NULL, reader, NULL);
    // pthread_join(write, NULL);
    // pthread_join(read, NULL);
    // pthread_join(read1, NULL);
    // pthread_join(read2, NULL);

    pthread_create(&read, NULL, reader, NULL);
    pthread_create(&write, NULL, writer, NULL);
    pthread_create(&read1, NULL, reader, NULL);

    pthread_create(&read2, NULL, reader, NULL);
    pthread_join(write, NULL);
    pthread_join(read, NULL);
    pthread_join(read1, NULL);
    pthread_join(read2, NULL);
}

int main(int argc, char *argv[])
{
    printf("-------pthread------\n");
    test();
}