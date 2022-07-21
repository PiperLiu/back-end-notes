# 5. Functions

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [5.1 Function Declarations](#51-function-declarations)
- [5.2 Recursion](#52-recursion)
- [5.3 Multiple Return Values](#53-multiple-return-values)
- [5.4 Errors](#54-errors)
  - [5.4.1 Error-Handling Strategies](#541-error-handling-strategies)
  - [5.4.2 End of File (EOF)](#542-end-of-file-eof)
- [5.5 Function Values](#55-function-values)
- [5.6 Anonymous Functions](#56-anonymous-functions)
  - [5.6.1 Caveat: Capturing Iteration Variables （重要：闭包捕获循环变量的坑）](#561-caveat-capturing-iteration-variables-重要闭包捕获循环变量的坑)
- [5.7 Variadic Functions](#57-variadic-functions)
- [5.8 Deferred Function Calls](#58-deferred-function-calls)
- [5.9 Panic](#59-panic)
- [5.10 Recover](#510-recover)

<!-- /code_chunk_output -->

### 5.1 Function Declarations

### 5.2 Recursion

### 5.3 Multiple Return Values

### 5.4 Errors

#### 5.4.1 Error-Handling Strategies

#### 5.4.2 End of File (EOF)

```go
in := bufio.NewReader(os.Stdin)
for {
    r, _, err := in.ReadRune()
    if err == io.EOF {
        break  // finished reading
    }
    if err != nil {
        return fmt.Errorf("read failed: %v", err)
    }
    // ...use r...
}
```

### 5.5 Function Values

### 5.6 Anonymous Functions

这里值得注意的一点与枚举有关。如果我们的匿名函数想要递归，就要先告诉编译器我们的匿名函数是什么。

```go
// 如下正确
var visitAll func(items []string)
visitAll = func(items []string) {
    ...
    visitAll(...)
    ...
}

// 如下错误
visitAll := func(items []string) {
    ...
    visitAll(...)  // compile error: undefined: visitAll
    ...
}
```

这与 C++ 11  lambda 一个道理。

```cpp
// 正确
function<int(int)> dfs = [&](int u) -> int {
    int sum = 0;
    for (auto&& v: p[u]) sum += dfs(v);
    return sum;
}

// 错误
auto dfs = [&](int u) -> int {
    int sum = 0;
    for (auto&& v: p[u]) sum += dfs(v);
    return sum;
}
```

#### 5.6.1 Caveat: Capturing Iteration Variables （重要：闭包捕获循环变量的坑）

```go
var rmdirs []func()
for _, d := range tempDirs() {
    dir := d                // NOTE: necessary!
    os.MkdirAll(dir, 0755)  // creates parent directories too
    rmdirs = append(rmdirs, func() {
        os.RemoveAll(dir)
    })
}
...do some work...
for _, rmdir := range rmdirs {
    rmdir()  // clean up
}

// 如下的写法是错误的！
var rmdirs []func()
for _, d := range tempDirs() {
    os.MkdirAll(d, 0755)
    rmdirs = append(rmdirs, func() {
        os.RemoveAll(d)  // 错误！
    })
}

// 如下写法也是错误的！
var rmdirs []func()
dirs := tempDirs()
for i := 0; i < len(dirs); i++ {
    os.MkdirAll(dirs[i], 0755)  // OK
    rmdirs = append(rmdirs, func() {
        os.RemoveAll(dirs[i])   // incorrect!
    })
}
```

为什么非要额外来一个 `dir := d` 或者 `dir := dirs[i]` 再把 `dir` 传入闭包函数才是正确的呢？

**因为对于闭包函数而言，其捕获到的 `d` 或者 `i` 来自 `for` 循环，而这个 `for` 循环产生的变量地址从未变过，但闭包函数捕获的却是其地址。**

### 5.7 Variadic Functions

在 C 中我们获取可变参数还需要并用几个类似指针一样的东西。在 Go 里则容易很多。所以我总觉得 Go 是有大量语法糖的 C 。

```go
func sum(vals ...int) int {
	total := 0
	for _, val := range vals {
		total += val
	}
	return total
}

func errorf(linenum int, format string, args ...interface{}) {
    fmt.Fprintf(os.Stderr, "Line %d: ", linenum)
    fmt.Fprintf(os.Stderr, format, args...)
    fmt.Fprintln(os.Stderr)
}
```

### 5.8 Deferred Function Calls

### 5.9 Panic

### 5.10 Recover

我之前记的笔记不错：[../../../notes/golang/a-tour-of-Go/drafts/gotour.1.2.md](../../../notes/golang/a-tour-of-Go/drafts/gotour.1.2.md)
