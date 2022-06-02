# 第 9 章 Linux 的进程同步机制——管道和 IPC

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Linux 的管道](#linux-的管道)
- [Linux 的命名管道/先进先出队列 tee 命令](#linux-的命名管道先进先出队列-tee-命令)
- [System V IPC 以及 ftok, ipcs 命令](#system-v-ipc-以及-ftok-ipcs-命令)
- [Linux 的消息队列](#linux-的消息队列)
- [Linux 的信号量](#linux-的信号量)
- [Linux 的共享内存](#linux-的共享内存)

<!-- /code_chunk_output -->

### Linux 的管道

匿名管道是老朋友了，这里记录一些 Linux 给出的函数便是。

```c
#include <unistd.h>
int pipe(int pipefd[2]);
```

### Linux 的命名管道/先进先出队列 tee 命令

命名管道我目前粗浅的理解是：
- 用 `mkfifo` 创建一个命名管道（其实是一个文件系统中的文件）
- 把这个命名管道当成文件来操作就好，只不过读端和写端会因为命名管道的读写关系形成阻塞（就像管道一样）

```c
#include <sys/types.h>
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
```

这里额外记录一个 `tee` 命令：用于读取标准输入的数据，并将其内容输出成文件。

```bash
tee [-ai][--help][--version][文件...]
```

### System V IPC 以及 ftok, ipcs 命令

System V IPC 机制是 Linux 从 Unix 继承的进程间通信机制，由消息队列、信号量以及共享内存三种具体实现方法组成。

| 实现方式 | 头文件    | 创建或打开IPC | IPC控制函数 | IPC操作函数    |
|----------|-----------|---------------|-------------|----------------|
| 消息队列 | sys/msg.h | msgget        | msgctl      | msgsnd, msgrcv |
| 信号量   | sys/sem.h | semget        | semctl      | semop          |
| 共享内存 | sys/shm.h | shmget        | shmctl      | shmat, shmdt   |

每个 IPC 结构都对应一个唯一标识符和关键字。关键字可以由 `ftok` 生成。

```c
#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok(const char *pathname, int proj_id);
```

每个 IPC 对象都有一个 `ipc_perm` 结构，用于描述 IPC 的权限等信息。

```c
struct ipc_perm {
    uid_t uid;
    gid_t gid;
    uid_t cuid;
    mode_t mode;
    ulong seq;
    key_t key;
}
```

可以使用 `ipcs` 查看内核中 IPC 的状态。

```bash
# https://geek-docs.com/linux-cmd/linux-performance-monitor-related-cmd/linux-cmd-ipcs.html
[root@rhel ～]# ipcs
------ Shared Memory Segments --------
key       shmid     owner     perms     bytes     nattch    status
0x6c6c6536 0         root      600       4096      0
0x0000000032769     gdm       600       393216    2         dest
0x0000000065538     gdm       600       393216    2         dest
0x0000000098307     gdm       600       393216    2         dest
------ Semaphore Arrays --------
key       semid     owner     perms     nsems
0x00000000 0         root      600       1
0x0000000032769     root      600       1
0x0000000065538     apache    600       1
0x0000000098307     apache    600       1
0x00000000131076    apache    600       1
0x00000000163845    apache    600       1
0x00000000196614    apache    600       1
------ Message Queues --------
key       msqid     owner     perms     used-bytes  messages
```

### Linux 的消息队列

我理解和命名管道都是 FIFO 的，只不过不是文件系统的一部分了，而是内核中的一部分；而且不会阻塞了（少了打开和关闭管道时的复杂性）。

参考 [CSDN](https://blog.csdn.net/ttyue_123/article/details/52370676)：
- 与命名管道相比：
  - 消息队列的优势在于，它独立于发送和接收进程而存在，这消除了在同步命名管道的打开和关闭时可能产生的一些困难。
  - 消息队列提供了一种从一个进程向另一个进程发送一个数据块的方法。
  - 而且，每个数据块被认为含有一个类型，接收进程可以独立地接收含有不同类型值的数据块。
- 优点：
  - 我们可以通过发送消息来几乎完全避免命名管道的同步和阻塞问题。
  - 我们可以用一些方法来提前查看紧急消息。
- 缺点：
  - 与管道一样，每个数据块有一个最大长度的限制。
  - 系统中所有队列所包含的全部数据块的总长度也有一个上限。

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key, int msgflg);
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
int msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

### Linux 的信号量

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
int semget(key_t key, int nsems, int semflg);
int semop(int semid, struct sembuf *sops, unsigned nsops);
int semctl(int semid, int semnum, int cmd, union semun arg);
```

信号量更多地是辅助共享内存使用。

### Linux 的共享内存

```c
#include <sys/types.h>
#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
void *shmat(int shmid, const void *shmaddr, int shmflg);
int shmdt(const void *shmaddr);
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

还是参考 [CSDN](https://blog.csdn.net/ttyue_123/article/details/52370676)：因为系统内核没有对访问共享内存进行同步，您必须提供自己的同步措施。例如，在数据被写入之前不允许进程从共享内存中读取信息、不允许两个进程同时向同一个共享内存地址写入数据等。 **解决这些问题的常用方法是通过使用信号量进行同步。**

共享内存区是最快的可用IPC形式，一旦这样的内存区映射到共享它的进程的地址空间，这些进程间数据的传递就不再通过执行任何进入内核的系统调用来传递彼此的数据，节省了时间。

共享内存和消息队列，FIFO，管道传递消息的区别：
- 消息队列，FIFO，管道的消息传递方式一般为
  - 服务器得到输入
  - 通过管道，消息队列写入数据，通常需要从进程拷贝到内核。
  - 客户从内核拷贝到进程
  - 然后再从进程中拷贝到输出文件
- 上述过程通常要经过4次拷贝，才能完成文件的传递。共享内存只需要
  - 从输入文件到共享内存区
  - 从共享内存区输出到文件
