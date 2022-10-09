// #include "sems.h"
#include"sems_signal.h"
#include <unistd.h>
#include <pthread.h>

const int RN=2;
// sem_s L={RN,0};
// sem_s mx={1,0};

sems_t L;
sems_t mx;

int a=1;

// void* reader(void *arg)
// {
//     while (1)
//     {
//         P(&mx,1,0);
//         P(&L,0,1);

//         printf("%d\n",a);

//         V(&L,1, 1);
//         sleep(1);
//     }
//     return NULL;
// }

// void* writer(void *arg)
// {
//     while (1)
//     {
//         P(&mx,0,1);
//         P(&L,RN,0);

//         a++;

//         V(&mx,1,1);
//         sleep(2);
//     }
//     return NULL;
// }

void* reader(void *arg)
{
    while (1)
    {
        P(&mx,1,0);
        P(&L,0,1);

        printf("%d\n",a);

        V(&L,1);
        sleep(1);
    }
    return NULL;
}

void* writer(void *arg)
{
    while (1)
    {
        P(&mx,0,1);
        P(&L,RN,RN);

        a++;

        V(&mx,1);
        V(&L, RN);
        sleep(2);
    }
    return NULL;
}

int main()
{
    sem_s_init(&L, RN);
    sem_s_init(&mx, 1);
    printf("Hello, world!\n");
    pthread_t write,read,read1;
    pthread_create(&write,NULL,writer,NULL);
    pthread_create(&read,NULL,reader,NULL);
    pthread_create(&read1,NULL,reader,NULL);
    pthread_join(write, NULL);
    pthread_join(read, NULL);
    pthread_join(read1, NULL);
}
