# 2. Program Structure

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [2.1 Names](#21-names)
- [2.2 Declarations](#22-declarations)
- [2.3 Variables](#23-variables)
  - [2.3.1 Short Variable Declarations](#231-short-variable-declarations)
  - [2.3.2 Pointers](#232-pointers)
  - [2.3.3 The new Function](#233-the-new-function)
  - [2.3.4 Lifetime of Variables](#234-lifetime-of-variables)
- [2.4 Assignments](#24-assignments)
  - [2.4.1 Tuple Assignment](#241-tuple-assignment)
  - [2.4.2 Assignability](#242-assignability)
- [2.5 Type Declarations](#25-type-declarations)
- [2.6 Packages and Files](#26-packages-and-files)
  - [2.6.1 Imports](#261-imports)
  - [2.6.2 Package Initialization](#262-package-initialization)
- [2.7 Scope](#27-scope)

<!-- /code_chunk_output -->

### 2.1 Names

Go 的 25 个关键字中，没见过 `fallthrough` ，是与 `switch` 一起用的，强制执行接下来的 `case` 。

Go 的常量有四个： `true`, `false`, `iota`, `nil` 。 `itoa` 似乎是一个语法糖，如下用法：

```go
const (
    a = iota  // 0
    b         // 1
    _         // 2
    d         // 3
    e = 100   // 100 插队， itoa 中断
    f         // 100
)
```

### 2.2 Declarations

### 2.3 Variables

#### 2.3.1 Short Variable Declarations

#### 2.3.2 Pointers

#### 2.3.3 The new Function

#### 2.3.4 Lifetime of Variables

这里还没有提及闭包，但是明确告诉读者：一个变量放在堆上还是内存上，并不是由我们的 go 代码来决定的，而是编译器来决定的。

摘抄一下原文最为准确： A compiler may choose to allocate local variables on the heap or on the stack but, perhaps surprisingly, this choice is not determined by whether `var` or `new` was used to declare the variable.

```go
var global *int
func f() {
    var x int
    x = 1
    global = &x
}
func g() {
    y := new(int)
    *y = 1
}
```

Here, x must be heap-allocated because it is still reachable from the variable global after f has returned, despite being declared as a local variable; we say x escapes from f. Conversely, when g returns, the variable `*y` becomes unreachable and can be recycled. Since `*y` does not escape from g, it's safe for the compiler to allocate `*y` on the stack, even though it was allocated with `new`.

### 2.4 Assignments

#### 2.4.1 Tuple Assignment

#### 2.4.2 Assignability

### 2.5 Type Declarations

### 2.6 Packages and Files

#### 2.6.1 Imports

#### 2.6.2 Package Initialization

### 2.7 Scope
