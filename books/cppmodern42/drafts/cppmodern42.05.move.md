# 5 | 右值引用、移动语义和完美转发

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [23 | 理解 std::move 和 std::forward](#23-理解-stdmove-和-stdforward)
  - [move 基本原理与 move “失效”的场景（move 会保留 const 特性）](#move-基本原理与-move-失效的场景move-会保留-const-特性)
  - [forward 用处的概括](#forward-用处的概括)
- [24 | 区分万能引用和右值引用](#24-区分万能引用和右值引用)
  - [是万能引用（转发引用）的情形](#是万能引用转发引用的情形)
  - [是右值引用的情形](#是右值引用的情形)
  - [vector 中的案例与 auto&& 都是万能引用](#vector-中的案例与-auto-都是万能引用)

<!-- /code_chunk_output -->

请先记住，形参总是左值，即使其型别是右值引用。即，给定函数形如 `void f(Widget&& w)` 形参 `w` 是个左值。

### 23 | 理解 std::move 和 std::forward

要注意， **`std::move` 并不进行任何移动， `std::forward` 也不进行任何转发。二者在运行期都什么也不会做，只不过是一个强制型别转换的函数（其实是函数模板）。** `std::move` 无条件地将实参强制转换成右值，而 `std::forward` 则仅在某个特定条件满足时才执行同一个强制转换。

#### move 基本原理与 move “失效”的场景（move 会保留 const 特性）

下面算是标准但是简易的 `move` 实现。

```cpp
// C++ 11
template<typename T>  // 位于 std namespace 内
typename remove_reference<T>::type&&
move(T&& param)
{
    using ReturnType = typename remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

// C++ 14
template<typename T>  // 位于 std namespace 内
decltype(auto)
move(T&& param)
{
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}
```

如果 `T` 是个左值引用，那么 `T&&` 由于是个万能引用，因此 `T&&` 就成了左值引用。 `remove_reference` 就可以将引用型别取消。

虽然 `move` 强制转为右值（右值代表实施移动），但并不是一定能保证移动会产生而非复制。

```cpp
class Annotation {
public:
    explicit Annotation(const std::string& text)
    : text_(std::move(text)) {}
private:
    std::string text_;
};
```

注意，虽然在初始化时，把 `text` 转换为右值，但是 `text_` 并非是 `text` 的移动，而是 `text` 的复制。

为什么？注意观察 `std::string` 的构造函数如下。

```cpp
class string {
public:
    string(const string& rhs);  // 复制构造函数
    string(string&& rhs);       // 移动构造函数
}
```

注意，在我们的 `Annotation` 中， `text` 本身是个 `const` 引用，就算 `move` 后也是个 `const` 右值。编译器在考察 `string` 的构造函数时，无法调用移动构造函数，因为移动构造函数没有用于接收 `const` 的参数的。而指涉到的常量的左值引用允许绑定到一个常量右值型别的形参，因此最后会调用复制构造函数。

所以总结一下：
- 如果想取得对某个对象执行移动操作能力，则不要将其声明为常量
- `std::move` 不仅不实际移动任何东西，甚至不保证经过其强制型别转换后的对象具备可移动的能力

#### forward 用处的概括

```cpp
void process(const Widget& lvalArg);  // 处理左值
void process(Widget&& rvalArg);       // 处理右值

template<typename T>
void logAndProcess(T&& param)  // 万能引用，保存 param 各自性质
{
    auto now = std::chrono::system_clock::now();
    makeLogEntry("Calling 'process'", now);
    process(std::forward<T>(param));
}

Widget w;
logAndProcess(w);             // 调用时传入左值
logAndProcess(std::move(w));  // 调用时传入右值
```

注意，我们为什么要使用 `std::forward` ？如果直接把 `param` 传入 `process` ，那么 `process` 就会接收一个左值，而不是一个右值。 **因为所有函数形参皆为左值。** 而 `std::forward` 可以将 `T` 的性质完美地保留下来，传给 `process` 。其原理将在条款 28 讨论。

### 24 | 区分万能引用和右值引用

`T&&` 有两种不同含义：
- 右值引用，用于识别出可移对象
- 万能引用 universal reference ，也被称为转发引用 forwarding reference

#### 是万能引用（转发引用）的情形

参考第一章，一般而言，遇到形如 `T&&` ，有型别推导，为万能引用。

```cpp
template<typename T>
void f(T&& param);  // param 是万能引用

template<typename MyTemplateType>
void f(MyTemplateType&& param);  // param 是万能引用

auto&& var2 = var1;  // var2 是万能引用
```

#### 是右值引用的情形

如果是 `T&&` 却没有型别推导，那么是右值引用。

```cpp
void f(Widget&& param);  // param 是右值引用（不涉及型别推导）
```

注意，万能引用仅能是 `T&&` ，加了一个 `const` 或者部分已知的模板类型都是右值引用。

```cpp
template<typename T>
void f(std::vector<T>&& param);  // param 是右值引用

std::vector<int> v;
f(v);  // 报错，因为要求传入右值

template<typename T>
void f(const T&& param);  // param 是右值引用
```

#### vector 中的案例与 auto&& 都是万能引用

以下来自 C++ 标准。

```cpp
template<class T, class Allocator = allocator<T>>
class vector {
public:
    void push_back(T&& x);  // 注意！是右值引用
};
```

`push_back` 中的 `T` 实际上不需要型别推导，完全是跟着类的 `T` 模板编译走的。比如 `std::vector<Widget> v` 会导致模板具现化如下。

```cpp
class vector<Widget, allocator<Widget>> {
public:
    void push_back(Widget&& x);  // 右值引用
};
```

而 `emplace_back` 却是涉及了型别推导，是万能引用。

```cpp
template<class T, class Allocator = allocator<T>>
class vector {
public:
    template<class... Args>
    void emplace_back(Args&&... args);  // 注意！是万能引用
};
```

最后，在 C++ 14 中我们可以更好地万能引用（`auto&&`）。比如下面这个计算任意函数调用时长的函数。

```cpp
auto timeFuncInvocation = 
    [](auto&& func, auto&&... params)
    {
        // 计时器启动
        std::foward<decltype(func)>(func)(
            std::forward<decltype(params)>(params)...
            );
        // 计时器停止，记录花费时间
    }
```
