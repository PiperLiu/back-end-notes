# 但是，I/O多路复用中是如何判断文件“可读”/“可写”的？

在学习I/O多路复用时，经常会得到如下描述：

> ...，在其中任何一个或多个描述符 **准备好进行 I/O 操作（可读、可写或异常）时获得通知** 。

那么，操作系统内核到底是如何判断某个文件描述符“可读”/“可写”呢？在达到相关状态后，是如何“立即”通知到应用程序的呢？本文在探究这个问题。

### I/O 多路复用与文件描述符状态检测

在进行网络编程或处理其他类型的 I/O 操作时，一个常见的挑战是如何高效地管理多个并发的 I/O 通道。如果为每个连接或文件都创建一个单独的线程或进程来阻塞等待 I/O，当连接数非常多时，系统资源的开销（如内存、上下文切换成本）会变得非常巨大。

I/O 多路复用 (I/O Multiplexing) 技术应运而生，它允许单个进程或线程监视多个 文件描述符 (file descriptor)，并在其中任何一个或多个变得“就绪”（例如，可读或可写）时得到通知，从而可以在单个执行流中处理多个 I/O 事件。Linux 提供了几种经典的 I/O 多路复用系统调用，主要是 `select`、`poll` 和 `epoll`。

要理解这些系统调用如何工作，关键在于理解 Linux 内核是如何跟踪和通知文件描述符状态变化的。这涉及到内核中的文件系统抽象、网络协议栈以及一种核心机制： **等待队列 (wait queue)** 。

#### 文件描述符与内核结构

在 Linux 中，“一切皆文件”是一个核心设计哲学。无论是磁盘文件、管道、终端还是网络套接字 (socket)，在用户空间看来，它们都通过一个非负整数来标识，即文件描述符。

当应用程序通过 `socket()` 系统调用创建一个套接字时，内核会执行以下关键步骤：
1.  在内核空间创建表示该套接字的核心数据结构，通常是 `struct socket`。这个结构包含了套接字的状态、类型、协议族、收发缓冲区、指向协议层处理函数的指针等信息。
2.  创建一个 `struct file` 结构。这是内核中代表一个打开文件的通用结构，它包含访问模式、当前偏移量等，并且有一个重要的成员 `f_op`，指向一个 `file_operations` 结构。
3.  `file_operations` 结构包含了一系列函数指针，定义了可以对这类文件执行的操作，如 `read`、`write`、`poll`、`release` 等。对于套接字，`struct file` 会通过其私有数据指针 (`private_data`) 关联到对应的 `struct socket`，并且其 `f_op` 会指向一套适用于套接字的文件操作函数集。
4.  内核在当前进程的文件描述符表中找到一个空闲位置，将该位置指向新创建的 `struct file` 结构，并将该位置的索引（即文件描述符）返回给用户空间。

因此，后续所有对该文件描述符的操作（如 `read`, `write`, `bind`, `listen`, `accept`, `select`, `poll`, `epoll_ctl` 等），都会通过系统调用进入内核，内核根据文件描述符找到对应的 `struct file`，再通过 `f_op` 调用相应的内核函数来执行。

#### 等待队列：事件通知的核心

操作系统需要一种机制，让某个进程在等待特定事件（例如，数据到达套接字、套接字发送缓冲区有可用空间）发生时能够暂停执行（睡眠），并在事件发生后被唤醒。这就是 **等待队列 (wait queue)** 机制 (`wait_queue_head_t` 在 Linux 内核中)。

```c
struct list_head {
	struct list_head *next, *prev;
};

struct wait_queue_head {
	spinlock_t		lock;
	struct list_head	head;
};
typedef struct wait_queue_head wait_queue_head_t;

struct wait_queue_entry {
	unsigned int		flags;
	void			*private;
	wait_queue_func_t	func;
	struct list_head	entry;
};
typedef struct wait_queue_entry wait_queue_entry_t;
```

等待队列 (`wait_queue_head_t`) 是 Linux 内核中的一个数据结构，用于管理一组等待特定事件（如数据到达）的进程。它本质上是一个链表，链表中的每个节点 (`wait_queue_entry_t`) 代表一个等待该事件的进程。
- `wait_queue_head_t`：包含一个锁（保护队列）和一个指向等待条目链表的指针。
- `wait_queue_entry_t`：包含进程的标识（如任务结构体 `task_struct`）和指向下一个条目的指针。

等待队列允许进程在事件未发生时暂停执行，并在事件发生时被唤醒，是阻塞式 I/O 和多路复用的基础。

内核中几乎所有可能导致阻塞等待的资源（如套接字的接收缓冲区、发送缓冲区、管道、锁等）都会关联一个或多个等待队列。

