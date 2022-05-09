# Packages, variables, and functions

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [package main](#package-main)
- [Imports](#imports)
- [函数 Functions](#函数-functions)
  - [share a type, multiple results](#share-a-type-multiple-results)
  - [Named return values](#named-return-values)
- [Variables](#variables)
  - [var](#var)
  - [Short variable declarations](#short-variable-declarations)
- [Basic types](#basic-types)
  - [%T %v](#t-v)
  - [Zero values](#zero-values)
  - [Type conversions](#type-conversions)
  - [Type inference](#type-inference)
  - [Constants, Numeric Constants](#constants-numeric-constants)

<!-- /code_chunk_output -->

### package main

```go
// Programs start running in package main.
package main

import (
	"fmt"
	"math/rand"
)

func main() {
	fmt.Println("My favorite number is", rand.Intn(10))
}
```

### Imports

This code groups the imports into a parenthesized, "factored" import statement.

You can also write multiple import statements, like:

```go
import "fmt"
import "math"
```

But it is good style to use the factored import statement.

### 函数 Functions

#### share a type, multiple results

```go
package main

import "fmt"

func add(x int, y int) int {
	return x + y
}

func main() {
	fmt.Println(add(42, 13))
}
```

简化如下。

```go
package main

import "fmt"

// When two or more consecutive named function parameters share a type,
// you can omit the type from all but the last.
func add(x, y int) int {
	return x + y
}

func main() {
	fmt.Println(add(42, 13))
}
```

返回多个结果如下。

```go
package main

import "fmt"

func swap(x, y string) (string, string) {
	return y, x
}

func main() {
	a, b := swap("hello", "world")
	fmt.Println(a, b)
}
```

#### Named return values

```go
package main

import "fmt"

// Go's return values may be named.
// If so, they are treated as variables defined at the top of the function.
func split(sum int) (x, y int) {
	x = sum * 4 / 9
	y = sum - x
	return
}

func main() {
	fmt.Println(split(17))
}
```

### Variables

#### var

```go
package main

import "fmt"

var c, python, java bool

func main() {
	var i int
	fmt.Println(i, c, python, java)  // 0 false false false

}
```

初值设置方式如下。

```go
package main

import "fmt"

var i, j int = 1, 2

func main() {
	var c, python, java = true, false, "no!"
	fmt.Println(i, j, c, python, java)  // 1 2 true false no!
}
```

#### Short variable declarations

```go
package main

import "fmt"

func main() {
	var i, j int = 1, 2
	// i, j int := 1, 2  // syntax error: unexpected int, expecting := or = or comma

	// Inside a function, the := short assignment statement
	// can be used in place of a var declaration with implicit type.
	// Outside a function, every statement begins with a keyword
	// (var, func, and so on) and so the := construct is not available.
	k := 3
	c, python, java := true, false, "no!"

	fmt.Println(i, j, k, c, python, java)
}
```

### Basic types

#### %T %v

Go's basic types are

```go
bool

string

int  int8  int16  int32  int64
uint uint8 uint16 uint32 uint64 uintptr

byte // alias for uint8

rune // alias for int32
     // represents a Unicode code point

float32 float64

complex64 complex128
```

The example shows variables of several types, and also that variable declarations may be "factored" into blocks, as with import statements.

The `int`, `uint`, and `uintptr` types are usually 32 bits wide on 32-bit systems and 64 bits wide on 64-bit systems. When you need an integer value you should use int unless you have a specific reason to use a sized or unsigned integer type.

```go
package main

import (
	"fmt"
	"math/cmplx"
)

var (
	ToBe   bool       = false
	MaxInt uint64     = 1<<64 - 1
	z      complex128 = cmplx.Sqrt(-5 + 12i)
)

func main() {
	fmt.Printf("Type: %T Value: %v\n", ToBe, ToBe)  // Type: bool Value: false
	fmt.Printf("Type: %T Value: %v\n", MaxInt, MaxInt)  // Type: uint64 Value: 18446744073709551615
	fmt.Printf("Type: %T Value: %v\n", z, z)  // Type: complex128 Value: (2+3i)
}
```

#### Zero values

Variables declared without an explicit initial value are given their zero value.

The zero value is:

```
0 for numeric types,
false for the boolean type, and
"" (the empty string) for strings.
```

```go
package main

import "fmt"

func main() {
	var i int
	var f float64
	var b bool
	var s string
	fmt.Printf("%v %v %v %q\n", i, f, b, s)  // 0 0 false ""
}
```

#### Type conversions

The expression `T(v)` converts the value `v` to the type `T`.

Some numeric conversions:

```go
var i int = 42
var f float64 = float64(i)
var u uint = uint(f)
```

Or, put more simply:

```go
i := 42
f := float64(i)
u := uint(f)
```

Unlike in C, **in Go assignment between items of different type requires an explicit conversion.**

```go
package main

import (
	"fmt"
	"math"
)

func main() {
	var x, y int = 3, 4
	// var f float64 = math.Sqrt(x*x + y*y)
	// 上面会报错：cannot use x * x + y * y (value of type int) as type float64 in argument to math.Sqrt
	var f float64 = math.Sqrt(float64(x*x + y*y))
	var z uint = uint(f)
	fmt.Println(x, y, z)
}
```

#### Type inference

```go
package main

import "fmt"

func main() {
	v := 42
	fmt.Printf("v is of type %T\n", v)
}
```

#### Constants, Numeric Constants

```go
package main

import "fmt"

// Constants cannot be declared using the := syntax.
const Pi = 3.14

func main() {
	const World = "世界"
	fmt.Println("Hello", World)
	fmt.Println("Happy", Pi, "Day")

	const Truth = true
	fmt.Println("Go rules?", Truth)
}
```

Go 的 const 似乎可以是无类型的？

```go
package main

import "fmt"

// Go 的 const 是无类型的？
const (
	// Create a huge number by shifting a 1 bit left 100 places.
	// In other words, the binary number that is 1 followed by 100 zeroes.
	Big = 1 << 100
	// Shift it right again 99 places, so we end up with 1<<1, or 2.
	Small = Big >> 99
)

func needInt(x int) int { return x*10 + 1 }
func needFloat(x float64) float64 {
	return x * 0.1
}

func main() {
	fmt.Println(needInt(Small))  // 21
	fmt.Println(needFloat(Small))  // 0.2
	fmt.Println(needFloat(Big))  // 1.2676506002282295e+29

	// cannot use Big (untyped int constant 1267650600228229401496703205376)
	// as int value in argument to fmt.Println (overflows)
	fmt.Println("%T", Big)
}
```
