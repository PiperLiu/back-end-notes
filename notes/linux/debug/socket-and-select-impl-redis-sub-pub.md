
# Linux 网络编程：从 Socket API 到极简 Redis 发布/订阅 sub/pub 服务的实现

### 引言

本文旨在系统性地阐述 Linux 环境下的网络编程基础，重点关注 Socket 应用程序接口（API）的原理与应用。通过循序渐进的方式，结合具体的 C 语言代码示例，我们将剖析核心系统调用的机制，并最终构建一个基于传输控制协议（TCP）的简化版发布/订阅（Publish/Subscribe, Pub/Sub）服务器模型。

在进行深入探讨之前，本文假设读者已具备以下先验知识：

1.  **C 语言编程能力** ：熟悉 C 语言的核心语法、指针操作及内存管理机制。
2.  **Linux 操作系统基础** ：了解 Linux 基本命令行操作及 C 程序的编译流程（例如，使用 GCC 工具链）。
3.  **计算机网络基础** ：对 OSI 参考模型或 TCP/IP 协议栈有概念性认识，理解 TCP 与 UDP 的核心差异（面向连接与无连接、可靠性保证机制等），并掌握 IP 地址和端口号在网络通信中的作用。

网络编程的本质是实现进程间通信（Inter-Process Communication, IPC），特别是跨越主机边界的分布式通信。在 Linux 及其他类 Unix 操作系统中，Socket（套接字）接口是实现网络通信的标准范式。它提供了一套抽象的 API，允许应用程序将网络通信视为一种特殊的文件 I/O 操作，从而简化了网络数据收发的复杂性。

本文的实践目标是构建一个功能类似于 Redis 服务器 `PUBLISH` 和 `SUBSCRIBE` 命令的简化服务实例：

* 服务器进程监听一个预定义的 TCP 端口。
* 允许多个客户端并发连接至服务器。
* 客户端通过发送特定格式的命令（如 `SUB <topic_name>`）订阅感兴趣的主题。
* 客户端通过发送特定格式的命令（如 `PUB <topic_name> <message_data>`）向指定主题发布消息。
* 服务器负责将发布的消息转发给所有订阅了对应主题的客户端（通常不包括发布者自身）。

接下来，我们将逐步解析相关的系统调用和编程技术。

### Linux 上的 Socket API

在 Linux 操作系统的设计哲学中，“一切皆文件”是一个核心概念。网络连接在内核层面被抽象为一种特殊的文件类型，并通过文件描述符（File Descriptor）进行管理。`socket()` 系统调用是创建此类“网络文件”的入口点，用于在网络通信的参与方创建一个通信端点（Endpoint）。

#### `socket()` 系统调用详解

`socket()` 的主要功能是在内核中创建一个新的、未连接的套接字，并返回一个与之关联的文件描述符，供用户空间程序使用。其函数原型定义于 `<sys/socket.h>` 头文件中：

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

**参数说明** ：

* **`domain`** (地址族 Address Family - AF): 指定套接字使用的协议族。常用取值包括：
    * `AF_INET`: 用于 IPv4 网络协议通信。本文主要采用此地址族。
    * `AF_INET6`: 用于 IPv6 网络协议通信。
    * `AF_UNIX` (或 `AF_LOCAL`): 用于同一主机内部的进程间通信，依赖本地文件系统路径而非网络地址。
* **`type`** (套接字类型 Socket Type): 定义套接字的通信语义。关键取值有：
    * `SOCK_STREAM`: 提供面向连接、可靠的、基于字节流的传输服务。TCP 协议即为此类型，保证数据传输的顺序性和可靠性（通过序列号、确认、重传等机制）。
    * `SOCK_DGRAM`: 提供无连接、不可靠的数据报服务。UDP 协议属此类，数据包传输可能发生丢失、重复或乱序。
* **`protocol`** (协议 Protocol): 通常设置为 0，表示由系统根据指定的 `domain` 和 `type` 自动选择默认协议。例如，`AF_INET` 与 `SOCK_STREAM` 组合通常默认选用 `IPPROTO_TCP`；`AF_INET` 与 `SOCK_DGRAM` 组合则默认选用 `IPPROTO_UDP`。亦可显式指定协议常量（如 `IPPROTO_TCP`）。

**内核操作与数据结构** ：

当应用程序调用 `socket()` 时，会触发一次系统调用，进入内核态执行：

1.  **资源分配** ：内核网络协议栈为新的套接字分配必要的内存资源，创建一个内部的 `struct socket` 或类似的核心数据结构。此结构包含了套接字的状态信息（如初始状态、类型、协议族）、发送和接收缓冲区、指向特定协议（如 TCP、UDP）处理函数的指针、以及等待队列等。
2.  **文件描述符关联** ：内核在当前进程的文件描述符表中找到一个未使用的条目，并将该条目指向一个代表该套接字的内核`文件对象`（`struct file`）。这个文件描述符（一个非负整数）是用户空间程序操作该套接字的句柄。
3.  **返回** ：系统调用返回新分配的文件描述符给应用程序。若创建失败（如资源不足、权限问题），则返回 -1，并设置全局变量 `errno` 以指示具体错误代码。

**返回值**：

* 成功：返回一个新的文件描述符（非负整数）。
* 失败：返回 -1，并设置 `errno`。

**示例：创建 IPv4 TCP 套接字**

