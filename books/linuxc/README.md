# Linux C 编程从基础到实践

程国钢. 张玉兰. Linux C 编程从基础到实践. 清华大学出版社. 第一版

在图书馆偶然发现的书。正好现在学了操作系统，也会用一些 Web 框架，但是这二者之间的桥梁， Linux 的系统调用以及其他面向用户态的 API ，值得先了解一下。

因此准备花个 6 天左右的时间，把重点的几章看看，也不需要记录详细的 API （毕竟有大量 Linux 文档），就是记录个思想、留个印象。

书中提供的代码我放在 [./sample/](./sample/) 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [杂记](#杂记)
- [第 6 章 Linux 的流](#第-6-章-linux-的流)
- [第 7 章 Linux 的进程](#第-7-章-linux-的进程)
- [第 8 章 Linux 的信号](#第-8-章-linux-的信号)
- [第 9 章 Linux 的进程同步机制——管道和 IPC](#第-9-章-linux-的进程同步机制管道和-ipc)
- [第 10 章 Linux 的线程](#第-10-章-linux-的线程)
- [第 11 章 Linux 的网络编程](#第-11-章-linux-的网络编程)

<!-- /code_chunk_output -->

### 杂记

[./drafts/linuxc.00.md](./drafts/linuxc.00.md)
- [关于 make 的一个案例：使用变量、自动变量、隐式规则、模式规则](./drafts/linuxc.00.md#关于-make-的一个案例使用变量-自动变量-隐式规则-模式规则)

### 第 6 章 Linux 的流

总体上给我感觉就是实践中很常见的“文件读写”。毕竟工作中都不会直接 `open` ，而是采用带有缓冲区的 `fopen` 系列。

[./drafts/linuxc.01.stream.md](./drafts/linuxc.01.stream.md)
- [流可以理解为由系统分配的内存缓冲区](./drafts/linuxc.01.stream.md#流可以理解为由系统分配的内存缓冲区)
- [流的基础操作（读写、按行读写、按二进制读写）](./drafts/linuxc.01.stream.md#流的基础操作读写-按行读写-按二进制读写)
- [流的定位](./drafts/linuxc.01.stream.md#流的定位)
- [流的缓冲管理](./drafts/linuxc.01.stream.md#流的缓冲管理)
- [流的格式化输出](./drafts/linuxc.01.stream.md#流的格式化输出)

### 第 7 章 Linux 的进程

最大的收获莫过于认识了会话 session 与前台进程组。

[./drafts/linuxc.02.process.md](./drafts/linuxc.02.process.md)
- [进程的状态、描述符、标识符](./drafts/linuxc.02.process.md#进程的状态-描述符-标识符)
- [进程的操作： fork, exec, vfork, _exit, atexit](./drafts/linuxc.02.process.md#进程的操作-fork-exec-vfork-_exit-atexit)
- [Linux 的进程组和会话（前台进程组）](./drafts/linuxc.02.process.md#linux-的进程组和会话前台进程组)
- [更改进程用户、 system 、 acct 、 time](./drafts/linuxc.02.process.md#更改进程用户-system-acct-time)

### 第 8 章 Linux 的信号

[./drafts/linuxc.03.signal.md](./drafts/linuxc.03.signal.md)
- [Linux 信号/软中断执行过程](./drafts/linuxc.03.signal.md#linux-信号软中断执行过程)
- [注册信号 singal, sigaction](./drafts/linuxc.03.signal.md#注册信号-singal-sigaction)
- [发送信号 kill, raise, sigqueue](./drafts/linuxc.03.signal.md#发送信号-kill-raise-sigqueue)
- [定时信号 alarm, 退出信号 abort](./drafts/linuxc.03.signal.md#定时信号-alarm-退出信号-abort)
- [Linux 信号集 sigemptyset, sigfillset, sigaddset, sigdelset, sigismember](./drafts/linuxc.03.signal.md#linux-信号集-sigemptyset-sigfillset-sigaddset-sigdelset-sigismember)
- [信号阻塞和挂起 sigprocmask, sigsuspend 、信号的精确定时 setitimer 以及避免在信号处理中调用不可重入函数](./drafts/linuxc.03.signal.md#信号阻塞和挂起-sigprocmask-sigsuspend-信号的精确定时-setitimer-以及避免在信号处理中调用不可重入函数)

### 第 9 章 Linux 的进程同步机制——管道和 IPC

[./drafts/linuxc.04.ipc.md](./drafts/linuxc.04.ipc.md)
- [Linux 的管道](./drafts/linuxc.04.ipc.md#linux-的管道)
- [Linux 的命名管道/先进先出队列 tee 命令](./drafts/linuxc.04.ipc.md#linux-的命名管道先进先出队列-tee-命令)
- [System V IPC 以及 ftok, ipcs 命令](./drafts/linuxc.04.ipc.md#system-v-ipc-以及-ftok-ipcs-命令)
- [Linux 的消息队列](./drafts/linuxc.04.ipc.md#linux-的消息队列)
- [Linux 的信号量](./drafts/linuxc.04.ipc.md#linux-的信号量)
- [Linux 的共享内存](./drafts/linuxc.04.ipc.md#linux-的共享内存)

### 第 10 章 Linux 的线程

### 第 11 章 Linux 的网络编程