**数据结构中的嵌入**

每个资源在内核中都有对应的数据结构。例如，对于网络套接字，内核维护一个 `struct socket` 结构，其中嵌入了与接收缓冲区和发送缓冲区相关的等待队列。通常，每个套接字会有两个独立的 `wait_queue_head_t`：
- 一个用于接收数据（等待接收缓冲区有数据）。
- 一个用于发送数据（等待发送缓冲区有空间）。

这些等待队列是 `struct socket` 或相关结构（如 `struct sock`）的成员变量，直接与资源绑定。

当进程对资源执行操作（例如通过 `read()` 读取套接字数据）时，如果资源不可用（接收缓冲区为空），内核会：
1. 内核创建一个 `wait_queue_entry_t`，关联到当前进程的 `task_struct`。
2. 将这个条目加入与接收缓冲区相关的等待队列。
3. 将进程状态设置为睡眠（`TASK_INTERRUPTIBLE` 或 `TASK_UNINTERRUPTIBLE`），然后调用调度器 `schedule()`，让出 CPU，运行其他进程。

对于套接字，`struct sock`（TCP/IP 协议栈中的核心结构）包含字段如 `sk_sleep`，它指向一个等待队列。当接收缓冲区为空时，进程会被加入这个队列；当数据到达时，协议栈会操作这个队列来唤醒进程。

**唤醒过程是如何实现的？**

当网络协议栈（如 TCP/IP）收到数据并将其放入套接字的接收缓冲区时：
1. **事件检测：** 协议栈代码检测到接收缓冲区从空变为非空。
2. **检查等待队列：** 协议栈访问与该缓冲区关联的 `wait_queue_head_t`，检查是否有进程在等待。
3. **调用 `wake_up()`：** 如果队列不为空，协议栈调用内核函数 `wake_up()`（或其变体，如 `wake_up_interruptible()`）。
4. **`wake_up()` 的工作：**
- 遍历等待队列中的每个 `wait_queue_entry_t`。
- 将对应的进程状态从睡眠改为 `TASK_RUNNING`。
- 将这些进程加入 CPU 的运行队列，等待调度器重新调度它们。

```txt
进程 A 调用 read(sockfd) -> 接收缓冲区为空
  -> 加入等待队列 -> 进程睡眠
数据到达 -> 协议栈放入缓冲区 -> 调用 wake_up()
  -> 进程 A 被唤醒 -> 重新调度 -> read() 返回数据
```

**可读性**

当一个套接字接收到数据时，网络协议栈（如 TCP/IP 栈）处理完数据包后，会将数据放入该套接字的接收缓冲区。如果此时有进程正在等待该套接字变为可读（即接收缓冲区中有数据），协议栈代码会 **唤醒** (wake up) 在该套接字接收缓冲区关联的等待队列上睡眠的所有进程。

**可写性**

当应用程序通过 `write()` 或 `send()` 发送数据时，数据首先被复制到套接字的发送缓冲区。网络协议栈随后从缓冲区取出数据并发送到网络。当数据成功发送出去，或者发送缓冲区中的空间被释放到某个阈值以上时，协议栈代码会 **唤醒** 在该套接字发送缓冲区关联的等待队列上睡眠的所有进程，通知它们现在可以写入更多数据了。

假设发送缓冲区大小为 8KB，高水位标记为 6KB：
- 写入 8KB 数据 -> 缓冲区满 -> 进程睡眠。
- 协议栈发送 3KB 数据 -> 剩余 5KB（低于高水位） -> 唤醒进程 -> 可写。

这个“生产者-消费者”模型（网络栈是数据的生产者/消费者，应用程序是数据的消费者/生产者）通过等待队列和唤醒机制实现，是理解 I/O 事件通知的基础。

#### `select` 和 `poll` 的工作原理

`select` 和 `poll` 是较早的 I/O 多路复用接口。它们的工作方式类似：