```c
#include <stdio.h>      // 标准输入输出
#include <stdlib.h>     // 标准库函数，如 exit
#include <sys/socket.h> // 套接字核心函数和数据结构
#include <errno.h>      // 错误码 errno
#include <unistd.h>     // close 函数

int main() {
    int sockfd;

    // 创建一个用于 IPv4 TCP 通信的套接字
    // AF_INET: 指定使用 IPv4 协议族
    // SOCK_STREAM: 指定使用面向连接的字节流服务 (TCP)
    // 0: 让内核自动选择合适的协议 (对于 AF_INET 和 SOCK_STREAM，通常是 IPPROTO_TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 检查 socket() 调用是否成功
    if (sockfd == -1) {
        perror("socket 创建失败"); // perror 会根据当前的 errno 值打印错误信息
        exit(EXIT_FAILURE);       // 异常退出程序
    }

    printf("套接字创建成功! 文件描述符: %d\n", sockfd);

    // 在实际应用中，套接字使用完毕后应显式关闭
    // close(sockfd);

    return 0;
}
```

**编译与执行**：

```bash
gcc create_socket_example.c -o create_socket_example
./create_socket_example
套接字创建成功! 文件描述符: 4
```

输出将提示套接字创建成功，并显示其文件描述符。

#### `sockaddr_in` 结构体与地址表示

仅创建套接字不足以进行通信，服务器端需要将其绑定到具体的本地网络地址（IP 地址和端口号）。在 IPv4 环境下，此地址信息通过 `struct sockaddr_in` 结构体（定义于 `<netinet/in.h>`）来承载：

```c
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t    sin_family; /* 地址族: 固定为 AF_INET */
    in_port_t      sin_port;   /* 端口号 (网络字节序) */
    struct in_addr sin_addr;   /* IPv4 地址 (网络字节序) */
    // char           sin_zero[8]; /* 填充字节，通常不直接使用，应清零 */
};

/* IPv4 地址结构 */
struct in_addr {
    uint32_t       s_addr;     /* 32位IPv4地址 (网络字节序) */
};
```

**关键字段解析**：

* **`sin_family`** : 地址族，对于 IPv4，必须设置为 `AF_INET`，与 `socket()` 调用中的 `domain` 参数保持一致。
* **`sin_port`** : 端口号。 **注意** ：该字段必须存储为 **网络字节序** （Network Byte Order，即大端序 Big-Endian）。应用程序需使用 `htons()` (Host to Network Short) 函数将主机字节序（Host Byte Order）的端口号转换为网络字节序。例如，`htons(8080)`。
* **`sin_addr.s_addr`** : 32位 IPv4 地址。 **注意**： 同样必须是 **网络字节序** 。
    * 服务器端执行 `bind()` 时，若希望监听本机所有可用的网络接口，应将此字段设置为 `htonl(INADDR_ANY)`。`INADDR_ANY` 是一个特殊常量（通常为 0），`htonl()` (Host to Network Long) 用于将其转换为主机字节序到网络字节序。
    * 若需绑定到特定 IP 地址（如服务器仅监听某块网卡，或客户端执行 `connect()` 时指定目标服务器 IP），可使用 `inet_pton()` (Presentation to Network) 函数将点分十进制表示的 IP 地址字符串（如 "192.168.1.100"）转换为网络字节序的 32 位整数，并存入 `s_addr`。

**字节序转换函数** （通常定义于 `<arpa/inet.h>`）：

* `htons()`: 主机字节序到网络字节序（16位，用于端口号）。
* `htonl()`: 主机字节序到网络字节序（32位，用于 IPv4 地址）。
* `ntohs()`: 网络字节序到主机字节序（16位）。
* `ntohl()`: 网络字节序到主机字节序（32位）。
* `inet_pton(AF_INET, "ip_string", &addr_struct->sin_addr)`: 将点分十进制 IP 字符串转换为网络字节序二进制形式。
* `inet_ntop(AF_INET, &addr_struct->sin_addr, buffer, buffer_size)`: 将网络字节序二进制 IP 地址转换为点分十进制字符串。

### `bind()`, `listen()`, `accept()` - 服务器端核心调用

对于 TCP 服务器而言，创建套接字后，必须执行一系列步骤来准备接收客户端连接：

1.  **`bind()`** : 将套接字与一个本地 IP 地址和端口号关联起来，定义服务的监听地址。
2.  **`listen()`** : 将套接字设置为监听模式，使其能够接受外来的连接请求，并配置连接请求队列。
3.  **`accept()`** : 从已完成三次握手的连接队列中接受一个连接，并为此连接创建一个新的专用套接字。

#### 1. `bind()` 系统调用

`bind()` 用于将 `socket()` 创建的套接字文件描述符 `sockfd` 与 `my_addr` 指定的本地地址（IP 和端口）进行绑定。

