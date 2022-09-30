#include <iostream>
#include "PV.h"
#include <pthread.h>
#include <time.h>
#include<unistd.h>

int RN=2;
Semaphore L(RN);
Semaphore mx(1);
int a=1;

void* reader(void *arg)
{
     while (1)
     {
        mx.wait(1,0);
        L.wait();

        cout<<a<<endl;

        L.signal();
        // sleep(1);
     }
     return NULL;
 }

void* writer(void *arg)
{
    while (1)
    {
        mx.wait();
        L.wait(RN,0);

        a++;

        mx.signal(1,1);
        sleep(2);
    }
    return NULL;
}

int main()
{

    cout << "Hello, world!\n";
    pthread_t write,read,read1;
    pthread_create(&write,NULL,writer,NULL);
    pthread_create(&read,NULL,reader,NULL);
    // pthread_create(&read1,NULL,reader,NULL);
    pthread_join(write, NULL);
    pthread_join(read, NULL);
    // pthread_join(read1, NULL);

}