1.  **用户调用** ：应用程序准备好要监视的文件描述符集合（`select` 使用 `fd_set`，`poll` 使用 `struct pollfd` 数组），并指定关心的事件类型（可读、可写、异常），然后调用 `select` 或 `poll` 系统调用。
2.  **内核操作** ：
    * 内核接收到文件描述符列表和关心的事件。
    * 内核遍历应用程序提供的 **每一个** 文件描述符。
    * 对于每个文件描述符，内核找到对应的 `struct file`，然后调用其 `file_operations` 结构中的 `poll` 方法（例如，对于套接字，最终会调用到类似 `sock_poll` 的函数）。
    * 该 `poll` 方法执行两个关键任务：
        * **检查当前状态** ：立即检查该文件描述符的当前状态是否满足用户请求的事件（例如，接收缓冲区是否非空？发送缓冲区是否有足够空间？是否有错误？）。如果满足，就标记该文件描述符为就绪。
        * **注册等待** ：如果当前状态不满足，并且调用者准备阻塞等待，则该 `poll` 方法会将当前进程添加到与所关心事件相关的 **等待队列** 上。这是通过内核函数 `poll_wait()` 实现的，它并不直接使进程睡眠，只是建立一个关联：如果未来该等待队列被唤醒，当前正在执行 `select`/`poll` 的进程也应该被唤醒。
    * 遍历完所有文件描述符后，如果发现至少有一个文件描述符是就绪的，`select`/`poll` 就将就绪信息返回给应用程序。
    * 如果没有文件描述符就绪，并且设置了超时时间，则进程会 **睡眠** （阻塞），直到以下任一情况发生：
        * 某个被监视的文件描述符相关的等待队列被唤醒（例如，因为数据到达或缓冲区变空）。
        * 超时时间到达。
        * 收到一个信号。
    * 当进程被唤醒后（如果是因等待队列事件唤醒），内核并 **不知道** 是哪个具体的文件描述符导致了唤醒。因此，内核需要 **重新遍历一遍** 所有被监视的文件描述符，再次调用它们的 `poll` 方法检查状态，找出哪些现在是就绪的，然后将结果返回给用户。

解释一下 `poll_wait()` ：它建立了一种关联：当等待队列被唤醒时，当前执行 `select` 或 `poll` 的进程也会被唤醒。
- 在 `select` 或 `poll` 的内核实现中，对于每个文件描述符，内核调用其 `file_operations` 中的 `poll` 方法（例如 `sock_poll`）。
- 在 `poll` 方法中，如果事件尚未就绪（例如接收缓冲区为空），会调用 `poll_wait(file, wait_queue_head_t, poll_table)`，`poll_wait()` 中创建一个 `wait_queue_entry_t`，关联到当前进程:
  - `file`：文件描述符对应的 `struct file`。
  - `wait_queue_head_t`：与事件（如接收缓冲区）关联的等待队列。
  - `poll_table`：`select`/`poll` 传入的临时结构，用于收集等待队列。
- `poll_wait()` 只是注册关联，不会直接调用 `schedule()` 使进程睡眠。睡眠是在 `select`/`poll` 遍历所有文件描述符后统一处理的。

```txt
select(fd_set) -> 内核遍历 fd
  -> fd1: poll() -> poll_wait(接收队列) -> 注册进程
  -> fd2: poll() -> poll_wait(发送队列) -> 注册进程
无就绪 fd -> 进程睡眠
数据到达 fd1 -> wake_up(接收队列) -> 进程唤醒 -> select 返回
```

**`select` 和 `poll` 的主要缺点**

* **效率问题** ：每次调用都需要将整个文件描述符集合从用户空间拷贝到内核空间。更重要的是，内核需要线性遍历所有被监视的文件描述符来检查状态和注册等待，唤醒后还需要再次遍历来确定哪些就绪。当监视的文件描述符数量 N 很大时，这个 O(N) 的开销变得非常显著。
* `select` 有最大文件描述符数量的限制（通常由 `FD_SETSIZE` 定义）。`poll` 没有这个限制，但仍有上述效率问题。

#### `epoll`：更高效的事件通知

`epoll` 是 Linux 对 `select` 和 `poll` 的重大改进，旨在解决大规模并发连接下的性能瓶颈。它采用了一种不同的、基于 **回调 (callback)** 的事件驱动机制：

1.  **`epoll_create()` / `epoll_create1()`**：创建一个 `epoll` 实例。这会在内核中创建一个特殊的数据结构，用于维护两个列表：
    * **监视列表 (Interest List)** ：通常使用高效的数据结构（如红黑树或哈希表）存储所有用户通过 `epoll_ctl` 添加的、需要监视的文件描述符及其关心的事件。
    * **就绪列表 (Ready List)** ：一个链表，存储那些已经被内核检测到发生就绪事件、但尚未被 `epoll_wait` 报告给用户的文件描述符。

这个 `epoll` 实例本身也由一个文件描述符表示。

