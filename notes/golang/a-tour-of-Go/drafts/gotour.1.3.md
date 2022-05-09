# More types: structs, slices, and maps

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Pointers](#pointers)
- [Structs](#structs)
- [Pointers to structs （取址不像 C 无需写箭头）](#pointers-to-structs-取址不像-c-无需写箭头)
- [Struct Literals](#struct-literals)
- [Arrays](#arrays)
- [Slices](#slices)
  - [Slices are like references to arrays](#slices-are-like-references-to-arrays)
  - [Slice literals](#slice-literals)
  - [Slice length and capacity](#slice-length-and-capacity)
  - [nil 判断是否为无数据（无数据不代表空切片）](#nil-判断是否为无数据无数据不代表空切片)
  - [Creating a slice with make](#creating-a-slice-with-make)
  - [Slices of slices](#slices-of-slices)
  - [Appending to a slice](#appending-to-a-slice)
  - [深入 Slices: usage and internals](#深入-slices-usage-and-internals)
- [Range](#range)
- [Maps](#maps)
  - [Map literals](#map-literals)
  - [Mutating Maps](#mutating-maps)
- [Function values](#function-values)
- [Function closures](#function-closures)

<!-- /code_chunk_output -->

### Pointers

Go has pointers. A pointer holds the memory address of a value.

The type `*T` is a pointer to a T value. Its zero value is `nil`.

```go
var p *int
```

The `&` operator generates a pointer to its operand.

```go
i := 42
p = &i
```

The `*` operator denotes the pointer's underlying value.

```go
fmt.Println(*p) // read i through the pointer p
*p = 21         // set i through the pointer p
```

This is known as "dereferencing" or "indirecting".

**Unlike C, Go has no pointer arithmetic.**

```go
package main

import "fmt"

func main() {
	i, j := 42, 2701

	p := &i         // point to i
	fmt.Println(*p) // read i through the pointer
	*p = 21         // set i through the pointer
	fmt.Println(i)  // see the new value of i

	p = &j         // point to j
	*p = *p / 37   // divide j through the pointer
	fmt.Println(j) // see the new value of j
}
```

### Structs

```go
package main

import "fmt"

type Vertex struct {
	X int
	Y int
}

func main() {
	fmt.Println(Vertex{1, 2})  // {1 2}
}
```

Struct fields are accessed using a dot.

```go
package main

import "fmt"

type Vertex struct {
	X int
	Y int
}

func main() {
	v := Vertex{1, 2}
	v.X = 4
	fmt.Println(v.X)
}
```

### Pointers to structs （取址不像 C 无需写箭头）

To access the field `X` of a struct when we have the struct pointer `p` we could write `(*p).X`. **However, that notation is cumbersome, so the language permits us instead to write just `p.X`, without the explicit dereference.**

```go
package main

import "fmt"

type Vertex struct {
	X int
	Y int
}

func main() {
	v := Vertex{1, 2}
	p := &v
	p.X = 1e9
	fmt.Println(v)
}
```

### Struct Literals

```go
package main

import "fmt"

type Vertex struct {
	X, Y int
}

var (
	v1 = Vertex{1, 2}  // has type Vertex
	v2 = Vertex{X: 1}  // Y:0 is implicit
	v3 = Vertex{}      // X:0 and Y:0
	p  = &Vertex{1, 2} // has type *Vertex
)

func main() {
	fmt.Println(v1, p, v2, v3)  // {1 2} &{1 2} {1 0} {0 0}
}
```

可以用 `X: 0` 赋值。

### Arrays

The type `[n]T` is an array of `n` values of type `T`.

The expression

```go
var a [10]int
```

declares a variable a as an array of ten integers.

**An array's length is part of its type, so arrays cannot be resized.** This seems limiting, but don't worry; Go provides a convenient way of working with arrays.

```go
package main

import "fmt"

func main() {
	var a [2]string
	a[0] = "Hello"
	a[1] = "World"
	fmt.Println(a[0], a[1])  // Hello World
	fmt.Println(a)  // [Hello World]

	primes := [6]int{2, 3, 5, 7, 11, 13}
	fmt.Println(primes)  // [2 3 5 7 11 13]
}
```

### Slices

An array has a fixed size. **A slice, on the other hand, is a dynamically-sized, flexible view into the elements of an array. In practice, slices are much more common than arrays.**

The type `[]T` is a slice with elements of type `T`.

A slice is formed by specifying two indices, a low and high bound, separated by a colon:

```go
a[low : high]
```

This selects a half-open range which includes the first element, but excludes the last one.

The following expression creates a slice which includes elements 1 through 3 of `a`:

```go
a[1:4]
```

```go
package main

import "fmt"

func main() {
	primes := [6]int{2, 3, 5, 7, 11, 13}

	var s []int = primes[1:4]
	fmt.Println(s)  // [3 5 7]
	fmt.Printf("s %T; primes %T", s, primes)  // s []int; primes [6]int
}
```

#### Slices are like references to arrays

**A slice does not store any data, it just describes a section of an underlying array.**

Changing the elements of a slice modifies the corresponding elements of its underlying array.

Other slices that share the same underlying array will see those changes.

**所以要注意， go 里的切片与 python 不同， python 切片将得到一个新对象；而 go 的切片不存储数据，是在原对象上修改。可以这么记忆： python 是脚本语言， go 贴近底层，前者无需考虑内存，后者需要考虑内存。**

```go
package main

import "fmt"

func main() {
	names := [4]string{
		"John",
		"Paul",
		"George",
		"Ringo",
	}
	fmt.Println(names)  // [John Paul George Ringo]

	a := names[0:2]
	b := names[1:3]
	fmt.Println(a, b)  // [John Paul] [Paul George]

	b[0] = "XXX"
	fmt.Println(a, b)  // [John XXX] [XXX George]
	fmt.Println(names)  // [John XXX George Ringo]
}
```

#### Slice literals

A slice literal is like an array literal without the length.

```go
package main

import "fmt"

func main() {
	q := []int{2, 3, 5, 7, 11, 13}
	fmt.Println(q)

	r := []bool{true, false, true, true, false, true}
	fmt.Println(r)

	s := []struct {
		i int
		b bool
	}{
		{2, true},
		{3, false},
		{5, true},
		{7, true},
		{11, false},
		{13, true},
	}
	fmt.Println(s)
}
```

关于切片的默认值如下。

```go
package main

import "fmt"

func main() {
	s := []int{2, 3, 5, 7, 11, 13}

	s = s[1:4]
	fmt.Println(s)  // [3 5 7]

	s = s[:2]
	fmt.Println(s)  // [3 5]

	s = s[1:]
	fmt.Println(s)  // [5]
}
```

#### Slice length and capacity

```go
package main

import "fmt"

func main() {
	s := []int{2, 3, 5, 7, 11, 13}
	printSlice(s)  // len=6 cap=6 [2 3 5 7 11 13]

	// Slice the slice to give it zero length.
	s = s[:0]
	printSlice(s)  // len=0 cap=6 []

	// Extend its length.
	s = s[:4]
	printSlice(s)  // len=4 cap=6 [2 3 5 7]

	// Drop its first two values.
	s = s[2:]
	printSlice(s)  // len=2 cap=4 [5 7]
}

func printSlice(s []int) {
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)
}
```

#### nil 判断是否为无数据（无数据不代表空切片）

```go
package main

import "fmt"

func main() {
	var s []int  // 这里不会调用构造函数
	fmt.Println(s, len(s), cap(s))  // [] 0 0
	if s == nil {
		fmt.Println("nil!")  // nil!
	}
}
```

#### Creating a slice with make

这里要思考内存中 Slice 什么样子的，应该是如下的。

```
变量 |data|len|cap|
其中 data 是一个指针，指向变量在内存中的第一个元素
```

```go
package main

import "fmt"

func main() {
	a := make([]int, 5)
	printSlice("a", a)  // a len=5 cap=5 [0 0 0 0 0]

	b := make([]int, 0, 5)
	printSlice("b", b)  // b len=0 cap=5 []

	c := b[:2]
	printSlice("c", c)  // c len=2 cap=5 [0 0]

	d := c[2:5]
	printSlice("d", d)  // d len=3 cap=3 [0 0 0]
}

func printSlice(s string, x []int) {
	fmt.Printf("%s len=%d cap=%d %v\n",
		s, len(x), cap(x), x)
}
```

#### Slices of slices

```go
package main

import (
	"fmt"
	"strings"
)

func main() {
	// Create a tic-tac-toe board.
	board := [][]string{
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
	}

	// The players take turns.
	board[0][0] = "X"
	board[2][2] = "O"
	board[1][2] = "X"
	board[1][0] = "O"
	board[0][2] = "X"

	for i := 0; i < len(board); i++ {
		fmt.Printf("%s\n", strings.Join(board[i], " "))
	}
	// X _ X
	// O _ X
	// _ _ O
}
```

#### Appending to a slice

```go
package main

import "fmt"

func main() {
	var s []int
	printSlice(s)  // len=0 cap=0 []

	// append works on nil slices.
	s = append(s, 0)
	printSlice(s)  // len=1 cap=1 [0]

	// The slice grows as needed.
	s = append(s, 1)
	printSlice(s)  // len=2 cap=2 [0 1]

	// We can add more than one element at a time.
	s = append(s, 2, 3, 4)
	printSlice(s)  // len=5 cap=6 [0 1 2 3 4]
}

func printSlice(s []int) {
	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)
}
```

#### 深入 Slices: usage and internals

https://go.dev/blog/slices-intro

### Range

```go
package main

import "fmt"

var pow = []int{1, 2, 4, 8, 16, 32, 64, 128}

func main() {
	for i, v := range pow {
		fmt.Printf("2**%d = %d\n", i, v)
	}
}
```

如果只用 index ，那么 `i := range list` 就行。注意这里的 `i` 是 index，而不是 value （与绝大部分语言不同）。

```go
package main

import "fmt"

func main() {
	pow := make([]int, 10)
	for i := range pow {
		pow[i] = 1 << uint(i) // == 2**i
	}
	for _, value := range pow {
		fmt.Printf("%d\n", value)
	}
}
```

### Maps

```go
package main

import "fmt"

type Vertex struct {
	Lat, Long float64
}

// The zero value of a map is nil. A nil map has no keys, nor can keys be added.
var m map[string]Vertex

func main() {
	m = make(map[string]Vertex)
	m["Bell Labs"] = Vertex{
		40.68433, -74.39967,
	}
	fmt.Println(m["Bell Labs"])  // {40.68433 -74.39967}
}
```

#### Map literals

```go
package main

import "fmt"

type Vertex struct {
	Lat, Long float64
}

var m = map[string]Vertex{
	"Bell Labs": Vertex{
		40.68433, -74.39967,
	},
	"Google": Vertex{
		37.42202, -122.08408,
	},
}

func main() {
	fmt.Println(m)  // map[Bell Labs:{40.68433 -74.39967} Google:{37.42202 -122.08408}]
}
```

此外，对于内部的大括号还可以自己调用构造函数。

```go
package main

import "fmt"

type Vertex struct {
	Lat, Long float64
}

var m = map[string]Vertex{
	"Bell Labs": {40.68433, -74.39967},
	"Google":    {37.42202, -122.08408},
}

func main() {
	fmt.Println(m)  // map[Bell Labs:{40.68433 -74.39967} Google:{37.42202 -122.08408}]
}
```

#### Mutating Maps


Insert or update an element in map `m`:
```go
m[key] = elem
```

Retrieve an element:
```go
elem = m[key]
```

Delete an element:
```go
delete(m, key)
```

Test that a key is present with a two-value assignment:
```go
elem, ok = m[key]
```

If `key` is in `m`, ok is `true`. If not, `ok` is `false`.

If `key` is not in the map, then `elem` is the zero value for the map's element type.

Note: If `elem` or `ok` have not yet been declared you could use a short declaration form:
```go
elem, ok := m[key]
```

```go
package main

import "fmt"

func main() {
	m := make(map[string]int)

	m["Answer"] = 42
	fmt.Println("The value:", m["Answer"])  // The value: 42

	m["Answer"] = 48
	fmt.Println("The value:", m["Answer"])  // The value: 48

	delete(m, "Answer")
	fmt.Println("The value:", m["Answer"])  // The value: 0

	v, ok := m["Answer"]
	fmt.Println("The value:", v, "Present?", ok)  // The value: 0 Present? false
}
```

### Function values

Functions are values too. They can be passed around just like other values.

Function values may be used as function arguments and return values.

```go
package main

import (
	"fmt"
	"math"
)

func compute(fn func(float64, float64) float64) float64 {
	return fn(3, 4)
}

func main() {
	hypot := func(x, y float64) float64 {
		return math.Sqrt(x*x + y*y)
	}
	fmt.Println(hypot(5, 12))  // 13

	fmt.Println(compute(hypot))  // 5
	fmt.Println(compute(math.Pow))  // 81
}
```

### Function closures

Go functions may be closures. A closure is a function value that references variables from outside its body. The function may access and assign to the referenced variables; in this sense the function is "bound" to the variables.

这里的闭包和 JavaScript 很像，也是每个闭包有一份作用域外的变量。

```go
package main

import "fmt"

func adder() func(int) int {
	sum := 0
	return func(x int) int {
		sum += x
		return sum
	}
}

func main() {
	// the adder function returns a closure.
	// Each closure is bound to its own sum variable.
	pos, neg := adder(), adder()
	for i := 0; i < 10; i++ {
		fmt.Println(
			pos(i),
			neg(-2*i),
		)
	}
}
```
