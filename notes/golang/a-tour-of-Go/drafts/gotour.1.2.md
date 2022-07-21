# Flow control statements: for, if, else, switch and defer

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [For (For is Go's "while")](#for-for-is-gos-while)
- [If](#if)
  - [with a short statement](#with-a-short-statement)
- [If and else](#if-and-else)
  - [Switch （无需 break ）](#switch-无需-break)
  - [Switch with no condition](#switch-with-no-condition)
- [Defer](#defer)
  - [Stacking defers](#stacking-defers)
  - [Defer, Panic, and Recover](#defer-panic-and-recover)
  - [Panic and Recover](#panic-and-recover)

<!-- /code_chunk_output -->

### For (For is Go's "while")

```go
package main

import "fmt"

func main() {
	sum := 0
	// Note: Unlike other languages like C, Java, or JavaScript
	// there are no parentheses surrounding the three components
	// of the for statement and the braces { } are always required.
	for i := 0; i < 10; i++ {
		sum += i
	}
	fmt.Println(sum)
}
```

当然也可以省略一些表达式。

```go
package main

import "fmt"

func main() {
	sum := 1
	for ; sum < 1000; {
		sum += sum
	}
	fmt.Println(sum)
}
```

甚至可以如下写法，直接当成 while 。

```go
package main

import "fmt"

func main() {
	sum := 1
	for sum < 1000 {
		sum += sum
	}
	fmt.Println(sum)
}
```

甚至可以再精简。

```go
package main

func main() {
	for {
	}
}
```

### If

#### with a short statement

```go
package main

import (
	"fmt"
	"math"
)

func sqrt(x float64) string {
	if x < 0 {
		return sqrt(-x) + "i"
	}
	return fmt.Sprint(math.Sqrt(x))
}

func main() {
	fmt.Println(sqrt(2), sqrt(-4))
}
```

这个跟 C 有点像，几个表达式连在一起，返回最后一个表达式的值，这里叫做 If with a short statement 。

```go
package main

import (
	"fmt"
	"math"
)

func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {
		return v
	}
	return lim
}

func main() {
	fmt.Println(
		pow(3, 2, 10),
		pow(3, 3, 20),
	)
}
```

### If and else

```go
package main

import (
	"fmt"
	"math"
)

func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {
		return v
	} else {
		// g用来输出实数，它根据数值的大小，自动选f格式或e格式
		fmt.Printf("%g >= %g\n", v, lim)
	}
	// can't use v here, though
	return lim
}

func main() {
	fmt.Println(
		pow(3, 2, 10),
		pow(3, 3, 20),
	)
}
```

#### Switch （无需 break ）

Go's switch is like the one in C, C++, Java, JavaScript, and PHP, **except that Go only runs the selected case, not all the cases that follow.**

```go
package main

import (
	"fmt"
	"runtime"
)

func main() {
	fmt.Print("Go runs on ")
	switch os := runtime.GOOS; os {
	case "darwin":
		fmt.Println("OS X.")
	case "linux":
		fmt.Println("Linux.")
	default:
		// freebsd, openbsd,
		// plan9, windows...
		fmt.Printf("%s.\n", os)
	}
}
```

For example,

```go
switch i {
case 0:
case f():
}
```

does not call `f` if `i==0`.

#### Switch with no condition

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	t := time.Now()
    // Switch without a condition is the same as switch true.
	switch {
	case t.Hour() < 12:
		fmt.Println("Good morning!")
	case t.Hour() < 17:
		fmt.Println("Good afternoon.")
	default:
		fmt.Println("Good evening.")
	}
}
```

### Defer

```go
package main

import "fmt"

func main() {
	defer fmt.Println("world")

	fmt.Println("hello")
	/* 在作用域内其他函数返回后， defer 才会执行
hello
world
	*/
}
```

#### Stacking defers

```go
package main

import "fmt"

func main() {
	fmt.Println("counting")

	for i := 0; i < 10; i++ {
		defer fmt.Println(i)
	}

	fmt.Println("done")
/* 多个 defer 是栈的关系
counting
done
9
8
7
6
5
4
3
2
1
0
*/
}
```

#### Defer, Panic, and Recover

这里有篇官方的 2010 年的 blog 。

```go
// https://go.dev/blog/defer-panic-and-recover
package main

import "fmt"

func a() {
    i := 0
	// A deferred function’s arguments are evaluated when the defer statement is evaluated.
	// 说白了就是 i=0 作为值传给了 defer fmt.Println 放到栈里，并不是最后再把 i=1 传给 fmt.Println
	// 所以这里最终打印 0 而非 1
    defer fmt.Println(i)
    i++
    return
}

func main() {
	fmt.Println("main")

	a()  // 0

	fmt.Println("main done")
}
```

**return 的操作并非原子的！**

```go
// https://go.dev/blog/defer-panic-and-recover
// https://tiancaiamao.gitbooks.io/go-internals/content/zh/03.4.html
package main

import "fmt"

// Deferred functions may read and assign to the returning function’s
// named return values. In this example, a deferred function increments
// the return value i after the surrounding function returns.
// Thus, this function returns 2.
func c() (i int) {
	// 注意！这里并不会在 return 执行结束后再执行 defer
	// 因为 return 操作并非原子！
	// return 实际上首先是对 i 赋值，然后执行 ret
	// defer 在 ret 指令前被调用，也就是 i ++ ，因此最终的 i 值是 1
    defer func() { i++ }()
    return 0
}

func main() {
	fmt.Println("main")

	fmt.Printf("c() is %v\n", c())

	fmt.Println("main done")
}
```

#### Panic and Recover

Panic is a built-in function that stops the ordinary flow of control and begins panicking. When the function F calls panic, execution of F stops, any deferred functions in F are executed normally, and then F returns to its caller. To the caller, F then behaves like a call to panic. The process continues up the stack until all functions in the current goroutine have returned, at which point the program crashes. Panics can be initiated by invoking panic directly. **They can also be caused by runtime errors, such as out-of-bounds array accesses.**

在函数 panic 后， defer 依然会执行。

Recover is a built-in function that regains control of a panicking goroutine. **Recover is only useful inside deferred functions. During normal execution, a call to recover will return nil and have no other effect. If the current goroutine is panicking, a call to recover will capture the value given to panic and resume normal execution.**

简言之， Recover 只能在 defer 函数中，且用来从 panic 中夺回控制权。

Here’s an example program that demonstrates the mechanics of panic and defer:

```go
package main

import "fmt"

func main() {
    f()
    fmt.Println("Returned normally from f.")
}

func f() {
    defer func() {
        if r := recover(); r != nil {
            fmt.Println("Recovered in f", r)
        }
    }()
    fmt.Println("Calling g.")
    g(0)
    fmt.Println("Returned normally from g.")
}

func g(i int) {
    if i > 3 {
        fmt.Println("Panicking!")
        panic(fmt.Sprintf("%v", i))
    }
    defer fmt.Println("Defer in g", i)
    fmt.Println("Printing in g", i)
    g(i + 1)
}
```

输出如下。

```go
Calling g.
Printing in g 0
Printing in g 1
Printing in g 2
Printing in g 3
Panicking!
Defer in g 3  // g panic 了，依然会执行 defer
Defer in g 2
Defer in g 1
Defer in g 0
Recovered in f 4  // 我们的 recover 也是定义在 defer 中的，因此也会在此时执行，并且夺回控制权
Returned normally from f.  // 注意到没有 Returned normally from g.
// 因为这个被 panic 掉了，我们直接跳到 defer 中了
```

Other uses of defer (beyond the file.Close example given earlier) include releasing a mutex:

```go
mu.Lock()
defer mu.Unlock()
```

printing a footer:

```go
printHeader()
defer printFooter()
```

and more.
