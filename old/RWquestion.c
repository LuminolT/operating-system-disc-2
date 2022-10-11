#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/ipc.h>

struct sembuf sbL={0,-1,0};
struct sembuf sbmx={1,-1,0};
struct sembuf plusL={0,+1,0};
struct sembuf plusmx={1,+1,0};
int RN=2;
int aaa=1;

void* reader(int semid)
{
    do
    {
        semop(semid,&sbL,1);
        semop(semid,&sbmx,1);
       
        printf("%d\n",aaa);

        semop(semid,&plusL,1);
        semop(semid,&plusmx,1);

        sleep(2);
    } while (1) ;  
    return NULL;
}
void* writer(int semid)
{
    do
    {
        int val=semctl(semid,1,GETVAL);
        if (val==RN)
        {
            semop(semid,&sbmx,1);

            aaa+=1;

            printf("Now, writing……\n");
            semop(semid,&plusmx,1);

            sleep(2);
        }
    } while (1);
    return NULL;
}

typedef union semun1{
    int val;
}semun_t1;

int main ()
{
    int semid;
    key_t key;
    key=ftok("./tt",1);
    semid=semget(key,2,IPC_CREAT|0666);
    semun_t1 L,mx;
    L.val=RN;
    mx.val=1;
    semctl(semid,0,SETVAL,L);
    semctl(semid,1,SETVAL,mx);
    
    pthread_t write;
    pthread_t read1;
    pthread_t read;
    pthread_create(&write,NULL,writer,semid);
    pthread_create(&read1,NULL,reader,semid);
    pthread_create(&read, NULL,reader,semid);
    pthread_join(write,NULL);
    pthread_join(read1,NULL);
    pthread_join(read,NULL);
    return 0;
}