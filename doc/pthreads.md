# A Simple Introduction to pthreads

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

## 1 线程的创建和使用

### 1.1 主要函数接口

| 函数名                         | 功能                       |
| :---------------------------: | :-----------------------: |
| `int pthread_create()`		| 创建线程					|
| `int pthread_detach()`		| 分离线程					|
| `int pthread_equal()`			| 比较线程 ID				|
| `void pthread_exit()`			| 终止线程					|
| `int pthread_join()`			| 挂起并等待指定线程终止		|
| `int pthread_kill()`			| 向线程发送信号			|
| `int pthread_once()`			| 仅一次调用指定函数		|
| `pthread_t pthread_self()`	| 返回线程 ID				|
| `int pthread_cancel()`		| 向线程发送取消信号			|
| `int pthread_setcancelstate()`| 设置本线程对取消信号的反应		|
| `int pthread_setcanceltype()`	| 设置本线程取消动作的执行时机		|
| `void pthread_testcancel()`	| 创建线程取消点				|

### 1.2 接口用法

#### 1.2.1 创建线程

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
			void *(*start_routine)(void *), void *arg)
```

- `pthread_t *thread` ：传递一个 `pthread_t` 类型的指针变量，也可以直接传递某个 `pthread_t` 类型变量的地址。

- `const pthread_attr_t *attr`：用于手动设置新建线程的属性，例如线程的调用策略、线程所能使用的栈内存的大小等。大部分场景中，我们都不需要手动修改线程的属性，将 `attr` 参数赋值为 `NULL`，`pthread_create()` 函数会采用系统默认的属性值创建线程。

- `void *(*start_routine) (void *)`：以函数指针的方式指明新建线程需要调用的函数，该函数的参数最多有 1 个（可以省略不写），形参和返回值的类型都必须为 `void*` 类型。

- `void *arg`：指定传递给 `start_routine` 函数的实参，当不需要传递任何数据时，将 `arg` 赋值为 `NULL` 即可。

#### 1.2.2 分离线程

```c
int pthread_detach(pthread_t thread)
```

- 表示线程运行结束后资源可被回收（线程所占用堆栈和线程描述符）。

#### 1.2.3 比较线程 ID

```c
int pthread_equal(pthread_t t1, pthread_t t2)
```

- 比较线程 `t1` 和 `t2` 的线程 ID。如果相等，返回非零值；如果不相等，返回 0。

#### 1.2.4 终止线程

```c
void pthread_exit(void *value_ptr)
```

- `void *value_ptr`： 可以指向任何类型的数据，它指向的数据将作为线程退出时的返回值。如果线程不需要返回任何数据，将参数置为 `NULL` 即可。

#### 1.2.5 挂起并等待指定线程终止

```c
int pthread_join(pthread_t thread, void **value_ptr)
```

- `pthread_t thread`：等待终止的目标线程。
- `void **value_ptr`：接收到的返回值，为 `void pthread_exit(void *value_ptr)` 中 `void *value_ptr` 指向的内容。
- 如果调用 `int pthread_join()` 函数的线程被取消，目标线程不会被分离。

#### 1.2.6 向线程发送信号

```c
int pthread_kill(pthread_t thread, int sig)
```

- `sig`：向指定线程 `pthread_t thread` 发送的信号。

#### 1.2.7 仅一次调用指定函数

```c
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
```

- `pthread_once_t *once_control`：指定函数 `init_routine` 是否被调用的标志，需赋初值 `PTHREAD_ONCE_INIT` 以保证 `int pthread_once()` 函数正常调用。
- `void (*init_routine)(void)`：指定函数 `init_routine`，在 `once_control` 值为 `PTHREAD_ONCE_INIT` 时，可被正常调用，随后使用再次调用 `int pthread_once()` 函数与相同 `once_control`，将不会再次调用 `init_routine` 函数。

#### 1.2.8 返回线程 ID

```c
pthread_t pthread_self(void)
```

- 返回调用该函数的线程的线程 ID。

#### 1.2.9 向线程发送取消信号

```c
int pthread_cancel(pthread_t thread)
```

- 发送终止信号给 `thread` 线程，如果成功则返回 0，否则为非 0 值。发送成功并不意味着 `thread` 会终止。

#### 1.2.10 设置本线程对取消信号的反应

```c
int pthread_setcancelstate(int state, int *oldstate)
```

- `int state` ：具有两种取值 `PTHREAD_CANCEL_ENABLE`（缺省）和 `PTHREAD_CANCEL_DISABLE`，
分别表示收到信号后设为 CANCLED 状态和忽略 CANCEL 信号继续运行；`old_state` 如果不为 `NULL` 则存入原来的 Cancel 状态以便恢复。

#### 1.2.11 设置本线程取消动作的执行时机

```c
int pthread_setcanceltype(int type, int *oldtype)
```

- `int type` ：具有两种取值 `PTHREAD_CANCEL_DEFFERED` 和 `PTHREAD_CANCEL_ASYCHRONOUS`，仅当 Cancel 状态为 Enable 时有效，分别表示收到信号后继续运行至下一个取消点再退出和立即执行取消动作（退出）；`oldtype` 如果不为 `NULL` 则存入运来的取消动作类型值。

#### 1.2.12 创建线程取消点

```c
void pthread_testcancel(void)
```

- 在不包含取消点，但是又需要取消点的地方创建一个取消点，以便在一个没有包含取消点的执行代码线程中响应取消请求。

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