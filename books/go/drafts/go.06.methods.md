# 6. Methods

In Go, an object is simply a value or variable that has methods, and a method is a function associated with a particular type.

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [6.1 Method Declarations](#61-method-declarations)
- [6.2 Methods with a Pointer Receiver](#62-methods-with-a-pointer-receiver)
  - [6.2.1 Nil is Valid Receiver Value](#621-nil-is-valid-receiver-value)
- [6.3 Composing Types by Struct Embedding](#63-composing-types-by-struct-embedding)
- [6.4 Method Values and Expressions](#64-method-values-and-expressions)
- [6.5 Example: Bit Vector Type](#65-example-bit-vector-type)
- [6.6 Encapsulation](#66-encapsulation)

<!-- /code_chunk_output -->

### 6.1 Method Declarations

之前没想到的是也可以让一个 Array 作为 `type` 。

```go
type path []Point
func (path Path) Distance() float64 {
    sum := 0.0
    for i := range path {
        if i > 0 {
            sum += path[i - 1].Distance(path[i])
        }
    }
    return sum
}

perim := Path {
    {1, 1}, {5, 1}, {5, 4}, {1, 1},
}
fmt.Println(perim.Distance())  // "12"
```

### 6.2 Methods with a Pointer Receiver

#### 6.2.1 Nil is Valid Receiver Value

即便是 `nil` ，也是我们规定的 `type` （与 Python 不同， Python 的 `None` 没有成员方法）。因此我们可以在成员方法中特判 `nil` 进行处理。

```go
func (list *IntList) Sum() int {
    if list == nil {
        return 0
    }
    return list.Value + list.Tail.Sum()
}
```

### 6.3 Composing Types by Struct Embedding

### 6.4 Method Values and Expressions

这里与 Python 类似，实际上成员方法传入的第一个参数是 `self` 或者说 `this` 。

```go
type Point struct{ X, Y float64 }
func (p Point) Add(q Point) Point { return Point{p.X + q.X, p.Y + q.Y} }
func (p Point) Sub(q Point) Point { return Point{p.X - q.X, p.Y - q.Y} }

type Path []Point

func (path Path) TranslateBy(offset Point, add bool) {
    var op func(p, q Point) Point
    if add {
        op = Point.Add
    } else {
        op = Point.Sub
    }
    for i := range path {
        path[i] = op(path[i], offset)
    }
}
```

### 6.5 Example: Bit Vector Type

见 [../gopl.io/ch6/intset/](../gopl.io/ch6/intset/) 。

### 6.6 Encapsulation

