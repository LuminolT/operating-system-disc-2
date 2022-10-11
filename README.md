# operating-system-disc-2
操作系统(1) 研讨2代码仓库

研讨题内容请参照：[利用信号量集解决Reader-and-Writer问题](doc/problem.md)

补充内容请参照：[pthreads库使用](doc/pthreads.md)

### 仓库结构

```
│  main.c                       主程序
│  README.md                    README文档
│  sems_from_pthread.h          信号量集：互斥锁和条件变量实现
│  sems_from_semaphore.h        信号量集：信号量实现
│
├─doc
│      problem.md               问题描述文档
│      pthreads.md              补充pthreads库使用文档
│
└─old
        RWquestion.c            已弃用，读者-写者问题解决尝试
        sems_from_cpp.h         已弃用，信号量集C++实现
```

### 注意

Fork Repository后自行完成

commit格式：`<operations>: <content>`

例：`Update: subexp-1`，`Finish: 1,4,7`

完成后Pull Request到`main`分支