2.  **`epoll_ctl()`** ：用于向 `epoll` 实例的监视列表添加 (`EPOLL_CTL_ADD`)、修改 (`EPOLL_CTL_MOD`) 或删除 (`EPOLL_CTL_DEL`) 文件描述符。
    * **关键操作**：当使用 `EPOLL_CTL_ADD` 添加一个文件描述符 `fd` 时，内核不仅将其加入 `epoll` 实例的监视列表，更重要的是，它会在与 `fd` 相关的 **等待队列** 上注册一个 **回调函数**。
    * 这个回调函数非常特殊：当 `fd` 对应的资源（如套接字缓冲区）状态改变，导致其关联的等待队列被唤醒时，这个注册的回调函数会被执行。
    * 回调函数的任务是：检查 `fd` 的当前状态是否匹配 `epoll` 实例对其关心的事件。如果匹配，就将这个 `fd` 添加到 `epoll` 实例的 **就绪列表** 中。如果此时有进程正在 `epoll_wait` 中睡眠等待该 `epoll` 实例，则唤醒该进程。

```txt
+-----------------+      epoll_ctl(ADD fd)      +----------------------------+
| epoll instance  | <-------------------------- | Application Process        |
| - Interest List |                             +----------------------------+
| - Ready List    |                                          |
| - Wait Queue    |      Registers Callback                  | system call
+-----------------+---------------------------> +----------------------------+
                                                | Kernel                     |
                                                | +------------------------+ |
                                                | | struct file (for fd)   | |
                                                | | - f_op                 | |
                                                | | - private_data (socket)| |
                                                | +---------+--------------+ |
                                                |           |                |
                                                |           v                |
                                                | +---------+-------------+  |
                                                | | Wait Queue (e.g., rx) |  |
                                                | | - epoll callback entry|  |
                                                | +-----------------------+  |
                                                +----------------------------+
```

3.  **`epoll_wait()`**：等待 `epoll` 实例监视的文件描述符上发生事件。
    * 调用 `epoll_wait` 时，内核首先检查 `epoll` 实例的 **就绪列表**。
    * 如果就绪列表 **非空**，内核直接将就绪列表中的文件描述符信息拷贝到用户空间提供的缓冲区，并立即返回就绪的文件描述符数量。
    * 如果就绪列表 **为空**，进程将 **睡眠**，等待在 `epoll` 实例自身的等待队列上。
    * 当某个被监视的文件描述符 `fd` 发生事件（如数据到达），其关联的等待队列被唤醒，触发之前注册的 `epoll` 回调。
    * 回调函数将 `fd` 加入 `epoll` 实例的就绪列表，并唤醒在 `epoll_wait` 中等待该实例的进程。
    * `epoll_wait` 被唤醒后，发现就绪列表非空，于是收集就绪信息并返回给用户。

**`epoll` 的优势**

* **高效** ：`epoll_wait` 的复杂度通常是 O(1)，因为它只需要检查就绪列表，而不需要像 `select`/`poll` 那样遍历所有监视的文件描述符。文件描述符状态的检查和就绪列表的填充是由事件发生时的回调机制异步完成的。
* **回调机制** ：避免了 `select`/`poll` 在每次调用和唤醒时都需要重复遍历所有文件描述符的问题。文件描述符和 `epoll` 实例的关联（包括回调注册）只需要在 `epoll_ctl` 时建立一次。
* **边缘触发 (Edge Triggered, ET) 与水平触发 (Level Triggered, LT)** ：`epoll` 支持这两种模式。LT 模式（默认）行为类似于 `poll`，只要条件满足（如缓冲区非空），`epoll_wait` 就会一直报告就绪。ET 模式下，只有当状态从未就绪变为就绪时，`epoll_wait` 才会报告一次，之后即使条件仍然满足也不会再报告，直到应用程序处理了该事件（例如，读取了所有数据使得缓冲区变空，然后又有新数据到达）。ET 模式通常能提供更高的性能，但编程也更复杂，需要确保每次事件通知后都将数据处理完毕。

**LT 模式（默认）**

```c
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int epfd = epoll_create1(0);
    int sockfd = /* 假设已创建并绑定监听的套接字 */;

    struct epoll_event event;
    event.events = EPOLLIN; // LT 模式，默认
    event.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);

    struct epoll_event events[10];
    while (1) {
        int nfds = epoll_wait(epfd, events, 10, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sockfd) {
                char buf[1024];
                ssize_t n = read(sockfd, buf, sizeof(buf));
                if (n > 0) {
                    printf("Read %zd bytes\n", n);
                    // 可只读部分数据，下次仍会触发
                } else if (n == 0) {
                    printf("Connection closed\n");
                }
            }
        }
    }
}
```

**ET 模式**

