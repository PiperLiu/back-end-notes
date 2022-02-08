# 4 | 设计与声明

Designs and Declarations

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [18 | 让接口容易被正确使用，不易被误用](#18-让接口容易被正确使用不易被误用)
- [19 | 设计 class 犹如设计 type](#19-设计-class-犹如设计-type)
- [20 | 宁以 pass-by-reference-to-const 替换 pass-by-value](#20-宁以-pass-by-reference-to-const-替换-pass-by-value)
  - [传引用可以防止参数被切割](#传引用可以防止参数被切割)
- [21 | 必须返回对象时，别妄想返回其 reference](#21-必须返回对象时别妄想返回其-reference)
  - [函数创建新对象之 stack-based 与 heap-based 的问题](#函数创建新对象之-stack-based-与-heap-based-的问题)
- [22 | 将成员变量声明为 private](#22-将成员变量声明为-private)
- [23 | 宁以 non-member 、 non-friend 替换 member 函数](#23-宁以-non-member-non-friend-替换-member-函数)
  - [使用命名空间函数替代成员函数](#使用命名空间函数替代成员函数)
- [24 | 若所有参数皆需类型转换，请为此次采用 non-member 函数](#24-若所有参数皆需类型转换请为此次采用-non-member-函数)
  - [成员函数中的函数难以对外部变量转换类型](#成员函数中的函数难以对外部变量转换类型)
- [25 | 考虑写出一个不抛出异常的 swap 函数](#25-考虑写出一个不抛出异常的-swap-函数)
  - [典型的 std::swap 可能浪费性能](#典型的-stdswap-可能浪费性能)
  - [全特化 total template specialization](#全特化-total-template-specialization)
  - [偏特化 paritally specialize 与 swap class template 而非 classes](#偏特化-paritally-specialize-与-swap-class-template-而非-classes)

<!-- /code_chunk_output -->

### 18 | 让接口容易被正确使用，不易被误用

Make interfaces easy to use correctly and hard to use incorrectly.

### 19 | 设计 class 犹如设计 type

Treat class design as type design.

新 type 的对象如果被 passed by value ，意味着什么？记住， copy 构造函数函数用来定义一个 type 的 pass-by-value 该如何实现。

如果你希望允许类型 T1 之物被隐式转换为类型 T2 之物，就必须在 class T1 内写一个类型转换函数（ operator T2 ）或在 class T2 内写一个 non-explicit-one-argument （可被单一实参调用）的构造函数。

### 20 | 宁以 pass-by-reference-to-const 替换 pass-by-value

Prefer pass-by-reference-to-const to pass-by-value.

#### 传引用可以防止参数被切割

参数切割就是，传入一个对象，结果发现只传进去了其一部分（比如其父类的）参数。

```cpp
class Window {
public:
    ...
    std::string name() const;
    virtual void display() const;
};

class WindowWithScrollBars: public Window {
public:
    ...
    virtual void display() const;
};

/**
 * 下述代码不正确，因为直接传入 w
 * 导致 w 将会按照 Window 的构造函数构造一次
 * 如果传入 WindowWithScrollBars ，也会被当成 Window 构造
 * 也就会调用 Window 的 display 方法
 */
void printNameAndDisplay(Window w)
{
    std::cout << w.name();
    w.display();
}

void printNameAndDisplay(const Window& w)
{   // 没有问题
    ...
}
```

### 21 | 必须返回对象时，别妄想返回其 reference

Don't try to return reference when you must return an object.

#### 函数创建新对象之 stack-based 与 heap-based 的问题

stack-based

```cpp
const Rational& operator* (const Rational& lhs,
                           const Rational& rhs)
{
    /**
     * 这将是非常糟糕的代码
     * result 是一个 local 对象，将在退出函数时被销毁
     */
    Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
    return result;
}
```

heap-based

```cpp
const Rational& operator* (const Rational& lhs,
                           const Rational& rhs)
{
    /**
     * 这是更糟糕的写法
     * 这里付出了构造函数的代价，此外对于 w = x * y * z
     * 即 w = operator*(operator*(x, y), z) ，我们无法对 x * y 的对象进行 delete
     */
    Rational* result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);
    return *result;
}
```

解决方法是，直接返回新对象，如下。

```cpp
const Rational& operator* (const Rational& lhs,
                           const Rational& rhs)
{
    return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
}
```

### 22 | 将成员变量声明为 private

Declare data members private.

为了使用时更好的一致性，以及封装性。

### 23 | 宁以 non-member 、 non-friend 替换 member 函数

Prefer non-member non-friend functions to member functions.

首先应该明确，我们应该减少能够直接访问 private 变量的函数，这使得类有更好的“封装性”。

因此应该尽量少使用 member 或者 friend 函数。

#### 使用命名空间函数替代成员函数

下面这种写法不好，使用命名空间改进。

```cpp
class WebBrowser {
public:
    ...
    void clearCache();
    void clearHistory();
    void removeCookies();
    ...
    void clearEverything();
};

WebBrowser::clearEverything()
{
    clearCache();
    clearHistory();
    removeCookies();
}
```

用命名空间改进。要知道 namespace 和 classes 不同，前者可以跨越多个源码文件而后者不能。

```cpp
// webbrowser.h
// class WebBrowser 自身以及核心技能
namespace WebBrowserStuff {
class WebBrowser { ... };
void clearBrowser(WebBrowser& wb);
...  // 其他 non-member 函数
}

WebBrowserStuff::clearBrowser(WebBrowserStuff::WebBrowser& wb)
{
    wb.clearCache();
    wb.clearHistory();
    wb.removeCookies();
}

// webbrowserbookmarks.h
namespace WebBrowserStuff {
    ...  // 与书签相关的便利函数
}

// webbrowsercookies.h
namespace WebBrowserStuff {
    ...  // 与 cookie 相关的便利函数
}
```

将所有便利函数放在多个头文件内但隶属于同一个命名空间，意味着客户可以轻松扩展这一组便利函数。他们需要做的就是添加更多 non-member non-friend 函数到此命名空间内。

### 24 | 若所有参数皆需类型转换，请为此次采用 non-member 函数

Declare non-member functions when type conversions should apply to all parameters.

#### 成员函数中的函数难以对外部变量转换类型

```cpp
class Rational {
public:
    Rational(int numerator = 0,
             int denominator = 1);
    int numerator() const;
    int denominator() const;

    const Rational operator* (const Rational& rhs) const;

private:
    ...
};
```

上述方案存在以下问题。

```cpp
// 假设已经写了隐式转换函数
Rational oneHalf(1, 2);
result = oneHalf * 2;  // 没问题
result = 2 * oneHalf;  // 错误
result = 2.operator*(oneHalf);  // 错误，相当于上一句
```

解决方案如下。

```cpp
const Rational operator*(const Rational& lhs,
                         const Rational& rhs)
{
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
}
```

### 25 | 考虑写出一个不抛出异常的 swap 函数

Consider support for a non-throwing swap.

#### 典型的 std::swap 可能浪费性能

```cpp
namespace std
{
    template<typename T>
    void swap(T& a, T& b)
    {
        T temp(a);
        a = b;
        b = temp;
    }
}
```

如上的 swap 在某些情况下，可能导致很多“无用功”。如下例。

```cpp
class WidgetImpl {
public:
    ...
private:
    int a, b, c;
    std::vector<double> v;  // 复制时间可能很长
}

class Widget {
public:
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs)
    {
        ...
        *pImpl = *(rhs.pImpl);
        ...
    }
private:
    WidgetImpl* pImpl;
}
```

如上，我置换两个 Widget 对象的值，只是希望置换其中的 pImpl 指针，但是如果我们用 std::swap 会导致其复制三个 Widgets 以及三个 WidgetImpl 对象！

#### 全特化 total template specialization

我们似乎可以写写成下面这个样子：

```cpp
namespace std {
    template<>
    void swap<Widget>(Widget& a, Widget& b)
    {
        swap(a.pImpl, b.pImpl);
    }
}
```

这个 `template<>` 叫做 std::swap 的 全特化 total template specialization 版本。

问题在于， pImpl 是 private 的。

我们可以改进一下，就对了。

```cpp
class Widget {
public:
    ...
    void swap(Widget& other)
    {
        using std::swap;
        swap(pImpl, other.pImpl);
    }
private:
    WidgetImpl* pImpl;
}

namespace std {
    template<>
    void swap<Widget>(Widget& a, Widget& b)
    {
        a.swap(b);
    }
}
```

#### 偏特化 paritally specialize 与 swap class template 而非 classes

```cpp
template<typename T>
class WidgetImpl { ... };
template<typename T>
class Widget { ... };

namespace std {
    template<typename T>
    void swap< Widget<T> >(Widget<T>& a, Widget<T>& b)
    { a.swap(b); }
}
```

如上在进行偏特化 paritally specialize 一个 function template （std::swap），但是 C++ 不允许， C++ 里只能偏特化 class template 。

因此我们做一个重载版本 swap ，如下。

```cpp
namespace std {
    template<typename T>
    void swap(Widget<T>& a, Widget<T>& b)
    { a.swap(b); }
}
```

如上，还是不行。为什么？ C++ 标准委员会不允许膨胀已经声明好的东西，比如 std 里不能添加新的 template 。

那该怎么办？使用命名空间，如下。

```cpp
namespace WidgetStuff {
    ...
    template<typename T>
    class Widget { ... };
    ...
    template<typename T>
    void swap(Widget<T>& a, Widget<T>& b)
    { a.swap(b); }
}
```

如上，对于 Widget 对象，我们直接调用 swap 就行，因为 C++ 会根据 name lookup rules ，找到 WidgetStuff 中的专属版本。

在调用时，注意 using std::swap 一下，让 std::swap 被曝光，方便编译器挑选最合适的 swap 。

```cpp
template<typename T>
void doSomething(T& obj1, T& obj2)
{
    using std::swap;
    ...
    swap(obj1, obj2);
    ...
}
```

总结一下：
- 首先，如果 swap 的缺省实现效率可接受，则不用做什么
- 其次，如果效率不足，则自己实现
  - 第一，提供一个 public swap 成员函数，高效置换，这个函数不该抛出异常
  - 第二，在 class 或 template 所在的命名空间内提供一个 non-member swap ，并令它调用上述 swap 成员函数
  - 第三，如果正在编写一个 class 而非 class template ，为 class 特化 std::swap 并令他调用 swap 成员函数
- 最后，如果调用 swap ，确定包含一个 using 声明式，调用时，赤裸裸地调用 swap