```c
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

* `sockfd`: `socket()` 返回的文件描述符。
* `addr`: 指向包含待绑定地址信息的 `sockaddr` 结构体的指针。对于 IPv4，实际传递的是已填充好的 `struct sockaddr_in` 结构体的地址，需强制类型转换为 `(struct sockaddr *)`。
* `addrlen`: `addr` 指向的结构体的大小，对于 `struct sockaddr_in`，通常为 `sizeof(struct sockaddr_in)`。

**内核操作** ：

调用 `bind()` 进入内核态后：

1.  **地址复制与校验** ：内核将用户空间传入的 `sockaddr` 结构体复制到内核内存。
2.  **状态检查** ：检查 `sockfd` 对应的套接字是否有效且未被绑定。
3.  **地址可用性检查** ：检查指定的 IP 地址和端口号是否可用。对于端口号，检查是否已被其他套接字绑定（除非设置了 `SO_REUSEADDR` 等选项）；对于 IP 地址，检查是否是分配给本机的有效地址（或 `INADDR_ANY`）。
4.  **权限检查** ：检查进程是否有权限绑定到指定端口（通常，绑定到 1024 以下的端口需要超级用户权限）。
5.  **绑定操作** ：如果所有检查通过，内核将该地址信息与内部的套接字结构关联起来。

**返回值** ：成功返回 0；失败返回 -1，并设置 `errno`。常见错误包括 `EADDRINUSE`（地址已在使用）、`EACCES`（权限不足）、`EINVAL`（`sockfd` 无效或已绑定）。

**示例：绑定套接字到本地 8080 端口**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>       // 用于 memset
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>    // 用于 htons, htonl
#include <unistd.h>       // 用于 close

#define PORT 8080

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1; // 用于 setsockopt

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket 创建失败");
        exit(EXIT_FAILURE);
    }

    // 可选: 设置套接字选项，允许地址重用，便于服务器快速重启
    // SO_REUSEADDR 允许重用本地地址 (IP+端口)，尤其是在 TIME_WAIT 状态下的端口
    // SO_REUSEPORT (需要内核支持) 允许多个进程绑定到同一 IP 和端口
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt 失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 准备 sockaddr_in 结构体
    memset(&address, 0, sizeof(address)); // 推荐清零结构体
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); // 监听所有网络接口
    address.sin_port = htons(PORT);              // 监听指定端口 (转换为网络字节序)

    // 将套接字绑定到指定的地址和端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind 失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("套接字成功绑定到端口 %d\n", PORT);

    // ... 后续步骤: listen() 和 accept() ...

    close(server_fd); // 完成后关闭监听套接字

    return 0;
}
```

#### 2. `listen()` 系统调用

`listen()` 用于将一个已绑定的流式套接字（`SOCK_STREAM` 或 `SOCK_SEQPACKET`）转换为被动监听状态，使其能够接受传入的连接请求。

```c
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```

* `sockfd`: 已成功 `bind()` 的套接字文件描述符。
* `backlog`: 指定内核为此监听套接字维护的 **已完成连接队列** （Completed Connection Queue, 或称 Accept Queue）的最大长度。

**内核操作与连接队列** ：

当调用 `listen()` 时：

1.  **状态转换** ：内核将 `sockfd` 对应的内部套接字结构的状态从 `CLOSED`（或 `BOUND`）修改为 `LISTEN`。
2.  **队列初始化** ：内核为该监听套接字关联并初始化两个重要的队列：
    * **未完成连接队列 (Incomplete Connection Queue / SYN Queue)** ：存储收到的 SYN 包，但尚未完成三次握手的连接请求（处于 `SYN_RCVD` 状态）。此队列的大小通常由系统参数（如 `net.ipv4.tcp_max_syn_backlog`）控制，`backlog` 参数对其影响有限或间接。
    * **已完成连接队列 (Completed Connection Queue / Accept Queue)** ：存储已经完成 TCP 三次握手，等待被应用程序通过 `accept()` 提取的连接（这些连接在内核中已是 `ESTABLISHED` 状态，但从服务器监听角度看是在等待 accept）。`backlog` 参数主要限制的是这个队列的大小。当此队列满时，内核可能会拒绝新的已完成握手的连接（例如，不响应 ACK，或发送 RST）。

**返回值** ：成功返回 0；失败返回 -1，并设置 `errno`。

**示例（续上）**：

```c
    // ... bind() 成功后 ...

    // 开始监听传入连接
    // backlog 设置为 10，意味着最多允许 10 个已完成三次握手的连接在队列中等待 accept()
    if (listen(server_fd, 10) < 0) {
        perror("listen 失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("服务器正在端口 %d 上监听...\n", PORT);

    // ... 下一步: accept() ...
```

#### 3. `accept()` 系统调用

`accept()` 从监听套接字 `sockfd` 的已完成连接队列中取出一个连接请求，为该连接创建一个 **新的** 、已连接的套接字，并返回这个新套接字的文件描述符。

