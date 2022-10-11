# A Simple Introduction to pthread.h

## Lead in

> A：🤔为什么我写的程序跑的这么慢呢
> B：😂check the server，一核有难万核围观

<img src="https://picgo-1303220879.cos.ap-shanghai.myqcloud.com/img/20221011090209.png" width="300" align="center"/>

在多处理器系统越来越普及的时代，你需要学习——并发编程！

POSIX 为我们提供了线程库 (pthreads)，这是在多核平台上进行并发编程的一套API。

本文，我们介绍三个内容：

- 如何使用pthreads库创建并发线程
- 如何使用pthreads库实现线程同步（即OS同步原语的使用）
- 线程API的使用准则

> Recall：在体系结构课中，我们使用了[OpenMP](https://en.wikipedia.org/wiki/OpenMP)，也是并发编程的一套接口，
其规范在编译器上实现，相较于pthreads更易于扩展。

## 1 线程创建和使用

## 2 同步原语

### 2.1 互斥锁

#### 主要函数接口

| 函数名                    | 功能             |
| ------------------------- | ---------------- |
| `pthread_mutex_init()`    | 初始化一个互斥锁 |
| `pthread_mutex_destroy()` | 销毁一个互斥锁   |
| `pthread_mutex_lock()`    | 加锁             |
| `pthread_mutex_trylock()` | 尝试加锁         |
| `pthread_mutex_unlock()`  | 解锁             |

#### 接口用法


#### 程序样例

```c


```

### 2.2 条件变量

### 2.3 读写锁

## 3 线程API的使用准则

> 此部分译自OSTEP 27.6 Summary

当你使用线程库来构建程序时，必须记住一些xio但很重要的事情：

- **保持简单**： 这是最重要的一件事，你应该尽量保持你涉及到锁定和发出信号（lock&signal）的代码
简单，以免出现一些神必的线程间相互作用导致的错误。
- **尽量减少线程之间的互动**：每一种交互都应该都要仔细考虑，并采用经过验证的方法来构建。
- **初始化同步原语**：如果不这么做，有时会出现你在调试时🈚问题，但是上环境以后出现非常诡异的错误。
- **检查你的返回代码**：当然，在你做的任何C和UNIX编程中，你应该检查每一个返回代码。
这里也是如此。如果不这样做，就会导致奇怪的和难以理解的行为，~~导致你砸键盘~~。
- **在传递参数和返回值时小心处理**：特别是在传递在堆栈空间内的指针时，你可能会出错。
- **注意每个线程都有自己的堆栈**：这和上一条相关请记住，每个线程都有自己的堆栈。因此，如果你有一个
本地分配的变量，它本质上是该线程的私有变量；其他线程不可能(轻易）访问它。要在线程之间共享数据，这些值必须在
在堆中，或者其他一些全局可访问的位置。
- **请使用条件变量在线程间传递信号**：虽然使用信号（signal flag）很简单且诱人，但是请不要这么做。
- **查看手册**~~RTFM~~：特别是在Linux上，pthread 手册的信息量很大，讨论了很多这里提出的细微差别，通常甚至更详细。仔细阅读它们吧~


## 参考资料

- [pthreads Wiki](https://en.wikipedia.org/wiki/Pthreads)

- [pthread.h Document by IBM](https://www.ibm.com/docs/en/aix/7.2?topic=files-pthreadh-file)

- [pthread.h Document by OpenGroup](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)

- [Operating System: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/),Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau, Arpaci-Dusseau Books, August, 2018 (Version 1.00)