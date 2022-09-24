# 利用信号量集解决Reader-and-Writer问题


## 题目内容
所谓“读者-写者问题”是指保证一个Writer进程必须与其他进程互斥地访问共享对象的同步问题。
为实现Reader与Writer进程间在读或写时的互斥而设置了一个互斥信号量Wmutex。另外，再设置
一个整型变量Readcount表示正在读的进程数目。由于只要有一个Reader进程在读，便不允许Writer进程
去写。因此，仅当Readcount=0, 表示尚无Reader进程在读时，Reader进程才需要执行P(Wmutex)操作。
若P(Wmutex)操作成功，Reader进程便可去读，相应地，做Readcount+1操作。同理，仅当Reader进程
在执行了Readcount减1操作后其值为0时，才须执行V(Wmutex)操作，以便让Writer进程写。又因为Readcount是
一个可被多个Reader进程访问的临界资源，因此，应该为它设置一个互斥信号量rmutex。 

读者-写者问题”扩展三：利用记录型信号量集分别给出“读写无优先、写者优先”问题的同步算法。(20min)

## 一些需要完成的工作

查询发现，没有“记录型信号量集”这种神必讲法，理解为信号量集

“读写无优先、写者优先”或许是读者无优先，一些文献提法是“倾向Writer”。
