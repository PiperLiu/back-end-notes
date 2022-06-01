# 第 7 章 Linux 的进程

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [进程的状态、描述符、标识符](#进程的状态-描述符-标识符)
- [进程的操作： fork, exec, vfork, _exit, atexit](#进程的操作-fork-exec-vfork-_exit-atexit)
- [Linux 的进程组和会话（前台进程组）](#linux-的进程组和会话前台进程组)
- [更改进程用户、 system 、 acct 、 time](#更改进程用户-system-acct-time)

<!-- /code_chunk_output -->

### 进程的状态、描述符、标识符

创建一个新进程时， Linux 将从系统内存中分配一个 `task_struct` 结构，加入 `task` 数组，并将其与一个新的 `pid` 对应起来。

```c
struct task_struct {
    ...
    pid_t tgid;
    ...
}
```

进程的状态有以下几种：
- `TASK_RUNNING`：进程正在运行，占用着处理器或者准备执行
- `TASK_INTERRUPTIBLE`：进程可以被中断，进程正在被挂起或者睡眠
- `TASK_UNINTERRUPTIBLE`：进程不可被中断，接收到信号时并不能退出这个等待状态
- `TASK_STOPPED`：进程已经被暂停，进程正在被挂起或者睡眠
- `TASK_ZOMBIE`：进程已经被终止，但是父进程还没有使用 `wait` 系统调用返回相应信息，因此内核不能丢弃相关信息

Linux 中有几个特殊的进程标识符所对应的进程：
- `0` ：对应的是交换进程 swapper ，用于执行多进程的调用
- `1` ：对应的是初始化进程 init ，对应的文件是 `/sbin/init` ，负责 Linux 的启动工作
- `2` ：可能对应页守护进程 pagedaemon ，用于虚拟存储系统的分页操作

获取进程标识符和用户的系统调用如下。

```c
#include <sys/types.h>
#include <unistd.h>
pid_t getpid(void);
pid_t getppid(void);

uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
```

### 进程的操作： fork, exec, vfork, _exit, atexit

```c
#include <unistd.h>
pid_t fork(void);  // 完全复制（深拷贝）父进程上下文、变量

int execl(const char *path, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execlp(const char *file, const char *arg, ...);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

如上， `l` 代表 `list` ，传入多个参数， `v` 代表 `vector` ，传入一个参数字符串； `e` 代表 `environment` ，代表可以传入一个环境变量。

比如对于 `execle` 如下。

```c
execle(char *arg0, char *arg1, ..., char *argn, (char*)0, char *envp[])
```

**其实环境变量默认被 main 捕获的，可以见例子 [exam706printfenvp.c](../sample/chapter7/exam706printfenvp.c) 。**

```c
/*这是一个输出envp环境变量的实例*/
#include <stdio.h>
int main(int argc, char *argv[ ], char *envp[ ])
{
   printf("这是参数argc\n%d\n", argc);  //首先打印参数的数目
   printf("这是参数argv\n");  //以下打印参数列表
   while(*argv)  //如果不为空，则输出这些字符串
   {
       printf("%s\n", *(argv++));
   }
   printf("这是环境变量envp\n");  //以下是envp字符串参数
   while(*envp)   //输出envp参数
   {
       printf("%s\n", *(envp++));
   }
    return 0;
}
```

关于 `vfork` 的说明：
- `fork` 要拷贝父进程的数据段；而 `vfork` 是共享的（在子进程调用 `exec` 或 `exit` 之前）
- `fork` 不对父进程执行次序进行限制，但是 `vfork` 中子进程先运行，父进程挂起，直到子进程调用了 `exec` 或 `exit`

`vfork` 最初是因为 fork 没有实现 COW 机制，而很多情况下 `fork` 之后会紧接着 `exec` ，而 `exec` 的执行相当于之前 `fork` 复制的空间全部变成了无用功，所以设计了 `vfork` 。

关于 `_exit` 的说明：
- `_exit` 直接使进程停止运行，清除其使用的内存空间
- `exit` 是对 `_exit` 进行包装，在退出前执行一些工作，比如把缓冲区未写入的数据写入磁盘

此外还可以利用系统调用添加 `exit` 的回调函数，见例子 [exam714atexit.c](../sample/chapter7/exam714atexit.c) 。

```c
#include <stdio.h>
#include <stdlib.h>
//用于登记退出执行的第一个函数
void exitfun1(void)
{
  printf("这是第一个终止处理函数!\n");
  return;
}
//用于登记执行的第二个函数
void exitfun2(void)
{
  printf("这是第二个终止处理函数!\n");
  return;
}
int main(int argc,char *argv[])
{
  atexit(exitfun1);  //登记两个函数
  atexit(exitfun2); 
  atexit(exitfun2);  //再次登记
  printf("这是主程序的输出!\n"); //在主程序中输出一个字符串
  exit(0);
}
```

销毁进程即 `wait` 系列函数。

```c
#include <sys/types.h>
#include <sys/wait.h>
pit_t wait(int *status);  // 子进程结束前父进程阻塞
pid_t waitpid(pid_t pid, int *status, int options);  // 可以指定不阻塞父进程
```

### Linux 的进程组和会话（前台进程组）

进程组是若干进程的组合。每个进程都隶属于一个进程组。

```c
#include <sys/types.h>
#include <unistd.h>
pid_t getpgrp(void);  // 获取当前进程的进程组

int setpgid(pid_t pid, pid_t pgid);  // 设置进程组
```

会话是多个进程组的集合。

比如，当我们有一个 shell 进程，并且在其中执行命令如下：

```bash
proc1 | proc2 &  # & 表示 proc1 | proc2 是后台进程
proc3 | proc4 | proc5
```

那此时会话里就有三个进程组，分别是 `shell` 、 `proc1, proc2` 和 `proc3, proc4, proc5` 。

用户在终端输入特殊的控制键（例如 Ctrl+C ），则内核会发送信号 **给前台进程组（`proc3 | proc4 | proc5`）** 的所有进程。

**每个会话都有一个控制终端，对控制终端进行读写的方法是打开文件 `/dev/tty` 。一个控制终端会有一个前台进程组，其余为后台进程组。比如 shell 会在执行 `proc3 | proc4 | proc5` 后，将前台进程组让给 `proc3 | proc4 | proc5` 。**

```c
#include <sys/types.h>
#include <unistd.h>
pid_t setsid(void);  // 创建一个新的会话，并将当前进程设置为新会话的首进程

// 返回前台进程组 id
pid_t tcgetpgrp(int fd);  // pid = tcgetpgrp(STDOUT_FILENO)

// 将一组进程组设置为前台进程组
int tcsetpgrp(int fd, pid_t pgid);  // tcsetpgrp(STDOUT_FILENO, getpid())
```

### 更改进程用户、 system 、 acct 、 time

```c
#include <sys/types.h>
#include <unistd.h>
int setuid(uid_t uid);  // 设置进程的用户 id
int setgid(gid_t gid);  // 设置进程的组 id

int setreuid(uid_t ruid, uid_t euid);  // 设置进程的 real uid 和 effective uid
int setregid(gid_t rgid, gid_t egid);

int seteuid(uid_t euid);  // 设置进程的 effective uid
int setegid(gid_t egid);
```

在 Linux 中调用其他程序如下。

```c
#include <stdlib.h>
int system(const char *command);  // 调用系统命令
```

进程状态统计如下。

```c
#include <unistd.h>
int acct(const char *filename);  // 记录进程的状态
```

关于时间命令有 `time` ，相关的函数如下。

```c
#include <unistd.h>
long sysconf(int name);  // 获取系统配置
// sysconf(_SC_CLK_TCK) 可获取一秒钟多少个 tick

#include <sys/times.h>
clock_t times(struct tms *buf);  // 获取进程的时间
```

使用示例如 [exam720time.c](../sample/chapter7/exam720time.c) 。

```c
#include <sys/times.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
//时间统计函数
static void get_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
  static long             clktck = 0;
  if (clktck == 0)        //第一次获得时间
  if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
  {
     perror("调用sysconf函数错误.\n");
  }
  //以下为时间输出
  printf("时钟时间:  %7.2f\n", real / (double) clktck);
  printf("用户CPU时间:  %7.2f\n",(tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
  printf("系统CPU时间:   %7.2f\n",(tmsend->tms_stime - tmsstart->tms_stime) / (double) clktck);
  printf("子进程时钟时间:  %7.2f\n",(tmsend->tms_cutime - tmsstart->tms_cutime) / (double) clktck);
  printf("子进程系统CPU时间:   %7.2f\n",(tmsend->tms_cstime - tmsstart->tms_cstime) / (double) clktck);
}
```
