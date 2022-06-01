# 第 8 章 Linux 的信号

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Linux 信号/软中断执行过程](#linux-信号软中断执行过程)
- [注册信号 singal, sigaction](#注册信号-singal-sigaction)
- [发送信号 kill, raise, sigqueue](#发送信号-kill-raise-sigqueue)
- [定时信号 alarm, 退出信号 abort](#定时信号-alarm-退出信号-abort)
- [Linux 信号集 sigemptyset, sigfillset, sigaddset, sigdelset, sigismember](#linux-信号集-sigemptyset-sigfillset-sigaddset-sigdelset-sigismember)
- [信号阻塞和挂起 sigprocmask, sigsuspend 、信号的精确定时 setitimer 以及避免在信号处理中调用不可重入函数](#信号阻塞和挂起-sigprocmask-sigsuspend-信号的精确定时-setitimer-以及避免在信号处理中调用不可重入函数)

<!-- /code_chunk_output -->

### Linux 信号/软中断执行过程

信号只是用来通知某进程发生了神秘事件，但并不给该进程传递任何数据。

没有编号为 0 的信号，这是一种空信号。

信号存在局限性，比如系统开销太大，要进行系统调用、传递信息十分有限等。

以 Ctrl+C 说明一下信号的执行过程：
- 用户输入命令，在 Shell 启动一个前台进程
- 用户按下 Ctrl+C ，键盘输入产生一个硬件中断
- 如果处理器当前正在执行这个进程的代码，则该进程的用户空间代码暂停执行， CPU 从用户态切换到内核态处理硬件中断
- 终端驱动程序将 Ctrl+C 解释成一个 SIGINT 信号，记录在该进程的 PCB 中（也可以说发送了一个 SIGINT 信号给该进程）
- 某个时刻要从内核返回到该进程的空间用户代码继续执行之前，首先处理 PCB 中记录的信号，发现有一个 SIGINT 信号待处理，而这个信号的默认处理动作是终止进程，所以直接中止进程，而不再返回它的用户空间代码

### 注册信号 singal, sigaction

```c
#include <signal.h>
void (*signal (int signum, void (*handler)(int)))(int);

// 简化版定义如下
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

由此我们可以自定义一些信号处理函数，如 [exam801singal.c](../sample/chapter8/exam801singal.c) 。

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//这是信号处理函数
void signalDeal(int sig)
{
  if(sig == SIGINT)    //对应ctrl+c
  {
    printf("CTRL+C按键被按下。\n");
  }
  else if(sig == SIGQUIT)  //对应ctrl+/
  {
    printf("CTRL+/按键被按下.\n");
  }
  else
  {
    printf("其他信号。\n");
  }
}
//以下是主函数
int main(int argc,char *argv[])
{
  signal(SIGINT,signalDeal);   //注册SIGINT对应的处理函数
  signal(SIGQUIT,signalDeal);  //注册SIGQUIT对应的处理函数
  while(1)    //永远循环
  {
  }
  return 0;
}
```

此外，还有更为强大的、可以获取信号进程等等相关信息的 `sigaction` 函数。

```c
#incldue <signal.h>
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

### 发送信号 kill, raise, sigqueue

```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int signum);  // pid 可以是进程、负数也可表示进程组、 0 表示当前进程组
int raise(int signum);  // 给自身发信号
int sigqueue(pid_t pid, int signum, const union sigval value);  // 常配合 sigaction ，比 kill 更多信息
```

值得注意的是， `kill` 是用来发送信号的，并不是用来杀死进程的。

### 定时信号 alarm, 退出信号 abort

```c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);  // 设置多少秒后发生 SIGALRM 信号

#include <stdlib.h>
void abort(void);  // 终止进程，进程没有返回值
```

### Linux 信号集 sigemptyset, sigfillset, sigaddset, sigdelset, sigismember

```c
typedef struct
{
    unsigned long sig[_NSIG_WORDS];
} sigset_t;

#include <signal.h>
int sigemptyset(sigset_t *set);  // 初始化信号集
int sigfillset(sigset_t *set);  // 初始化信号集，并将所有信号都设置为 1
int sigaddset(sigset_t *set, int signum);  // 将 signum 信号加入到信号集中
int sigdelset(sigset_t *set, int signum);  // 将 signum 信号从信号集中删除
int sigismember(const sigset_t *set, int signum);  // 判断 signum 信号是否在信号集中
```

### 信号阻塞和挂起 sigprocmask, sigsuspend 、信号的精确定时 setitimer 以及避免在信号处理中调用不可重入函数

有时候，我们不希望信号立即打断当前工作，也不希望信号被忽视。因此有了这里的几个函数。

```c
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);  // 设置信号阻塞和挂起
int sigsuspend(const sigset_t *set);  // 挂起进程，直到收到一个信号
```

如下参考 [https://blog.csdn.net/ShaoLiang_Ge/article/details/57984123](https://blog.csdn.net/ShaoLiang_Ge/article/details/57984123) 。

```c
/*
1)头文1)头文件：#include <signal.h>
2）一个保护临界区代码的错误实例：(sigprocmask()和pause()实现)
*/
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
 
void handler(int sig)    //信号处理函数的实现
{
   printf("SIGINT sig");
}
int main()
{
    sigset_t new,old;
    struct sigaction act;
    act.sa_handler = handler;  //信号处理函数handler
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);  //准备捕捉SIGINT信号
    sigemptyset(&new);
    sigaddset(&new, SIGINT);
    sigprocmask(SIG_BLOCK, &new, &old);  //将SIGINT信号阻塞，同时保存当前信号集
    printf("Blocked");
    sigprocmask(SIG_SETMASK, &old, NULL);  //取消阻塞
    pause();  //让进程暂停直到信号出现
    return 0；
}
/*
上面实例的问题是：本来期望pause()之后，来SIGINT信号，可以结束程序；可是，如果当“取消阻塞”和“pause”之间，正好来了SIGINT信号，结果程序因为pause的原因会一直挂起。。。
解决的方式，当然是sigsuspend()函数了。
3）使用sigsuspend()的程序
*/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
void handler(int sig)   //信号处理程序
{
   if(sig == SIGINT)
      printf("SIGINT sig");
   else if(sig == SIGQUIT)
      printf("SIGQUIT sig");
   else
      printf("SIGUSR1 sig");
}
 
int main()
{
    sigset_t new,old,wait;   //三个信号集
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);    //可以捕捉以下三个信号：SIGINT/SIGQUIT/SIGUSR1
    sigaction(SIGQUIT, &act, 0);
    sigaction(SIGUSR1, &act, 0);
   
    sigemptyset(&new);
    sigaddset(&new, SIGINT);  //SIGINT信号加入到new信号集中
    sigemptyset(&wait);
    sigaddset(&wait, SIGUSR1);  //SIGUSR1信号加入wait
    sigprocmask(SIG_BLOCK, &new, &old);       //将SIGINT阻塞，保存当前信号集到old中
   
    //临界区代码执行    
  
    if(sigsuspend(&wait) != -1)  //程序在此处挂起；用wait信号集替换new信号集。即：过来SIGUSR1信  号，阻塞掉，程序继续挂起；过来其他信号，例如SIGINT，则会唤醒程序。执行sigsuspend的原子操作。注意：如果“sigaddset(&wait, SIGUSR1);”这句没有，则此处不会阻塞任何信号，即过来任何信号均会唤醒程序。
        printf("sigsuspend error");
    printf("After sigsuspend");
    sigprocmask(SIG_SETMASK, &old, NULL);
    return 0;
}
/*
sigsuspend的原子操作是：
（1）设置新的mask阻塞当前进程(上面是用wait替换new，即阻塞SIGUSR1信号)
（2）收到SIGUSR1信号，阻塞，程序继续挂起；收到其他信号，恢复原先的mask(即包含SIGINT信号的)。
（3）调用该进程设置的信号处理函数(程序中如果先来SIGUSR1信号，然后过来SIGINT信号，则信号处理函数会调用两次，打印不同的内容。第一次打印SIGINT,第二次打印SIGUSR1，因为SIGUSR1是前面阻塞的)
（4）待信号处理函数返回，sigsuspend返回了。(sigsuspend将捕捉信号和信号处理函数集成到一起了)
*/
```

信号精确定时 `setitimer` 如下。

```c
#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
```

最后，聊一下 **可重入函数。**

一般而言，信号处理函数中不应该有不可重入函数。在多任务系统下，中断可能在任务执行的任何时间发生；如果一个函数的执行期间被中断后，到重新恢复到断点进行执行的过程中，函数所依赖的环境没有发生改变，那么这个函数就是可重入的，否则就不可重入。我们知道中断时确实保存一些上下文，但是仅限于返回地址， cpu 寄存器等之类的少量上下文，而函数内部使用的诸如全局或静态变量， buffer 等并不在保护之列，所以如果这些值在函数被中断期间发生了改变，那么当函数回到断点继续执行时，其结果就不可预料了。

满足下面条件之一的多数是不可重入函数：
- 使用了静态数据结构
- 调用了 `malloc` 或 `free`
- 调用了标准 I/O 函数
- 进行了浮点运算，因为浮点运算大多依赖协处理器或者软件模拟来实现

不可重入函数可能存在以下两种情况：
- 信号处理程序A内外都调用了同一个不可重入函数B；B在执行期间被信号打断，进入A (A中调用了B),完事之后返回B被中断点继续执行，这时B函数的环境可能改变，其结果就不可预料了
- 多线程共享进程内部的资源，如果两个线程A，B调用同一个不可重入函数F，A线程进入F后，线程调度，切换到B，B也执行了F，那么当再次切换到线程A时，其调用F的结果也是不可预料的。