```c
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main() {
    int epfd = epoll_create1(0);
    int sockfd = /* 假设已创建并绑定监听的套接字 */;

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET; // ET 模式
    event.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);

    struct epoll_event events[10];
    while (1) {
        int nfds = epoll_wait(epfd, events, 10, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sockfd) {
                while (1) { // 必须一次性读完
                    char buf[1024];
                    ssize_t n = read(sockfd, buf, sizeof(buf));
                    if (n > 0) {
                        printf("Read %zd bytes\n", n);
                    } else if (n == 0) {
                        printf("Connection closed\n");
                        break;
                    } else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            printf("Buffer drained\n");
                            break;
                        }
                    }
                }
            }
        }
    }
}
```

#### 连接套接字（`socket`, `bind`, `listen`, `accept`）与就绪状态

现在我们将这些概念与服务器套接字的工作流程联系起来：

* **`socket()`** : 创建一个套接字文件描述符 `sockfd`。此时它通常既不可读也不可写。
* **`bind()`** : 将 `sockfd` 绑定到一个本地地址和端口。这本身通常不改变其可读写状态。
* **`listen()`** : 将 `sockfd` 标记为监听套接字，并创建两个队列（SYN 队列和 Accept 队列）。此时 `sockfd` 仍不可直接读写数据。

**何时监听套接字 `sockfd` 变为“可读”？**

当一个客户端连接请求完成 TCP 三次握手后，内核会创建一个代表这个新连接的 **已完成连接** (established connection)，并将其放入与监听套接字 `sockfd` 关联的 **Accept 队列** 中。此时，对于 `select`/`poll`/`epoll` 来说，监听套接字 `sockfd` 就被认为是 **可读** 的。调用 `accept(sockfd, ...)` 将会从 Accept 队列中取出一个已完成连接，并返回一个 **新的** 文件描述符 `connfd`，这个 `connfd` 才代表了与客户端的实际通信通道。如果 Accept 队列为空，则监听套接字 `sockfd` 不可读。

* **`accept()`**: 从监听套接字的 Accept 队列中取出一个已完成的连接，返回一个新的已连接套接字 `connfd`。

**何时已连接套接字 `connfd` 变为“可读”？**

当内核的网络协议栈收到属于 `connfd` 这个连接的数据，并将数据放入其 **接收缓冲区** 后，`connfd` 就变为可读。此时，网络栈会唤醒在该套接字接收缓冲区等待队列上的进程（包括通过 `epoll` 注册的回调）。

**何时已连接套接字 `connfd` 变为“可写”？**

当 `connfd` 的 **发送缓冲区** 有足够的可用空间来容纳更多待发送的数据时，`connfd` 就变为可写。当内核成功将发送缓冲区中的数据发送到网络，释放了空间后，会唤醒在该套接字发送缓冲区等待队列上的进程（包括 `epoll` 回调）。初始状态下，新创建的 `connfd` 通常是可写的。

**异常状态** ：通常指带外数据到达，或者发生某些错误（如连接被对方重置 RST）。

**总结**

Linux 内核通过为每个可能阻塞的 I/O 资源（如套接字缓冲区）维护 **等待队列** 来跟踪哪些进程在等待事件。当事件发生时（数据到达、缓冲区变空），内核代码（如网络协议栈）会 **唤醒** 相应等待队列上的进程。

* `select` 和 `poll` 在每次调用时，都需要遍历所有被监视的文件描述符，检查它们的当前状态，并将进程注册到相关的等待队列上。唤醒后还需要再次遍历以确定哪些就绪。
* `epoll` 通过 `epoll_ctl` 预先在文件描述符的等待队列上注册 **回调函数** 。当事件发生并唤醒等待队列时，回调函数被触发，它负责将就绪的文件描述符添加到 `epoll` 实例的 **就绪列表** 中，并唤醒等待在 `epoll_wait` 上的进程。`epoll_wait` 只需检查这个就绪列表即可，大大提高了效率。

理解等待队列和唤醒机制，以及 `epoll` 基于回调的事件驱动模型，是掌握 Linux 下高性能网络编程和 I/O 多路复用技术的关键。

### 总结

- **可读**
  - 监听套接字：Accept 队列非空（有新连接）。
  - 已连接套接字：接收缓冲区有数据。
  - 内核通过网络协议栈监控缓冲区状态。
- **可写**
  - 发送缓冲区有足够空间（低于高水位）。
  - 协议栈监控发送进度并更新空间。

如何“立即”通知应用程序？
- **等待队列机制：** 资源状态变化时，协议栈调用 `wake_up()` 唤醒等待队列上的进程。
- **`select`/`poll`：** 通过 `poll_wait()` 注册等待，事件发生时唤醒并重新检查状态。
- **`epoll`：** 通过回调函数异步将就绪文件描述符加入就绪列表，`epoll_wait` 直接返回。