```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

* `sockfd`: 处于监听状态的套接字文件描述符。
* `addr`: （可选）指向 `sockaddr` 结构体的指针，用于接收发起连接的客户端的地址信息。若不关心客户端地址，可传递 `NULL`。
* `addrlen`: （可选）指向 `socklen_t` 类型变量的指针。这是一个 **值-结果** 参数：
    * 调用前，`*addrlen` 必须初始化为 `addr` 指向的缓冲区的实际大小。
    * 调用成功后，内核会将客户端地址结构的实际大小写入 `*addrlen`。若传入 `addr` 为 `NULL`，则 `addrlen` 也应为 `NULL`。

**内核操作** ：

1.  **检查队列** ：内核检查与 `sockfd` 关联的已完成连接队列。
2.  **阻塞/非阻塞行为** ：
    * 如果队列为空且 `sockfd` 是阻塞模式（默认），`accept()` 调用将使进程 **睡眠** ，直到队列中出现新的已完成连接。
    * 如果队列为空且 `sockfd` 是非阻塞模式，`accept()` 立即返回 -1，并将 `errno` 设置为 `EAGAIN` 或 `EWOULDBLOCK`。
3.  **提取连接** ：如果队列非空，内核从中取出一个连接。
4.  **创建新套接字** ：内核为这个被接受的连接创建一个 **全新的** 内部套接字结构和对应的文件对象。这个新套接字的状态被设为 `ESTABLISHED`，并关联了客户端的地址信息。
5.  **返回新描述符** ：内核在进程的文件描述符表中分配一个新的文件描述符，指向这个新创建的已连接套接字的文件对象，并将此描述符返回给应用程序。
6.  **填充地址信息** ：如果 `addr` 和 `addrlen` 参数有效，内核将客户端的地址信息复制到 `addr` 指向的缓冲区，并更新 `*addrlen`。

**关键点** ：`accept()` 返回的是一个 **新的文件描述符** ，代表与特定客户端的通信通道。后续与该客户端的数据收发（`send()`, `recv()` 等）必须使用这个 **新描述符** ，而非原来的监听描述符 `sockfd`。监听描述符 `sockfd` 保持不变，继续用于接受后续的连接请求。

**返回值** ：

* 成功：返回一个新的、非负的已连接套接字文件描述符。
* 失败：返回 -1，并设置 `errno`。

**示例（续上，接受单个连接）** ：

```c
    // ... listen() 成功后 ...

    int new_socket;
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address); // 注意类型是 socklen_t
    char client_ip[INET_ADDRSTRLEN]; // 用于存储客户端 IP 字符串

    printf("等待连接...\n");

    // 接受一个传入连接
    // 默认情况下，accept() 会阻塞，直到有客户端连接进来
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, &addrlen)) < 0) {
        perror("accept 失败");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 将客户端的 IP 地址从网络字节序转换为点分十进制字符串以便打印
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("接受来自 %s:%d 的连接\n", client_ip, ntohs(client_address.sin_port));
    printf("用于通信的新套接字描述符: %d\n", new_socket);

    // 现在可以使用 new_socket 与该客户端进行数据交换
    // 例如: send(new_socket, "欢迎!\n", 7, 0);
    // 例如: recv(new_socket, buffer, 1024, 0);

    // 完成与该客户端的通信后，关闭连接套接字
    close(new_socket);

    // 服务器最终关闭时，关闭监听套接字
    close(server_fd);
```

上述示例仅能处理一次连接。为实现并发处理多个客户端，需引入循环结构，并结合多进程/多线程模型或 I/O 多路复用技术。对于需要高效处理大量并发连接的场景（如我们的 Pub/Sub 服务器），I/O 多路复用是更常用的方案。

### `select()` - I/O 多路复用机制

服务器程序通常需要同时关注多个事件源：监听套接字上的新连接请求，以及多个已连接客户端套接字上的数据到达。若使用阻塞式的 `accept()` 和 `recv()`，程序执行流会在单一调用点暂停，无法及时响应其他事件。I/O 多路复用技术解决了这个问题，它允许进程同时监视多个文件描述符，并在其中任何一个或多个描述符准备好进行 I/O 操作（可读、可写或异常）时获得通知。`select()` 是 POSIX 标准中定义的一种经典 I/O 多路复用机制。

```c
#include <sys/select.h>
#include <sys/time.h> // 对于 struct timeval

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
```

* `nfds`: 被监听的文件描述符的数量，其值应为所有被监听文件描述符中的最大值 **加 1** 。例如，若监听 fd 3, 5, 8，则 `nfds` 应为 9。
* `readfds`: 指向 `fd_set` 结构体的指针，用于指定需要监听 **可读** 事件的文件描述符集合。对于监听套接字，可读意味着有新连接待 `accept()`；对于已连接套接字，可读意味着有数据到达、连接已关闭（收到 FIN）或发生错误。
* `writefds`: 指向 `fd_set` 的指针，用于指定需要监听 **可写** 事件的文件描述符集合。通常表示套接字的发送缓冲区有可用空间。
* `exceptfds`: 指向 `fd_set` 的指针，用于指定需要监听 **异常** 条件的文件描述符集合（如 TCP 带外数据）。
* `timeout`: 指向 `struct timeval` 结构体的指针，用于设定 `select()` 的最大等待时间。
    * `struct timeval { time_t tv_sec; suseconds_t tv_usec; };` (秒和微秒)
    * 若 `timeout` 为 `NULL`，`select()` 将无限期阻塞，直到至少有一个描述符就绪。
    * 若 `timeout` 指向的结构体中 `tv_sec` 和 `tv_usec` 均为 0，`select()` 执行非阻塞检查，立即返回。
    * 若 `timeout` 指向的结构体包含正值，`select()` 最多等待指定时间。超时前有描述符就绪则返回；超时则返回 0。

**`fd_set` 相关宏定义**：

* `FD_ZERO(fd_set *set)`: 清空（初始化）一个 `fd_set` 集合。 **每次调用 `select()` 前，对工作集合必须执行此操作或从主集合复制** 。
* `FD_SET(int fd, fd_set *set)`: 将文件描述符 `fd` 添加到集合 `set` 中。
* `FD_CLR(int fd, fd_set *set)`: 将文件描述符 `fd` 从集合 `set` 中移除。
* `FD_ISSET(int fd, fd_set *set)`:  **在 `select()` 返回后** ，用于检查文件描述符 `fd` 是否仍在就绪集合 `set` 中。

**重要特性** ：`select()` 调用会 **修改** 传入的 `fd_set` 集合（`readfds`, `writefds`, `exceptfds`），将其中未就绪的文件描述符移除。因此，应用程序通常需要维护一个 **主集合** （master set）记录所有需要关心的文件描述符，在每次循环调用 `select()` 之前，将主集合的内容 **复制** 到一个 **工作集合** （working set），然后将工作集合传递给 `select()`。

**返回值** ：

* 成功：返回三个集合中总共就绪的文件描述符数量。
* 超时：返回 0。
* 失败：返回 -1，并设置 `errno`。

**使用 `select()` 的服务器模式** ：

1.  初始化监听套接字 (`socket`, `bind`, `listen`)。
2.  初始化主文件描述符集合 `master_fds`：`FD_ZERO(&master_fds)`，然后 `FD_SET(listener_fd, &master_fds)`。
3.  维护当前最大文件描述符 `max_fd`，初始值为 `listener_fd`。
4.  进入主事件循环：
    a.  创建临时工作集合 `read_fds`，将其初始化为主集合：`read_fds = master_fds`。
    b.  调用 `select(max_fd + 1, &read_fds, NULL, NULL, NULL)`（此处示例仅关心读事件，阻塞等待）。
    c.  检查 `select` 返回值。若为 -1，处理错误（需注意 `EINTR` 信号中断）。
    d.  遍历从 0 到 `max_fd` 的所有文件描述符 `i`：
        i.  使用 `FD_ISSET(i, &read_fds)` 判断描述符 `i` 是否在就绪的读集合中。
        ii. 若 `i` 是监听套接字 (`listener_fd`) 且 `FD_ISSET` 为真：
            * 调用 `accept()` 接受新连接，得到 `new_fd`。
            * 将 `new_fd` 添加到主集合 `master_fds`：`FD_SET(new_fd, &master_fds)`。
            * 更新 `max_fd`：`if (new_fd > max_fd) max_fd = new_fd;`。
        iii. 若 `i` 是已连接客户端套接字且 `FD_ISSET` 为真：
            * 调用 `recv(i, buffer, ...)` 或 `read(i, buffer, ...)` 尝试读取数据。
            * **处理连接状态** ：
                * **返回值 `> 0`** : 成功读取 `valread` 字节数据。处理接收到的数据（解析命令 `SUB`/`PUB` 等）。
                * **返回值 `0`** : 表示对端执行了有序关闭（发送了 FIN 包），所有数据已接收完毕。这是 TCP **半关闭** 状态的体现（对方关闭了写，本方可以继续写，但通常也应准备关闭）。应用程序应：
                    * `close(i)` 关闭本端的套接字连接。
                    * 从 `master_fds` 中移除 `i`：`FD_CLR(i, &master_fds)`。
                    * 清理与该客户端相关的应用层资源（如订阅信息）。
                * **返回值 `-1`** : 发生错误。检查 `errno`：
                    * 若 `errno` 为 `EAGAIN` 或 `EWOULDBLOCK`（非阻塞模式下），表示暂时无数据可读，不是错误。
                    * 若 `errno` 为 `ECONNRESET`，表示对端发送了 RST 包（连接异常中断）。
                    * 其他错误（如 `ETIMEDOUT`, `ENOTCONN` 等）。
                    * 对于实际错误，同样需要 `close(i)`，从 `master_fds` 移除 `i`，并清理资源。

#### 关于 `shutdown()` 和半关闭

`recv`/`read` 返回 0 表明对端关闭了其发送通道。如果本端应用还想发送数据，理论上可以（只要对端接收缓冲区未满且未完全关闭连接），但这通常不符合应用逻辑。更常见的是，接收到 0 后本端也应关闭连接。

有时应用可能需要主动进行 **半关闭** ，即关闭自己的发送通道，但仍保持接收通道打开，以接收对端可能还未发送完的数据。这可以通过 `shutdown()` 系统调用实现：

```c
#include <sys/socket.h>

int shutdown(int sockfd, int how);
```

* `sockfd`: 要操作的套接字。
* `how`: 指定关闭方式：
    * `SHUT_RD`: 关闭接收通道（之后不能再从此套接字接收数据）。
    * `SHUT_WR`: 关闭发送通道（之后不能再从此套接字发送数据）。这会向对端发送一个 FIN 包。
    * `SHUT_RDWR`: 同时关闭接收和发送通道（等同于 `close()` 的一部分效果，但不释放文件描述符）。

使用 `shutdown(sockfd, SHUT_WR)` 后，对端 `recv` 会在接收完所有已在途的数据后返回 0。而本端仍可调用 `recv` 接收数据，直到对端也关闭其发送通道（发送 FIN）。

`close()` 调用则会同时关闭读写两个方向（如果引用计数为零，还会释放文件描述符和相关内核资源）。

在我们的简单 Pub/Sub 服务器中，我们直接使用 `close()` 处理连接终止，这隐含了双向关闭。

### 极简 Pub/Sub 服务器 C 代码实现

以下是使用 `select()` 实现的极简 Pub/Sub 服务器的 C 代码，注释已更新为中文。请注意，此实现为了教学目的保持简洁，省略了许多生产环境中必要的健壮性设计（如完善的错误处理、动态资源管理、优化的 `max_fd` 更新策略等）。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h>

#define PORT 8080               // 服务器监听端口
#define MAX_CLIENTS 30          // 最大并发客户端数量
#define MAX_TOPICS 10           // 最大主题数量
#define MAX_SUBS_PER_TOPIC 10   // 每个主题最大订阅者数量
#define BUFFER_SIZE 1024        // 接收缓冲区大小

// 主题订阅数据结构
typedef struct {
    char name[50];                       // 主题名称
    int subscribers[MAX_SUBS_PER_TOPIC]; // 订阅该主题的客户端套接字描述符数组
    int sub_count;                       // 当前订阅者数量
} Topic;

Topic topics[MAX_TOPICS]; // 全局主题数组
int topic_count = 0;      // 当前主题数量

// 辅助函数：查找或创建主题
int find_or_create_topic(const char* topic_name) {
    // 查找现有主题
    for (int i = 0; i < topic_count; ++i) {
        if (strcmp(topics[i].name, topic_name) == 0) {
            return i; // 找到，返回索引
        }
    }
    // 如果未找到且还有空间，则创建新主题
    if (topic_count < MAX_TOPICS) {
        strncpy(topics[topic_count].name, topic_name, sizeof(topics[topic_count].name) - 1);
        topics[topic_count].name[sizeof(topics[topic_count].name) - 1] = '\0'; // 确保空字符结尾
        topics[topic_count].sub_count = 0; // 初始化订阅者数量为0
        return topic_count++; // 返回新主题的索引，并增加主题计数
    }
    return -1; // 主题数组已满，无法创建
}

// 辅助函数：将客户端添加到主题的订阅列表
void add_subscriber(int topic_index, int client_socket) {
    if (topic_index < 0 || topic_index >= topic_count) return; // 无效的主题索引
    Topic* topic = &topics[topic_index];
    if (topic->sub_count < MAX_SUBS_PER_TOPIC) {
        // 检查是否已订阅，避免重复添加
        for(int i = 0; i < topic->sub_count; ++i) {
            if (topic->subscribers[i] == client_socket) return; // 已存在，直接返回
        }
        // 添加新的订阅者
        topic->subscribers[topic->sub_count++] = client_socket;
        printf("客户端 %d 订阅了主题 '%s'\n", client_socket, topic->name);
    } else {
        // 主题订阅已满
        printf("主题 '%s' 订阅已满, 无法添加客户端 %d\n", topic->name, client_socket);
        // 可选: 向客户端发送错误消息
        // send(client_socket, "ERR topic full\n", 15, 0);
    }
}

// 辅助函数：从所有主题中移除指定客户端的订阅
void remove_subscriber(int client_socket) {
    printf("正在移除客户端 %d 的所有订阅。\n", client_socket);
    for (int i = 0; i < topic_count; ++i) { // 遍历所有主题
        int found_idx = -1;
        // 在当前主题的订阅者列表中查找该客户端
        for (int j = 0; j < topics[i].sub_count; ++j) {
            if (topics[i].subscribers[j] == client_socket) {
                found_idx = j;
                break;
            }
        }
        // 如果找到该客户端
        if (found_idx != -1) {
            // 将后续订阅者向前移动一位，覆盖掉要移除的客户端
            for (int k = found_idx; k < topics[i].sub_count - 1; ++k) {
                topics[i].subscribers[k] = topics[i].subscribers[k + 1];
            }
            topics[i].sub_count--; // 减少订阅者计数
            printf("已将客户端 %d 从主题 '%s' 中移除\n", client_socket, topics[i].name);
        }
    }
}

// 辅助函数：向指定主题发布消息
void publish_message(int topic_index, const char* message, int publisher_socket) {
    if (topic_index < 0 || topic_index >= topic_count) return; // 无效的主题索引
    Topic* topic = &topics[topic_index];
    char full_message[BUFFER_SIZE + 100]; // 预留足够空间构造 "MSG <topic> <data>\n" 格式的消息

    // 构造完整的消息格式
    snprintf(full_message, sizeof(full_message), "MSG %s %s", topic->name, message);
     // 确保消息以换行符结束 (如果原始消息没有的话)
    if (full_message[strlen(full_message)-1] != '\n') {
         strncat(full_message, "\n", sizeof(full_message) - strlen(full_message) - 1);
    }


    printf("向主题 '%s' 发布消息: %s", topic->name, message); // 假设原始 message 可能已包含换行符
    // 遍历该主题的所有订阅者
    for (int i = 0; i < topic->sub_count; ++i) {
        int subscriber_socket = topic->subscribers[i];
        // 不将消息发回给发布者自己
        if (subscriber_socket != publisher_socket) {
            // 发送消息给订阅者
            if (send(subscriber_socket, full_message, strlen(full_message), 0) < 0) {
                // 发送失败，可能需要处理连接断开等问题
                perror("向订阅者发送消息失败");
                // 注意: 健壮的实现应在此处检测到错误后，可能也需要关闭并清理该订阅者的连接
            } else {
                 printf("  -> 已发送至客户端 %d\n", subscriber_socket);
            }
        }
    }
}


int main() {
    int listener_fd;                   // 监听套接字描述符
    int new_socket;                    // 新接受的客户端连接套接字描述符
    int client_sockets[MAX_CLIENTS];   // 存储客户端套接字描述符的数组
    int max_clients = MAX_CLIENTS;     // 最大客户端数 (冗余变量，可直接用宏)
    int activity;                      // select() 的返回值
    int i, valread, sd;                // 循环变量, read() 返回值, 当前处理的套接字描述符
    int max_sd;                        // select() 需要的最大文件描述符值 + 1
    struct sockaddr_in address;        // 服务器地址结构
    char buffer[BUFFER_SIZE + 1];      // 数据接收缓冲区 (+1 用于空字符结尾)

    // 文件描述符集合
    fd_set readfds;

    // 初始化客户端套接字数组，全部置 0 (0 不是有效的文件描述符)
    for (i = 0; i < max_clients; i++) {
        client_sockets[i] = 0;
    }

    // 创建主监听套接字
    if ((listener_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket 创建失败");
        exit(EXIT_FAILURE);
    }

    // 设置监听套接字选项，允许地址重用
    int opt = 1;
    if (setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt 失败");
        exit(EXIT_FAILURE);
    }

    // 配置服务器地址结构
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有接口
    address.sin_port = htons(PORT);       // 指定端口（网络字节序）

    // 将监听套接字绑定到指定地址和端口
    if (bind(listener_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind 失败");
        exit(EXIT_FAILURE);
    }
    printf("监听器已启动在端口 %d \n", PORT);

    // 使套接字进入监听状态，设定等待队列长度为 3
    if (listen(listener_fd, 3) < 0) {
        perror("listen 失败");
        exit(EXIT_FAILURE);
    }

    // 等待客户端连接
    socklen_t addrlen = sizeof(address);
    puts("等待连接中 ...");

    while (1) { // 服务器主循环
        // 清空读文件描述符集合
        FD_ZERO(&readfds);

        // 将监听套接字加入集合
        FD_SET(listener_fd, &readfds);
        max_sd = listener_fd; // 初始化 max_sd

        // 将所有活动的客户端套接字加入集合
        for (i = 0; i < max_clients; i++) {
            sd = client_sockets[i]; // 获取客户端套接字描述符

            // 如果是有效的套接字描述符 (大于 0)，则加入读集合
            if (sd > 0)
                FD_SET(sd, &readfds);

            // 更新 max_sd 以跟踪最大的文件描述符值
            if (sd > max_sd)
                max_sd = sd;
        }

        // 调用 select() 等待活动发生，timeout 设置为 NULL 表示无限期阻塞
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        // 检查 select() 是否出错 (忽略 EINTR 信号中断)
        if ((activity < 0) && (errno != EINTR)) {
            perror("select 错误");
            // 在实际应用中可能需要更复杂的错误处理逻辑
        }

        // 检查监听套接字是否就绪 (表示有新连接请求)
        if (FD_ISSET(listener_fd, &readfds)) {
            // 接受新连接
            if ((new_socket = accept(listener_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
                perror("accept 失败");
                // 严重错误，通常需要退出或重启服务
                exit(EXIT_FAILURE);
            }

            // 获取并打印新客户端的 IP 和端口信息
             char client_ip[INET_ADDRSTRLEN];
             inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
             printf("新连接建立，套接字描述符为 %d，IP 为: %s，端口为: %d\n",
                   new_socket, client_ip, ntohs(address.sin_port));

            // 将新套接字添加到客户端数组中
            for (i = 0; i < max_clients; i++) {
                // 找到数组中第一个空位
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("已将套接字 %d 添加到列表位置 %d\n", new_socket, i);
                    break; // 添加成功后退出循环
                }
            }
            // 如果客户端数组已满
             if (i == max_clients) {
                printf("客户端连接数已达上限，拒绝连接。\n");
                close(new_socket); // 关闭这个无法处理的连接
            }
        }

        // 检查各个客户端套接字是否有活动 (数据到达或连接关闭)
        for (i = 0; i < max_clients; i++) {
            sd = client_sockets[i]; // 当前要检查的客户端套接字

            // 如果当前客户端套接字在就绪的读集合中
            if (FD_ISSET(sd, &readfds)) {
                // 尝试读取数据
                // read() 返回值: >0 表示读取的字节数, 0 表示连接已关闭 (收到FIN), <0 表示错误
                if ((valread = read(sd, buffer, BUFFER_SIZE)) <= 0) {
                    // 连接关闭或发生错误
                    if (valread == 0) {
                        // 对端正常关闭连接 (FIN)
                        getpeername(sd, (struct sockaddr*)&address, &addrlen); // 获取对端地址信息用于打印
                        char client_ip[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
                        printf("主机 %s:%d (套接字 %d) 断开连接\n", client_ip, ntohs(address.sin_port), sd);
                    } else {
                        // 读取错误
                        perror("read 错误");
                        // 也可以在这里获取对端信息打印
                    }

                    // 关闭套接字
                    close(sd);
                    // 从客户端数组中移除 (标记为 0)
                    client_sockets[i] = 0;
                    // 从所有主题订阅中移除该客户端
                    remove_subscriber(sd);

                } else {
                    // 成功读取到数据，处理客户端发送的命令
                    buffer[valread] = '\0'; // 添加空字符结尾，确保字符串处理安全

                    // 简单的命令解析 (生产环境需要更健壮的解析器)
                    char command[5];       // 存储命令 (SUB/PUB)
                    char topic_name[50];   // 存储主题名
                    char message[BUFFER_SIZE]; // 存储消息内容 (PUB 命令)
                    memset(message, 0, sizeof(message)); // 清空消息缓冲区

                    // 使用 sscanf 解析输入，格式为 "CMD TOPIC [MESSAGE]"
                    // %4s: 读取最多4个字符作为命令
                    // %49s: 读取最多49个字符作为主题名 (防止溢出)
                    // %[^\n]: 读取从主题名后到行尾的所有字符作为消息 (包括空格)
                    int parsed_items = sscanf(buffer, "%4s %49s %[^\n]", command, topic_name, message);

                    printf("收到来自客户端 %d 的数据: %s", sd, buffer); // buffer 可能自带换行符

                    if (parsed_items >= 2 && strcmp(command, "SUB") == 0) {
                        // 处理 SUB 命令
                        int topic_idx = find_or_create_topic(topic_name);
                        if (topic_idx != -1) {
                            add_subscriber(topic_idx, sd);
                            // 可选: 向客户端发送订阅成功确认
                            // send(sd, "SUB OK\n", 7, 0);
                        } else {
                            // 可选: 发送错误（如主题数达到上限）
                            // send(sd, "ERR max topics\n", 15, 0);
                        }
                    } else if (parsed_items >= 3 && strcmp(command, "PUB") == 0) {
                         // 处理 PUB 命令
                         int topic_idx = find_or_create_topic(topic_name); // 查找主题，如果不存在，简单实现可以忽略或报错
                          if (topic_idx != -1) {
                             // 确保消息格式包含换行符，便于客户端接收处理
                             // 注意: 这里假设 message 已包含从 sscanf 读取的内容
                             // 这里对 message 的处理可以省略，因为 publish_message 内部会确保换行符
                             publish_message(topic_idx, message, sd);
                         } else {
                              printf("未找到用于发布的主题 '%s'.\n", topic_name);
                             // 可选: 发送错误（主题不存在）
                             // send(sd, "ERR no such topic\n", 18, 0);
                         }
                    } else {
                        // 无效命令格式
                        printf("来自客户端 %d 的无效命令: %s", sd, buffer);
                        // 可选: 发送错误
                        // send(sd, "ERR invalid command\n", 20, 0);
                    }
                    // 清理缓冲区以便下次读取 (可选，因为 read 会覆盖)
                    memset(buffer, 0, BUFFER_SIZE + 1);
                }
            }
        }
    }

    // 理论上，服务器的无限循环不会到达这里，但在正常关闭流程中应关闭监听套接字
    close(listener_fd);

    return 0;
}
```

**编译**

```bash
gcc pubsub_server_revised.c -o pubsub_server_revised
```

**运行**

```bash
./pubsub_server_revised
```

```bash
(base) ➜  back-end-notes git:(master) ✗ nc localhost 8080
^C
(base) ➜  back-end-notes git:(master) ✗ nc localhost 8080
SUB TOPIC-1
MSG TOPIC-1 1,242.42,214\n
```

```bash
(base) ➜  back-end-notes git:(master) ✗ nc localhost 8080
PUB TOPIC-1 1,242.42,214\n
```

```bash
piperliu@go-x86:~/code/socket$ ./pubsub_server_revised
监听器已启动在端口 8080 
等待连接中 ...
新连接建立，套接字描述符为 5，IP 为: 127.0.0.1，端口为: 38910
已将套接字 5 添加到列表位置 0
主机 127.0.0.1:38910 (套接字 5) 断开连接
正在移除客户端 5 的所有订阅。
新连接建立，套接字描述符为 5，IP 为: 127.0.0.1，端口为: 38924
已将套接字 5 添加到列表位置 0
收到来自客户端 5 的数据: SUB TOPIC-1
客户端 5 订阅了主题 'TOPIC-1'
新连接建立，套接字描述符为 6，IP 为: 127.0.0.1，端口为: 58630
已将套接字 6 添加到列表位置 1
收到来自客户端 6 的数据: PUB TOPIC-1 1,242.42,214\n
向主题 'TOPIC-1' 发布消息: 1,242.42,214\n  -> 已发送至客户端 5
```

### 订阅与发布流程的内核视角梳理

结合内核操作，我们重新梳理客户端订阅和发布时服务器端的处理流程：

1.  **服务器初始化与监听**

* `socket()`：内核创建 `socket` 结构，分配资源，返回文件描述符 `listener_fd`。
* `bind()`：内核校验地址、端口可用性及权限，将本地地址信息关联到 `listener_fd` 的 `socket` 结构。
* `listen()`：内核将 `listener_fd` 对应的 `socket` 结构状态置为 `LISTEN`，并初始化 SYN 队列和 Accept 队列。

2.  **客户端连接建立**

* 客户端发起 TCP 连接请求（发送 SYN 包）。
* 服务器内核网络协议栈收到 SYN，创建半连接条目放入 SYN 队列，回复 SYN-ACK。
* 客户端回复 ACK，完成三次握手。内核将对应的连接从 SYN 队列移至 Accept 队列。
* 服务器进程调用 `select()`，`select` 检测到 `listener_fd` 可读（因为 Accept 队列非空）。
* `FD_ISSET(listener_fd, ...)` 为真。
* 服务器进程调用 `accept()`。内核从 Accept 队列取出一个连接，创建新的 `socket` 结构和文件描述符 `new_socket`（状态为 `ESTABLISHED`），返回给服务器进程。

3.  **客户端订阅 (`SUB`)**

* 客户端通过 `new_socket` 发送 "SUB ..." 数据。
* 数据到达服务器，内核将其放入 `new_socket` 的接收缓冲区。
* 服务器进程调用 `select()`，`select` 检测到 `new_socket` 可读（接收缓冲区有数据）。
* `FD_ISSET(new_socket, ...)` 为真。
* 服务器进程调用 `read(new_socket, ...)`，内核从接收缓冲区复制数据到用户空间 `buffer`。
* 服务器进程解析 `buffer`，识别订阅请求，更新应用层数据结构（`topics` 数组）。

4.  **客户端发布 (`PUB`)**：
    * 客户端通过 `new_socket` 发送 "PUB ..." 数据。
    * 数据到达服务器，内核处理同上，`select` 报告 `new_socket` 可读。
    * 服务器进程调用 `read()` 获取数据并解析。
    * 服务器进程查找主题，遍历订阅者列表。
    * 对于每个订阅者 `sub_socket`（且 `sub_socket != new_socket`）：
        * 服务器进程调用 `send(sub_socket, ...)`。内核将数据复制到 `sub_socket` 的发送缓冲区。
        * 内核网络协议栈负责将发送缓冲区的数据打包成 TCP 段并发送出去。

5.  **客户端断开连接 (有序关闭)**：
    * 客户端调用 `close()` 或 `shutdown(SHUT_WR)`，其内核发送 FIN 包。
    * 服务器内核收到 FIN，将 `new_socket` 标记为收到 FIN，并向客户端回复 ACK。连接进入 `CLOSE_WAIT` 状态。
    * 服务器进程调用 `select()`，`select` 检测到 `new_socket` 可读（因为收到 FIN 也是可读事件）。
    * `FD_ISSET(new_socket, ...)` 为真。
    * 服务器进程调用 `read(new_socket, ...)`，`read` 返回 0。
    * 服务器进程识别出连接关闭，调用 `close(new_socket)`。内核发送 FIN 包给客户端（如果尚未发送），释放套接字资源（当引用计数为0时），从 `master_fds` 移除 `new_socket`，清理应用层资源。
