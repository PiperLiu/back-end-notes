# 7 | 并发 API

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [35 | 优先选用基于任务而非基于线程的程序设计](#35-优先选用基于任务而非基于线程的程序设计)
  - [三个层面的线程以及线程超订 oversubscription](#三个层面的线程以及线程超订-oversubscription)
  - [基于任务相比基于线程的好处](#基于任务相比基于线程的好处)

<!-- /code_chunk_output -->

C++ 11 提供了并发支持，对于跨平台来讲，是好事；但是功能可能没有已有线程 API 库（例如， pthread 或 Windows 线程库）功能强大。

这里再额外说一下 `std::future` 和 `std::shared_future` ，我们管这个单词叫做“期望”。 `std::future` 型别对象支持移动操作，也可以用以构造 `std::shared_future` 型别对象，而 `std::shared_future` 型别对象又可以复制。

### 35 | 优先选用基于任务而非基于线程的程序设计

```cpp
int doAsyncWork();

// 线程
std::thread t(doAsyncWork);
// fut 是期望的缩写
// 传递给 std::async 的函数对象（如 doAsynWork ）被看作任务 task
auto fut = std::async(doAsyncWork);
```

#### 三个层面的线程以及线程超订 oversubscription

“线程”在 C++ 软件中的三种意义：
- 硬件线程（实际执行计算的线程）
- 软件线程（又称操作系统线程或系统线程），操作系统用以实施跨进程的管理
- `std::thread` 是 C++ 进程里的对象，用作底层软件线程的句柄

有些 `std::thread` 对象表示为 `null` 句柄，对应于“无软件线程”，可能的原因有：
- 它们处于默认构造状态（因此没有待执行的函数）
- 或者被移动了（作为移动目的的 `std::thread` 对象成为了底层线程的句柄）
- 被 `join` 了（待运行函数已结束）
- 被分离了（ `std::thread` 对象与其底层软件线程的连接被切断了）

接下来讨论一下，什么是线程超订（ thread oversubscription ）？

线程超订（ thread oversubscription ）是就绪状态（非阻塞）的软件线程超过了硬件线程数量的时候。这种情况下，线程调度器（通常是操作系统的一部分）会为软件线程在硬件线程之上分配 CPU 时间片。（疑问一下：这种情况应该很常见？）

书中认为超定带来的线程切换，将导致：
- 软件线程通常不会命中 CPU 缓存（即，几乎不会包含对于那软件线程有用的任何数据和指令）
- CPU 内核运行的的“新”软件线程还会“污染” CPU 缓存上为“旧”线程所准备的数据

书中的意思是，虽然避免超订极其困难，但是开发者可以把“是否开一个新线程”这种难题留给 `std::async` 来解决，没准，它会帮你避免一些超订。

#### 基于任务相比基于线程的好处

首先， `auto fut = std::async(doAsyncWork);` 可以通过 `fut.get()` 很方便地获取 `doAsyncWork` 返回值。

另外，基于线程的程序设计要求手动管理线程耗尽、超订、负载均衡，以及新平台适配。

但是，线程有更大的灵活性，更能贴近底层。

