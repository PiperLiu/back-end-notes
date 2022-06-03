# 第 10 章 Linux 的线程

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Linux 的线程/轻量级进程 Light Weight Process](#linux-的线程轻量级进程-light-weight-process)
- [Linux 的线程操作](#linux-的线程操作)
- [Linux 的线程属性](#linux-的线程属性)
- [Linux 的线程私有数据](#linux-的线程私有数据)
- [Linux 的线程同步（互斥锁与条件变量）](#linux-的线程同步互斥锁与条件变量)

<!-- /code_chunk_output -->

### Linux 的线程/轻量级进程 Light Weight Process

进程的问题：
- fork 开销大
- 地址空间独立，通信不方便

线程是进程中的一个实体，由线程标识符、程序计数器（PC）、寄存器集合、堆栈等组成。

### Linux 的线程操作

```bash
gcc -lpthread  # 编译时告诉编译器头文件pthread 函数库
```

这里可以粗略对比一下进程和线程的函数：
- `fork` `pthread_create`
- `exit` `pthread_exit`
- `waitpid` `pthread_join`
- `atexit` `pthread_cleanup_push`
- `getpid` `pthread_self`
- `abort` `pthread_cancel`

```c
#include <pthread.h>
// 创建线程
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);

// 阻塞和退出线程 retval 用于存放去线程返回值
int pthread_join(pthread_t thread, void **retval);
void pthread_exit(void *retval);

// 取消和清理线程
int pthread_cancel(pthread_t thread);
void pthread_cleanup_push(void (*routine)(void *), void *arg);
void pthread_cleanup_pop(int execute);

// 分离线程
int pthread_detach(pthread_t thread);
```

### Linux 的线程属性

```c
typedef struct
{
    int detachstate;
    int schedpolicy;
    struct sched_param schedparam;
    int inheritsched;
    int scope;
    size_t guardsize;
    int stackaddr_set;
    void *stackaddr;
    size_t stacksize;
} pthread_attr_t;

#include <pthread.h>

// 线程属性对象的初始化和销毁函数
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);

// 线程堆栈大小相关函数
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

// 线程堆栈地址相关函数
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);

// 线程分离状态相关函数
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);

// 线程作用域函数
int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);

// 线程的继承调度函数
int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched);

// 线程的调度策略函数
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int schedpolicy);
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *schedpolicy);

// 线程的调度参数函数
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *schedparam);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *schedparam);
```

### Linux 的线程私有数据

每个线程都有一些属于自己的数据，当线程对这些数据进行操作时可以独立地访问他们，不用担心与其他线程争夺所有权，这些数据被称为线程私有数据（线程特定数据）。

```c
#include <pthread.h>

// 创建键函数
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));

// 取消键函数
int pthread_key_delete(pthread_key_t key);

// 解决键冲突函数
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));

// 键关键函数
int pthread_setspecific(pthread_key_t key, const void *pointer);

// 线程私有数据地址获取函数
void *pthread_getspecific(pthread_key_t key);
```

### Linux 的线程同步（互斥锁与条件变量）

```c
#include <pthread.h>

// 互斥锁的初始化函数
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

// 互斥锁的销毁函数
int pthread_mutex_destroy(pthread_mutex_t *mutex);

// 互斥锁锁定函数
int pthread_mutex_lock(pthread_mutex_t *mutex);

// 互斥锁加锁函数（如果已经 lock 则返回错误而非阻塞）
int pthread_mutex_trylock(pthread_mutex_t *mutex);

// 互斥锁解锁函数
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

此外，还可以使用条件变量来实现线程同步（但是也常常配合互斥锁一起使用）。

```c
#include <pthread.h>

// 条件变量的初始化函数
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *cond_attr);

// 条件变量的销毁函数
int pthread_cond_destroy(pthread_cond_t *cond);

// 条件变量的阻塞函数
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

// 待时间的条件变量阻塞函数
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

// 单个条件变量的唤醒函数
int pthread_cond_signal(pthread_cond_t *cond);

// 全部条件变量的唤醒函数
int pthread_cond_broadcast(pthread_cond_t *cond);
```
