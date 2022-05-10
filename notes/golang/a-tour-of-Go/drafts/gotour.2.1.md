# Methods and interfaces

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Methods](#methods)
  - [关于 type](#关于-type)
  - [Pointer receivers （拷贝一个还是传引用）](#pointer-receivers-拷贝一个还是传引用)
  - [Pointers and functions](#pointers-and-functions)
  - [Methods and pointer indirection](#methods-and-pointer-indirection)
- [Interfaces](#interfaces)
  - [Interfaces are implemented implicitly](#interfaces-are-implemented-implicitly)
  - [Interface values with nil underlying values](#interface-values-with-nil-underlying-values)
  - [The empty interface](#the-empty-interface)
- [Type assertions](#type-assertions)
  - [使用括号两种返回值](#使用括号两种返回值)
  - [Type switches](#type-switches)
- [Stringers](#stringers)
- [Errors](#errors)
- [Readers](#readers)

<!-- /code_chunk_output -->

### Methods

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

// Go does not have classes. However, you can define methods on types.
// A method is a function with a special receiver argument.
// The receiver appears in its own argument list between the func keyword and the method name.
// In this example, the Abs method has a receiver of type Vertex named v.
func (v Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(v.Abs())
}
```

此外， **Methods are functions** ，如下是函数写法。

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

func Abs(v Vertex) float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func main() {
	v := Vertex{3, 4}
	fmt.Println(Abs(v))
}
```

#### 关于 type

```go
package main

import (
	"fmt"
	"math"
)

type MyFloat float64

// You can declare a method on non-struct types, too.
func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

// You can only declare a method with a receiver
// whose type is defined in the same package as the method.
// You cannot declare a method with a receiver whose type
// is defined in another package
// (which includes the built-in types such as int).
func main() {
	f := MyFloat(-math.Sqrt2)
	fmt.Println(f.Abs())
}
```

#### Pointer receivers （拷贝一个还是传引用）

You can declare methods with pointer receivers.

This means the receiver type has the literal syntax `*T` for some type `T`. (Also, `T` cannot itself be a pointer such as `*int`.)

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

// 这里改为 v *Vertex 效率会不会更高？（不用调用拷贝构造函数了？）
// 答：会更好，工程上还是尽量传 Pointer receivers
// 此外， go 中没有拷贝构造函数概念，有的是 mem cpy
func (v Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

// 可以理解为这里传的是 v 对象的引用？
// 因此会改变 v 的值
// 如果这里改为 v Vertex
// 则 main 中 print 值为 5
func (v *Vertex) Scale(f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

func main() {
	v := Vertex{3, 4}
	v.Scale(10)
	fmt.Println(v.Abs())  // 50
}
```

#### Pointers and functions

注意，如果改为纯函数，如果接收函数是指针，则必须要“传引用”，否则会报错。

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

func Abs(v Vertex) float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func Scale(v *Vertex, f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

func main() {
	v := Vertex{3, 4}
	Scale(&v, 10)  // 这里必须传入 &v
	fmt.Println(Abs(v))
}
```

#### Methods and pointer indirection

```go
package main

import "fmt"

type Vertex struct {
	X, Y float64
}

func (v *Vertex) Scale(f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

func ScaleFunc(v *Vertex, f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

func main() {
	v := Vertex{3, 4}
	fmt.Printf("v: %T\n", v)  // v: main.Vertex
	v.Scale(2)
	ScaleFunc(&v, 10)

	p := &Vertex{4, 3}
	fmt.Printf("p: %T\n", p)  // p: *main.Vertex
	p.Scale(3)
	ScaleFunc(p, 8)

	fmt.Println(v, p)  // {60 80} &{96 72}
}
```

There are two reasons to use a pointer receiver.
- The first is so that the method can modify the value that its receiver points to.
- The second is to avoid copying the value on each method call. This can be more efficient if the receiver is a large struct, for example.

```go
package main

import (
	"fmt"
	"math"
)

type Vertex struct {
	X, Y float64
}

// In this example, both Scale and Abs are with receiver type *Vertex,
// even though the Abs method needn't modify its receiver.
// In general, all methods on a given type should have
// either value or pointer receivers, but not a mixture of both.

func (v *Vertex) Scale(f float64) {
	v.X = v.X * f
	v.Y = v.Y * f
}

func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func main() {
	v := &Vertex{3, 4}
	// %+v &{X:3 Y:4}
	// %v  &{3 4}
	fmt.Printf("Before scaling: %+v, Abs: %v\n", v, v.Abs())  // Before scaling: &{X:3 Y:4}, Abs: 5
	v.Scale(5)
	fmt.Printf("After scaling: %+v, Abs: %v\n", v, v.Abs())  // After scaling: &{X:15 Y:20}, Abs: 25
}
```

### Interfaces

An interface type is defined as a set of method signatures.

A value of interface type can hold any value that implements those methods.

```go
package main

import (
	"fmt"
	"math"
)

type Abser interface {
	Abs() float64
}

func main() {
	var a Abser
	f := MyFloat(-math.Sqrt2)
	v := Vertex{3, 4}

	// 注意， MyFloat Vertex* 都在后文实现了 Abs() float64 方法
	// 所以这里才可以给 Abser a 赋值
	a = f  // a MyFloat implements Abser
	a = &v // a *Vertex implements Abser

	fmt.Println(a.Abs())
}

type MyFloat float64


func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}


type Vertex struct {
	X, Y float64
}

func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}
```

#### Interfaces are implemented implicitly

```go
package main

import "fmt"

// A type implements an interface by implementing its methods.
//     There is no explicit declaration of intent, no "implements" keyword.
// Implicit interfaces decouple the definition of an interface
//     from its implementation, which could then appear
//     in any package without prearrangement.
type I interface {
	M()
}

type T struct {
	S string
}

// This method means type T implements the interface I,
// but we don't need to explicitly declare that it does so.
func (t T) M() {
	fmt.Println(t.S)
}

func main() {
	var i I = T{"hello"}
	i.M()
}
```

Under the hood, interface values can be thought of as a tuple of a value and a concrete type:

```
(value, type)
```

An interface value holds a value of a specific underlying concrete type.

Calling a method on an interface value executes the method of the same name on its underlying type.

```go
package main

import (
	"fmt"
	"math"
)

type I interface {
	M()
}

type T struct {
	S string
}

func (t *T) M() {
	fmt.Println(t.S)
}

type F float64

func (f F) M() {
	fmt.Println(f)
}

func main() {
	// i 是接口 I 的实例
	var i I

	i = &T{"Hello"}
	describe(i)  // (&{Hello}, *main.T)
	i.M()        // Hello

	i = F(math.Pi)
	describe(i)  // (3.141592653589793, main.F)
	i.M()        // 3.141592653589793
}

func describe(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

#### Interface values with nil underlying values

**In some languages this would trigger a null pointer exception, but in Go it is common to write methods that gracefully handle being called with a nil receiver.**

```go
package main

import "fmt"

type I interface {
	M()
}

type T struct {
	S string
}

// If the concrete value inside the interface itself is nil,
//     the method will be called with a nil receiver.
func (t *T) M() {
	if t == nil {
		fmt.Println("<nil>")
		return
	}
	fmt.Println(t.S)
}

func main() {
	var i I

	var t *T  // t 仅仅声明了，是 nil
	i = t
	describe(i)  // (<nil>, *main.T)
	i.M()        // <nil>

	i = &T{"hello"}
	describe(i)  // (&{hello}, *main.T)
	i.M()        // hello
}

func describe(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

A nil interface value holds neither value nor concrete type.

Calling a method on a nil interface is a run-time error because there is no type inside the interface tuple to indicate which concrete method to call.

```go
package main

import "fmt"

type I interface {
	M()
}

func main() {
	var i I  // i 连实例化都没有，自然 I.M() runtime error
	describe(i)  // (<nil>, <nil>)
	i.M()  // panic: runtime error: invalid memory address or nil pointer dereference
}

func describe(i I) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

#### The empty interface

he interface type that specifies zero methods is known as the empty interface:

```go
interface{}
```

**An empty interface may hold values of any type. (Every type implements at least zero methods.)**

Empty interfaces are used by code that handles values of unknown type. For example, `fmt.Print` takes any number of arguments of type `interface{}`.

Go 的 interface 是很灵活的，在我看来，就是规范数据与方法的组合。

```go
package main

import "fmt"

func main() {
	var i interface{}
	describe(i)  // (<nil>, <nil>)

	i = 42
	describe(i)  // (42, int)

	i = "hello"
	describe(i)  // (hello, string)
}

func describe(i interface{}) {
	fmt.Printf("(%v, %T)\n", i, i)
}
```

### Type assertions

#### 使用括号两种返回值

```go
package main

import "fmt"

func main() {
	var i interface{} = "hello"

	s := i.(string)
	fmt.Println(s)  // hello

	s, ok := i.(string)
	fmt.Println(s, ok)  // hello true

	f, ok := i.(float64)
	fmt.Println(f, ok)  // 0 false

	f = i.(float64) // panic
	fmt.Println(f)  // panic: interface conversion: interface {} is string, not float64
}
```

如果是两个返回值，则不会触发 panic ，只不过返回值中的第二个值为布尔值。

#### Type switches

```go
package main

import "fmt"

func do(i interface{}) {
	switch v := i.(type) {
	case int:
		fmt.Printf("Twice %v is %v\n", v, v*2)
	case string:
		fmt.Printf("%q is %v bytes long\n", v, len(v))
	default:
		fmt.Printf("I don't know about type %T!\n", v)
	}
}

func main() {
	do(21)      // Twice 21 is 42
	do("hello") // "hello" is 5 bytes long
	do(true)    // I don't know about type bool!
}
```

注意这里 `switch v := i.(type)` ，只能在 `switch` 中使用 `.(type)` 。

### Stringers

One of the most ubiquitous interfaces is `Stringer` defined by the `fmt` package.

```go
type Stringer interface {
    String() string
}
```

A `Stringer` is a type that can describe itself as a string. The `fmt` package (and many others) look for this interface to print values.

这里：所有 type 都默认继承了 `Stringer` 的 `interface` ，因此都有 `String() string` 的方法。或许这样更好理解。

```go
package main

import "fmt"

type Person struct {
	Name string
	Age  int
}

// 这里我们重载 Person 的 String() string 方法
// 相当于 Python 中的 __str__
func (p Person) String() string {
	return fmt.Sprintf("%v (%v years)", p.Name, p.Age)
}

func main() {
	a := Person{"Arthur Dent", 42}
	z := Person{"Zaphod Beeblebrox", 9001}
	fmt.Println(a, z)  // Arthur Dent (42 years) Zaphod Beeblebrox (9001 years)
}
```

### Errors

Go programs express error state with `error` values.

The `error` type is a built-in interface similar to `fmt.Stringer`:

```go
type error interface {
    Error() string
}
```

(As with `fmt.Stringer`, the `fmt` package looks for the `error` interface when printing values.)

Functions often return an error value, and calling code should handle errors by testing whether the error equals `nil`.

```go
i, err := strconv.Atoi("42")
if err != nil {
    fmt.Printf("couldn't convert number: %v\n", err)
    return
}
fmt.Println("Converted integer:", i)
```

**A nil `error` denotes success; a non-nil `error` denotes failure.**

```go
package main

import (
	"fmt"
	"time"
)

type MyError struct {
	When time.Time
	What string
}

// 这里相当于重载了 *MyError 的 Error() string 函数
func (e *MyError) Error() string {
	return fmt.Sprintf("at %v, %s",
		e.When, e.What)
}

// 这里 run 会返回一个 error 类型
// 因此会返回 MyError 的 error 类型
func run() error {
	return &MyError{
		time.Now(),
		"it didn't work",
	}
}

func main() {
	// 这里 run() 会返回一个 error 类型
	// 在 Println 中，会有个 case error:
	// 会调用传入对象的 Error 方法
	if err := run(); err != nil {
		// at 2009-11-10 23:00:00 +0000 UTC m=+0.000000001, it didn't work
		fmt.Println(err)
	}
}
```

### Readers

The `io` package specifies the `io.Reader` interface, which represents the read end of a stream of data.

The Go standard library contains [many implementations](https://cs.opensource.google/search?q=Read%5C(%5Cw%2B%5Cs%5C%5B%5C%5Dbyte%5C)&ss=go%2Fgo) of this interface, including files, network connections, compressors, ciphers, and others.

The `io.Reader` interface has a Read method:

```go
func (T) Read(b []byte) (n int, err error)
```

`Read` populates the given byte slice with data and returns the number of bytes populated and an error value. It returns an `io.EOF` error when the stream ends.

```go
package main

import (
	"fmt"
	"io"
	"strings"
)

func main() {
	// 读对象是 Hello, Reader! 的 string
	r := strings.NewReader("Hello, Reader!")

	b := make([]byte, 8)  // 一次读 8 个字节
	for {  // 这里的 for 就是 while
		n, err := r.Read(b)
		fmt.Printf("n = %v err = %v b = %v\n", n, err, b)
		// %q 单引号围绕的字符字面值，由Go语法安全地转义
		fmt.Printf("b[:n] = %q\n", b[:n])
		if err == io.EOF {
			break
		}
	}
/*
n = 8 err = <nil> b = [72 101 108 108 111 44 32 82]
b[:n] = "Hello, R"
n = 6 err = <nil> b = [101 97 100 101 114 33 32 82]
b[:n] = "eader!"
n = 0 err = EOF b = [101 97 100 101 114 33 32 82]
b[:n] = ""
*/
}
```
