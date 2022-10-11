#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H
#include <mutex>
#include <condition_variable>
using namespace std;
 
class Semaphore
{
public:
    Semaphore(long count = 0) : count(count) {}
    //V操作，唤醒
    void signal(long t=0, long d=1)
    {
        unique_lock<mutex> unique(mt);
        count+=d;
        if (count <= t)
            cond.notify_one();
    }
    //P操作，阻塞
    void wait(long t=0, long d=1)
    {
        unique_lock<mutex> unique(mt);
        count-=d;
        if (count < t)
            cond.wait(unique);
    }
    
private:
    mutex mt;
    condition_variable cond;
    long count;
};
#endif