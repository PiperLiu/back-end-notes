# 3 | 转向现代 C++

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [7 | 在创建对象时注意区分 () 和 {}](#7-在创建对象时注意区分-和)
  - [使用大括号的调用构造函数（喜忧参半）](#使用大括号的调用构造函数喜忧参半)
  - [关于 std::vector 接口设计的败笔（与可变模板配合）](#关于-stdvector-接口设计的败笔与可变模板配合)
- [8 | 优先选用 nullptr ，而非 0 或 NULL](#8-优先选用-nullptr-而非-0-或-null)
- [9 | 优先选用别名声明，而非 typedef](#9-优先选用别名声明而非-typedef)
  - [别名模板 alias template](#别名模板-alias-template)
- [10 | 优先选用限定作用域的枚举型别，而非不限定作用域的枚举型别](#10-优先选用限定作用域的枚举型别而非不限定作用域的枚举型别)
  - [enum class 的前置声明](#enum-class-的前置声明)
- [11 | 优先选用删除函数，而非 private 未定义函数](#11-优先选用删除函数而非-private-未定义函数)
  - [通过删除函数拒绝型别与（指针）模板特化](#通过删除函数拒绝型别与指针模板特化)

<!-- /code_chunk_output -->

### 7 | 在创建对象时注意区分 () 和 {}

#### 使用大括号的调用构造函数（喜忧参半）

```cpp
Widget w2();  // 可能被当成声明还是
// 因此可以用
Widget w3{};
```

但是，如果构造函数中带有 `std::initializer_list` 的形参，那么更有可能造成歧义。

```cpp
class Widget {
public:
    Widget();
    Widget(int i, bool b);
    Widget(int i, double d);
    Widget(std::initializer_list<long double> il);
}

Widget w1(1, true);  // 调用第二个构造函数
Widget w2{1, true};  // 调用第四个构造函数，并且强制类型转换

// 注意，如果是空 {} ，则规定一定该调用 Widget() 构造函数
Widget w3{};
// 如果想要调用第四个构造函数，且传入空 list ，那么如下写
Widget w4({});
```

#### 关于 std::vector 接口设计的败笔（与可变模板配合）

书中认为如下设计是败笔，因为用户使用大括号和小括号是两种行为，导致需要额外记忆区别。

```cpp
// 创建一个 vector ， 10 个元素都是 20
std::vector<int> v1(10, 20);
// 创建一个 vector 是 {10, 20}
std::vector<int> v2{10, 20};
```

如果你现在应用一个别人开发的库（`doSomeWork`来自这个库），书写了如下代码。

```cpp
std::vector<int> v;
doSomeWork<std::vector<int>>(10, 20);
```

你并不知道 `doSomeWork` 内部具体怎么实现的，这就糟糕了，如下。

```cpp
template <typename T,
          typename... Ts>  // 可变模板
void doSomeWork(Ts&&... args) {
    // 利用 params 创建局部对象 T
    T localObject(std::forward<Ts>(args)...);
    T localObject{std::forward<Ts>(args)...};
}
```

那么，用小括号或者大括号将导致完全不同的结果。

**这正是标准库函数 `std::make_unique` 和 `std::make_shared` 所面临的问题。而这些函数解决问题的办法是在内部使用了小括号，并把这个决定以文档的形式广而告之，作为其接口的组成部分。**

### 8 | 优先选用 nullptr ，而非 0 或 NULL

C++ 的基本观点是 0 的型别是 `int` ，而非指针。因此使用 0 或者 NULL 会导致首先被以为成整数。

### 9 | 优先选用别名声明，而非 typedef

C++ 11 提供了别名声明 alias declaration ：

```cpp
using UPtrMapSS = 
  std::unique_ptr<std::unordered_map<std::string, std::string>>;

// C++ 98
typedef void (*FP)(int, const std::string&);
// C++ 11
using FP = void (*)(int, const std::string&);
```

#### 别名模板 alias template

别名声明可以模板化， `typedef` 就不行。

```cpp
// C++ 11
template<typename T>
using MyAllocaList = std::<T, MyAlloc<T>>;

// C++ 98
template<typename T>
struct MyAllocaList {
    typedef std::<T, MyAlloc<T>> type;
};
MyAllocaList<int>::type myList;  // 用户代码
```

### 10 | 优先选用限定作用域的枚举型别，而非不限定作用域的枚举型别

```cpp
// C++ 98
enum Color { Red, Green, Blue };
auto Red = false;  // 报错
```

C++ 11 中提供限定范围的枚举型别 `enum class` 。

```cpp
// C++ 11
enum class Color { Red, Green, Blue };
auto Red = false;  // 没问题
Color c = Red;  // 报错，因为没有 Red ，只有 Color::Red
Color c = Color::Red;  // 没问题
```

#### enum class 的前置声明

此外，还应注意限定作用域的枚举型别底层型别是 `int` ，而不限定作用域的型别没有默认底层型别。

```cpp
enum class Status;  // 底层型别 int

// 前置声明
#include <cstdint>
enum class Status : std::uint32_t;
```

### 11 | 优先选用删除函数，而非 private 未定义函数

我们在 《Effective C++》 中学过可以把函数放在 `private` 中阻止外部调用，但是这过时了。

如下， C++ 98 中 `basic_ios` 像下面这样让输入流和输出流不可赋值（甚至连注释也是标准的一部分）：

```cpp
// C++ 98
template <class charT, class traits = char_traits<charT> >
class basic_ios : public ios_base {
public:
    ...
private:
    basic_ios(const basic_ios&);             // not defined
    basic_ios& operator=(const basic_ios&);  // not defined
}
```

对于 C++ 11 来说，我们有删除函数 `=delete` 标识 deleted function 。更直观，更有效，让编译器更早发现问题。

```cpp
// C++ 98
template <class charT, class traits = char_traits<charT> >
class basic_ios : public ios_base {
public:
    basic_ios(const basic_ios&) = delete;
    basic_ios& operator=(const basic_ios&) = delete;
}
```

#### 通过删除函数拒绝型别与（指针）模板特化

```cpp
bool isLucky(int number);
bool isLucky(char) = delete;  // 拒绝传入 char 类型（防止 char 被隐式转换为 int）
```

对于模板而言，我们定义了一个模板指针（`T* ptr`），要注意防止两个异类：
- `void*` 指针，无法对其执行提领、自增、自减操作
- `char*` 指针，是 C 风格的字符串，而非涉及到单个字符的指针

删除函数可以帮助我们做到这点：

```cpp
template<typename T>
void processPointer(T* ptr);

// 特化一下
template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;
// const 也要删除
template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;
```

另外，我们想在类里通过 `private` 删除模板特化几乎不可能，如下。

```cpp
// C++ 98
class Widget {
public:
    ...
    template<typename T>
    void processPointer(T* ptr)
    { ... }
private:
    template<>  // 报错！
    void processPointer<void>(void*);
};
```

上述会报错，因为模板特化必须在名字空间作用域内而非类作用域内撰写。

因此，如下想达到上述目标，可以如下这么写：

```cpp
// C++ 11
class Widget {
public:
    ...
    template<typename T>
    void processPointer(T* ptr)
    { ... }
};

template<>
void Widget::processPointer<void>(void*) = delete;
```
