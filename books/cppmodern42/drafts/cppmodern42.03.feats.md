# 3 | 转向现代 C++

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [7 | 在创建对象时注意区分 () 和 {}](#7-在创建对象时注意区分-和)
  - [使用大括号的调用构造函数（喜忧参半）](#使用大括号的调用构造函数喜忧参半)
  - [关于 std::vector 接口设计的败笔（与可变模板配合）](#关于-stdvector-接口设计的败笔与可变模板配合)
- [8 | 优先选用 nullptr ，而非 0 或 NULL](#8-优先选用-nullptr-而非-0-或-null)

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
