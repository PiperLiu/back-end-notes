# Concurrency

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Goroutines](#goroutines)
- [Channelss](#channelss)
  - [Buffered Channels](#buffered-channels)
  - [Range and Close](#range-and-close)
  - [Select](#select)
  - [Default Selection](#default-selection)
- [sync.Mutex](#syncmutex)

<!-- /code_chunk_output -->

### Goroutines

A goroutine is a lightweight thread managed by the Go runtime.

```go
go f(x, y, z)
```

starts a new goroutine running

```go
f(x, y, z)
```

The evaluation of `f`, `x`, `y`, and `z` happens in the current goroutine and the execution of `f` happens in the new goroutine.

Goroutines run in the same address space, so access to shared memory must be synchronized. The [sync](https://pkg.go.dev/sync) package provides useful primitives, although you won't need them much in Go as there are other primitives.

```go
package main

import (
	"fmt"
	"time"
)

func say(s string) {
	for i := 0; i < 5; i++ {
		time.Sleep(100 * time.Millisecond)
		fmt.Println(s)
	}
}

func main() {
	go say("world")
	say("hello")
}
```

### Channelss

Channels are a typed conduit through which you can send and receive values with the channel operator, `<-`.

```go
ch <- v    // Send v to channel ch.
v := <-ch  // Receive from ch, and
           // assign value to v.
```

(The data flows in the direction of the arrow.)

Like maps and slices, channels must be created before use:

```go
ch := make(chan int)
```

By default, sends and receives block until the other side is ready. This allows goroutines to synchronize without explicit locks or condition variables.

```go
package main

import "fmt"

// c 是一个用于传 int 的 chan
func sum(s []int, c chan int) {
	sum := 0
	for _, v := range s {
		sum += v
	}
	c <- sum // send sum to c
}

func main() {
	s := []int{7, 2, 8, -9, 4, 0}

	c := make(chan int)
	go sum(s[:len(s)/2], c)
	go sum(s[len(s)/2:], c)
	x, y := <-c, <-c // receive from c

	fmt.Println(x, y, x+y)  // -5 17 12
}
```

#### Buffered Channels

Channels can be buffered. Provide the buffer length as the second argument to make to initialize a buffered channel:

```go
ch := make(chan int, 100)
```

**Sends to a buffered channel block only when the buffer is full. Receives block when the buffer is empty.**

```go
package main

import "fmt"

func main() {
	ch := make(chan int, 2)  // 容量是 2
	ch <- 1
	ch <- 2
	// 注意这里是先进先出
	fmt.Println(<-ch)  // 1
	fmt.Println(<-ch)  // 2
}
```

如果让 buffer 满了的话，如下。

```go
package main

import "fmt"

func main() {
	ch := make(chan int, 2)  // 容量是 2
	ch <- 1
	ch <- 2
	ch <- 3  // fatal error: all goroutines are asleep - deadlock!
	// 会检测到死锁，因为 buffer 满了， ch 会在这里阻塞
	fmt.Println(<-ch)
	fmt.Println(<-ch)
}
```

这里我有个额外思考：[How to know a buffered channel is full](https://stackoverflow.com/a/25657232/17707800) 。

```go
package main

import "fmt"

func main() {
    ch := make(chan int, 1)

    // Fill it up
    ch <- 1

    select {
    case ch <- 2: // Put 2 in the channel unless it is full
    default:
        fmt.Println("Channel full. Discarding value")
    }
}
```

#### Range and Close

A sender can `close` a channel to indicate that no more values will be sent. Receivers can test whether a channel has been closed by assigning a second parameter to the receive expression: after

```go
v, ok := <-ch
```

`ok` is `false` if there are no more values to receive and the channel is closed.

The loop for `i := range c` receives values from the channel repeatedly until it is closed.

Note: **Only the sender should close a channel, never the receiver. Sending on a closed channel will cause a panic.**

Another note: **Channels aren't like files; you don't usually need to close them. Closing is only necessary when the receiver must be told there are no more values coming, such as to terminate a range loop.**

```go
package main

import (
	"fmt"
)

func fibonacci(n int, c chan int) {
	x, y := 0, 1
	for i := 0; i < n; i++ {
		c <- x
		x, y = y, x+y
	}
	close(c)
}

func main() {
	c := make(chan int, 10)
	go fibonacci(cap(c), c)
	// 这里 range 和 chan 的组合就是，只要 c 不关，那么就一直 loop
	for i := range c {
		fmt.Println(i)
	}
/*
0
1
1
2
3
5
8
13
21
34
*/
}
```

#### Select

The `select` statement lets a goroutine wait on multiple communication operations.

A `select` blocks until one of its cases can run, then it executes that case. It chooses one at random if multiple are ready.

```go
package main

import "fmt"

func fibonacci(c, quit chan int) {
	x, y := 0, 1
	for {  // 这是个 while
		select {  // select 阻塞，直到任何一个 case 可以 run
		case c <- x:
			x, y = y, x+y
		case <-quit:
			fmt.Println("quit")
			return
		}
	}
}

func main() {
	c := make(chan int)
	quit := make(chan int)
	go func() {  // 从 c 中读取 10 次，读完了将 0 送到 quit
		for i := 0; i < 10; i++ {
			fmt.Println(<-c)
		}
		quit <- 0
	}()
	fibonacci(c, quit)
}
```

#### Default Selection

The `default` case in a `select` is run if no other case is ready.

Use a `default` case to try a send or receive without blocking:

```go
select {
case i := <-c:
    // use i
default:
    // receiving from c would block
}
```

如下是一个例子。

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	tick := time.Tick(100 * time.Millisecond)
	boom := time.After(500 * time.Millisecond)
	for {
		select {
		case <-tick:
			fmt.Println("tick.")
		case <-boom:
			fmt.Println("BOOM!")
			return
		default:
			fmt.Println("    .")
			time.Sleep(50 * time.Millisecond)
		}
	}
}
/*
    .
    .
tick.
    .
    .
tick.
    .
    .
tick.
    .
    .
tick.
    .
    .
tick.
BOOM!
*/
```

### sync.Mutex

We've seen how channels are great for communication among goroutines.

But what if we don't need communication? What if we just want to make sure only one goroutine can access a variable at a time to avoid conflicts?

This concept is called mutual exclusion, and the conventional name for the data structure that provides it is mutex.

Go's standard library provides mutual exclusion with `sync.Mutex` and its two methods:

```go
Lock
Unlock
```

We can define a block of code to be executed in mutual exclusion by surrounding it with a call to `Lock` and `Unlock` as shown on the `Inc` method.

We can also use `defer` to ensure the mutex will be unlocked as in the `Value` method.

```go
package main

import (
	"fmt"
	"sync"
	"time"
)

// SafeCounter is safe to use concurrently.
type SafeCounter struct {
	mu sync.Mutex
	v  map[string]int
}

// Inc increments the counter for the given key.
func (c *SafeCounter) Inc(key string) {
	c.mu.Lock()
	// Lock so only one goroutine at a time can access the map c.v.
	c.v[key]++
	c.mu.Unlock()
}

// Value returns the current value of the counter for the given key.
func (c *SafeCounter) Value(key string) int {
	c.mu.Lock()
	// Lock so only one goroutine at a time can access the map c.v.
	defer c.mu.Unlock()  // 直接把 c.mu.Unlock 放到 Lock 方法下面的 defer 里
	return c.v[key]
}

func main() {
	c := SafeCounter{v: make(map[string]int)}
	for i := 0; i < 1000; i++ {
		go c.Inc("somekey")
	}

	time.Sleep(time.Second)
	fmt.Println(c.Value("somekey"))
}
```
