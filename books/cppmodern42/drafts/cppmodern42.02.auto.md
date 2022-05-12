# 2 | auto

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [5 | 优先选用 auto ，而非显式型别声明](#5-优先选用-auto-而非显式型别声明)
  - [auto 对 std::function 声明的优化（更好地应用闭包）](#auto-对-stdfunction-声明的优化更好地应用闭包)
  - [auto 性能优化实例（unordered_map）](#auto-性能优化实例unordered_map)
- [6 | 当 auto 推到的型别不符合要求时，使用带显式型别的初始化物习惯用法](#6-当-auto-推到的型别不符合要求时使用带显式型别的初始化物习惯用法)

<!-- /code_chunk_output -->

### 5 | 优先选用 auto ，而非显式型别声明

使用 `auto` 深层次的好处是，可以阻止那些由于手动指定型别带来的潜在错误和性能问题。

#### auto 对 std::function 声明的优化（更好地应用闭包）

如下是闭包。

```cpp
// C++ 11
auto derefUPLess = 
    [](const std::unique_ptr<Widget>& p1,
       const std::unique_ptr<Widget>& p2)
    { return *p1 < *p2; };

// C++ 14
auto derefUPLess =
    [](const auto& p1,
       const auto& p2)
    { return *p1 < *p2; };
```

**如上我们用变量持有闭包，只有编译器知道它们具体是什么。**

`std::function` 指涉任何可调用对象。或许你觉得用 `std::function` 效果相同，但是不是的。

```cpp
std::function<bool(const std::unique_ptr<Widget>&,
                   const std::unique_ptr<Widget>&)>
    derefUPLess = [](const std::unique_ptr<Widget>& p1,
                     const std::unique_ptr<Widget>& p2)
                    { return *p1 < *p2; };
```

使用 `std::function` 声明相比 `auto` 有两个劣势：
- `auto` 由编译器确定，其内存量与闭包一样大； `std::function` 声明是一个 `std::function` 的实例，所以不管签名是什么，其内存都是固定的，对于其存储的闭包，其会在堆上申请内存，因此体积更大；
- `auto` 可以内联，而后者往往会被编译器拒绝内联，有额外函数调用的开销，因此更慢。

#### auto 性能优化实例（unordered_map）

```cpp
std::unordered_map<std::string, int> m;

// 前者
for (const std::pair<std::string, int>& p : m) {
    std::cout << p.first << ": " << p.second << std::endl;
}

// 后者
for (const auto& p : m) {
    std::cout << p.first << ": " << p.second << std::endl;
}
```

后者效率更高，因为在 `unordered_map` 内部实际上是 `std::pair<const std::string, int>` ，你错误的声明将导致编译器为 `pair` 每次都拷贝构造一个非 `const string` 的 `pair` 出来。

### 6 | 当 auto 推到的型别不符合要求时，使用带显式型别的初始化物习惯用法

“隐形”代理类和 `auto` 无法和平共处。

```cpp
std::vector<bool> features(const Widget& w);
auto x = features(w)[5];
```

注意，现在 `x` 并不是 `bool` ，而是 `std::vector<bool>::reference` 。 `auto` 并不会帮我们隐式转换类型，这导致 `features` 函数调用结束后，中间产生的 `std::vector<bool>` 将被析构，这样这个 `reference` 就会没有所指，在应用时会报错。

正确用法：

```cpp
int index = d * c.size();
auto index = static_cast<int>(d * c.size());
```
