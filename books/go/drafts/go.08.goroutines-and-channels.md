# 8. Goroutines and Channels

本章讨论 communicating sequential process (CSP) a model of concurrency in which values are passed between independent activities (goroutines) but variables are for the most part confined to a single activity.

下一章才讨论 the more traditional model of shared memory multithreading 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [8.1 Goroutines](#81-goroutines)
- [8.2 Example: Concurrent Clock Server](#82-example-concurrent-clock-server)
- [8.3 Example: Concurrent Echo Server](#83-example-concurrent-echo-server)
- [8.4 Channels](#84-channels)
  - [8.4.1 Unbuffered Channels](#841-unbuffered-channels)
  - [8.4.2 Pipelines](#842-pipelines)
  - [8.4.3 Unidirectional Channel Types](#843-unidirectional-channel-types)
  - [8.4.4 Buffered Channels](#844-buffered-channels)
- [8.5 Looping in Parallel](#85-looping-in-parallel)
- [8.6 Example: Concurrent Web Crawler](#86-example-concurrent-web-crawler)
- [8.7 Multiplexing with select](#87-multiplexing-with-select)
- [8.8 Example: Concurrent Directory Traversal](#88-example-concurrent-directory-traversal)
- [8.9 Cancellation](#89-cancellation)
- [8.10 Example: Chat Server](#810-example-chat-server)

<!-- /code_chunk_output -->

### 8.1 Goroutines

### 8.2 Example: Concurrent Clock Server

### 8.3 Example: Concurrent Echo Server

### 8.4 Channels

```go
ch <- x  // a send statement
x = <-ch // a recevie expression in an assignment statement
<-ch     // a receive statement; result is discarded
```

还看到一种说法， `<-chan` 和 `chan<-` 分别表示只读和只写。一般用于参数传递才有意义，参考 [Go 只读/只写channel](https://www.cnblogs.com/baiyuxiong/p/4545028.html) 。

```go
package main

import (
    "fmt"
    "time"
)

func main() {
    c := make(chan int)
    go send(c)
    go recv(c)
    time.Sleep(3 * time.Second)
}
//只能向chan里写数据
func send(c chan<- int) {
    for i := 0; i < 10; i++ {
        c <- i
    }
}
//只能取channel中的数据
func recv(c <-chan int) {
    for i := range c {
        fmt.Println(i)
    }
}
```

#### 8.4.1 Unbuffered Channels

#### 8.4.2 Pipelines

这里有些不错的代码示例。

#### 8.4.3 Unidirectional Channel Types

#### 8.4.4 Buffered Channels

### 8.5 Looping in Parallel

```go
// NOTE: incorrect!
func makeThumbnails2(filenames []string) {
    for _, f := range filenames {
        go thumbnail.ImageFile(f)  // NOTE: ignoring errors
    }
}
// 上面存在第一点不正确的就是没有等待 goroutines
// 第二点就是直接将属于 for 的变量 f 传给了 goroutine ，回顾一下 5.6.1
```

正确写法如下。

```go
func makeThumbnails(filenames []string) {
    ch := make(chan struct{})
    for _, f := range filenames {
        go func(f string) {
            thumbnail.ImageFile(f)
            ch <- struct{}{}
        }(f)
        // NOTE: ignoring errors
        // 这里传入 f 而非直接使用 f 是很必要的
        // 否则 for 循环的 f 会导致 f 变量产生不可预期行为
    }
    for range filenames {
        <-ch // 接收 len(filenames) 次
    }
}
```

### 8.6 Example: Concurrent Web Crawler

### 8.7 Multiplexing with select

这里很好地利用了 chan 的阻塞。

[../gopl.io/ch8/countdown1/](../gopl.io/ch8/countdown1/)

```go
func main() {
    fmt.Println("Commencing countdown.")
    tick := time.Tick(1 * time.Second)
    for countdown := 10; countdown > 0; countdown-- {
        fmt.Println(countdown)
        <-tick
    }
    launch()
}
```

此外还有和 select 联用的一些内容，这里只记录一个典型案例。

```go
ch := make(chan int, 1)
for i := 0; i < 10; i++ {
    select {
    case x := <-ch:
        fmt.Println(x)  // "0" "2" "4" "6" "8"
    case ch <- i:
    }
}
```

然后还应再提一下 `ticker` ，要注意 `ticker.C` 是一个 `<-chan time.Time` 而非 `chan time.Time` 。

```go
ticker := time.NewTicker(500 * time.Millisecond)
<-ticker.C  // receive from the ticker's channel
ticker.Stop() // cause the ticker's goroutine to terminate
```

### 8.8 Example: Concurrent Directory Traversal

参考 ch8/du 系列代码。

### 8.9 Cancellation

这里讨论了在一个在一个 select 中“加锁”或者取消本 goroutine 的方式。

```go
func dirents(dir string) []os.FileInfo {
    select {
    case sema <- struct{}{}:  // acquire token
    case <-done:
        return nil  // cancelled
    defer func() { <-sema }()  // release token
    }
    // ...do something...
}
```

### 8.10 Example: Chat Server
