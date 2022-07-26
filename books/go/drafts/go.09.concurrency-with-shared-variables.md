# 9. Concurrency with Shared Variables

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [9.1 Race Conditions](#91-race-conditions)
- [9.2 Mutual Exclusion: sync.Mutex](#92-mutual-exclusion-syncmutex)
- [9.3 Read/Write Mutexes: sync.RWMutex](#93-readwrite-mutexes-syncrwmutex)
- [9.4 Memory Synchronization](#94-memory-synchronization)
- [9.5 Lazy Initialization: sync.Once](#95-lazy-initialization-synconce)
- [9.6 The Race Detector](#96-the-race-detector)
- [9.7 Example: Concurrent Non-Blocking Cache （一个键值对一个锁？不，人均 entry 做 chan broadcast ）](#97-example-concurrent-non-blocking-cache-一个键值对一个锁不人均-entry-做-chan-broadcast)
- [9.8 Goroutines and Threads （重要）](#98-goroutines-and-threads-重要)
  - [9.8.1 Growable Stacks](#981-growable-stacks)
  - [9.8.2 Goroutine Scheduling](#982-goroutine-scheduling)
  - [9.8.3 GOMAXPROCS](#983-gomaxprocs)
  - [9.8.4 Goroutines Have No Identity](#984-goroutines-have-no-identity)

<!-- /code_chunk_output -->

### 9.1 Race Conditions

### 9.2 Mutual Exclusion: sync.Mutex

### 9.3 Read/Write Mutexes: sync.RWMutex

### 9.4 Memory Synchronization

### 9.5 Lazy Initialization: sync.Once

`sync.Once` 保证了在并发的情况下，也可以只进行一次懒加载。

如果不使用 `sync.Once` 如下。

```go
var mu sync.RWMutex // guards icons
var icons map[string]image.Image

// Concurrency-safe
func Icon(name string) image.Image {
    mu.RLock()
    if icons != nil {
        icon := icons[name]
        mu.RUnlock()
        return icon
    }
    mu.RUnlock()

    // Load the icon and store it in the map.
    // acquire an exclusive lock
    mu.Lock()
    if icons == nil {
        loadIcons()
    }
    icon := icons[name]
    mu.Unlock()
    return icon
}
```

现在可以如下这么写。

```go
var loadIconsOnce sync.Once
var icons map[string]image.Image

// Concurrency-safe
func Icon(name string) image.Image {
    loadIconsOnce.Do(loadIcons)
    return icons[name]
}
```

### 9.6 The Race Detector

Go 的 runtime 自带竞态检测器。

Just add the `-race` flag to your `go build`, `go run`, or `go test` command.

### 9.7 Example: Concurrent Non-Blocking Cache （一个键值对一个锁？不，人均 entry 做 chan broadcast ）

This is the problem of memoizing a function, that is, caching the result of a function so that it need be computed only once. Our solution will be concurrency-safe and will avoid the contention associated with designs based on a single lock for the whole cache.

如下函数运行一次成本极高，因此将其作为本次实验对象。

```go
func httpGetBody(url string) (interface{}, error) {
    resp, err := http.Get(url)
    if err != nil {
        return nil, err
    }
    defer resp.Body.Close()
    return ioutil.ReadAll(resp.Body)
}
```

在 [memo1](../gopl.io/ch9/memo1/memo.go) 中，无法处理并行访问的情况（有 race ）。

```bash
$ go test -run=TestConcurrent -race -v gopl.io/ch9/memo1
=== RUN   TestConcurrent
...
WARNING: DATA RACE
Write by goroutine 36:
  runtime.mapassign1()
      ~/go/src/runtime/hashmap.go:411 +0x0
  gopl.io/ch9/memo1.(*Memo).Get()
      ~/gobook2/src/gopl.io/ch9/memo1/memo.go:32 +0x205
  ...

Previous write by goroutine 35:
  runtime.mapassign1()
      ~/go/src/runtime/hashmap.go:411 +0x0
  gopl.io/ch9/memo1.(*Memo).Get()
      ~/gobook2/src/gopl.io/ch9/memo1/memo.go:32 +0x205
...
Found 1 data race(s)
FAIL	gopl.io/ch9/memo1	2.393s
```

最直观的解决办法如 [memo2](../gopl.io/ch9/memo2/memo.go) 就是加锁。但是这里是对一整个 cache map 进行加锁，这将导致这个并发场景下的 cache 做得得不偿失。

在 [memo3](../gopl.io/ch9/memo3/memo.go) 中拆分成了两个临界区，但是原函数的调用又会导致一些 race 。

总不能一个键值对一个锁吧？没错，还真是，只不过这里是一个键值对一个 entry ，更加优雅了。参考 [memo4](../gopl.io/ch9/memo4/memo.go) 。

```go
package memo

import "sync"

// Func is the type of the function to memoize.
type Func func(string) (interface{}, error)

type result struct {
	value interface{}
	err   error
}

//!+
type entry struct {
	res   result
	ready chan struct{} // closed when res is ready
}

func New(f Func) *Memo {
	return &Memo{f: f, cache: make(map[string]*entry)}
}

type Memo struct {
	f     Func
	mu    sync.Mutex // guards cache
	cache map[string]*entry
}

func (memo *Memo) Get(key string) (value interface{}, err error) {
	memo.mu.Lock()
	e := memo.cache[key]
	if e == nil {
		// This is the first request for this key.
		// This goroutine becomes responsible for computing
		// the value and broadcasting the ready condition.
		e = &entry{ready: make(chan struct{})}
		memo.cache[key] = e
		memo.mu.Unlock()

		e.res.value, e.res.err = memo.f(key)

		close(e.ready) // broadcast ready condition
	} else {
		// This is a repeat request for this key.
		memo.mu.Unlock()

		<-e.ready // wait for ready condition
	}
	return e.res.value, e.res.err
}
```

[memo5](../gopl.io/ch9/memo5/memo.go) 提供了一种很 Go 的方式，没有用到锁，每次 Get 都新建并传递 chan 。

### 9.8 Goroutines and Threads （重要）

#### 9.8.1 Growable Stacks

OS 的 thread 一般而言有一个 fixed-size block of memory (often as large as 2MB) for its stack.

Go 的 goroutine 则不是 fixed-size 的。通常而言其首先是 2KB 的，然后按需增长，最大可能是 1GB 。

#### 9.8.2 Goroutine Scheduling

OS threads are scheduled by the OS kernel. Every few milliseconds, a hardware timer interrupts the processor, which causes a kernel function called the scheduler to be invoked. This function suspends the currently executing thread and saves its registers in memory, looks over the list of threads and decides which one should run next, restores that thread's registers from memory, then resumes the execution of that thread. Because OS threads are scheduled by the kernel, passing control from one thread to another requires a full context switch, that is, saving the state of one user thread to memory, restoring the state of another, and updating the scheduler's data structures. This operation is slow, due to its poor locality and the number of memory accesses required, and has historically only gotten worse as the number of CPU cycles required to access memory has increased.

The Go runtime contains its own scheduler that uses a technique known as m:n scheduling, because it multiplexes (or schedules) m goroutines on n OS threads. The job of the Go scheduler is analogous to that of the kernel scheduler, but it is concerned only with the goroutines of a single Go program.

Unlike the operating system's thread scheduler, the Go scheduler is not invoked periodically by a hardware timer, byt implicitly by certain Go language constructs. For example, when a goroutine calls time.Sleep or blocks in a channel or mutex operation, the scheduler puts it to sleep and runs another goroutine until it is time to wake the first one up. Because it doesn't need to switch to kernel context, rescheduling a ggoroutine is much cheaper than rescheduling a thread.

#### 9.8.3 GOMAXPROCS

GOMAXPROCS 一般而言是 CPU 的核数，也就是 `m:n` 中的 `n` 。

```go
for {
    go fmt.Print(0)
    fmt.Print(1)
}
```

运行结果可能如下。

```bash
$ GOMAXPROCS=1 go run hacker-cliché.go
1111111111111111111110000000000000000000001111...

$ GOMAXPROCS=1 go run hacker-cliché.go
0101010101001100010111010110101010100110101010...
```

这是软件切换和硬件切换带来的区别。

#### 9.8.4 Goroutines Have No Identity

这里主要想说大部分 OS 或者编程语言的 thread 是有标识的，这是为了更好的配合 `thread-local storage` 。但是 Go 设计者认为这种模式容易被 abused ，干脆就不提供支持（或许也能减少运行时开销）。
