# 第 12 章 事件

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [12.1 文件事件](#121-文件事件)
  - [12.1.1 文件事件处理器的构成](#1211-文件事件处理器的构成)
  - [12.1.2 I/O 多路复用程序的实现](#1212-io-多路复用程序的实现)
  - [12.1.3 事件的类型](#1213-事件的类型)
  - [12.1.4 API](#1214-api)
  - [12.1.5 文件事件的处理器](#1215-文件事件的处理器)
- [12.2 时间事件](#122-时间事件)
  - [12.2.1 实现](#1221-实现)
  - [12.2.2 API](#1222-api)
- [12.3 事件的调度与执行](#123-事件的调度与执行)

<!-- /code_chunk_output -->

两种：
- file event 监听并处理事件
- time event 定时执行事件

阅读源码：
- initServer 里先 aeCreateEventLoop 把 event 链表创建出来，开辟内存
- listenToPort 根据用户配置创建 socket 并 bind 到指定端口，listen 等待客户端连接
- aeCreateFileEvent 注册文件事件
  - 其实就是把 acceptTcpHandler 回调绑定给 eventLoop 中的 `aeFileEvent *fe = &eventLoop->events[fd];`
  - 在接收到 `AE_READABLE` 事件时，会调用 `fe->rfileProc` 回调函数
  - 因此，想要将 `acceptTcpHandler` 绑定到 `AE_READABLE` 事件， `if (mask & AE_READABLE) fe->rfileProc = proc;` 即可

### 12.1 文件事件

基于 Reactor 模式的 file event handler
- I/O 多路复用
- 监听套接字

#### 12.1.1 文件事件处理器的构成

```
套接字 -> I/O 多路复用 -> 文件事件分派器 dispatcher -> 文件事件处理器
```

#### 12.1.2 I/O 多路复用程序的实现

有多种实现方式： `select`, `epoll`, `kqueue`, `epoll` 等。

实现同一套函数签名，在编译器根据宏定义选择不同实现。

#### 12.1.3 事件的类型

src/ae.h

```c
#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITABLE 2
```

- 客户端对套接字 write 、或执行 close ，或客户端对服务器的监听套接字执行 connect （有新的 acceptable 套接字出现），产生 AE_READABLE 事件
- 客户端对套接字执行 read ，产生 AE_WRITABLE 事件

#### 12.1.4 API

参考 src/ae.c

#### 12.1.5 文件事件的处理器

注册 Event 是动态的，感兴趣可以看 `prepareClientToWrite` 。

### 12.2 时间事件

其实也把 aeCreateTimeEvent 注册到 eventLoop 的 events 数组里，不过是注册到 timeEventHead 链表里。

`aeProcessEvents` 中 `aeSearchNearestTimer` 找到最近的定时事件；之后 `aeApiPoll` 负责文件事件。

```c
void aeMain(aeEventLoop *eventLoop) {
    eventLoop->stop = 0;
    while (!eventLoop->stop) {
        if (eventLoop->beforesleep != NULL)
            eventLoop->beforesleep(eventLoop);
        aeProcessEvents(eventLoop, AE_ALL_EVENTS);
    }
}
```

#### 12.2.1 实现

#### 12.2.2 API

### 12.3 事件的调度与执行
