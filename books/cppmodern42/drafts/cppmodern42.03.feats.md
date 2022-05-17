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
- [12 | 为意在改写的函数添加 override 声明](#12-为意在改写的函数添加-override-声明)
  - [引用饰词（reference qualifier）](#引用饰词reference-qualifier)
  - [加上 override 的必要性](#加上-override-的必要性)
- [13 | 优先选用 const_iterator ，而非 iterator](#13-优先选用-const_iterator-而非-iterator)
- [14 | 只要函数不会发射异常，就为其加上 noexcept 声明](#14-只要函数不会发射异常就为其加上-noexcept-声明)
  - [关于 throw() 和 noexcept 发射异常以及调用栈开解 stack unwinding](#关于-throw-和-noexcept-发射异常以及调用栈开解-stack-unwinding)
  - [大多数函数都是异常中立的（exception-neutral），那么什么时候用 noexcept](#大多数函数都是异常中立的exception-neutral那么什么时候用-noexcept)
- [只要有可能使用 constexpr ，就是用它](#只要有可能使用-constexpr-就是用它)
  - [constexpr 函数可在编译器计算](#constexpr-函数可在编译器计算)
- [16 | 保证 const 成员函数的线程安全性](#16-保证-const-成员函数的线程安全性)
  - [只移型别（move-only type）的 mutex 保证线程安全](#只移型别move-only-type的-mutex-保证线程安全)
  - [std::atomic 取代互斥量的例子](#stdatomic-取代互斥量的例子)
- [17 | 理解特种成员函数的生成机制](#17-理解特种成员函数的生成机制)
  - [移动构造函数和移动赋值运算符](#移动构造函数和移动赋值运算符)
  - [=default关键字](#default关键字)

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

### 12 | 为意在改写的函数添加 override 声明

#### 引用饰词（reference qualifier）

如果让派生类改写父类的函数，以下一系列要求必须满足：
- 基类的函数必须是虚函数
- 基类和派生类中的函数名字必须完全相同（析构函数例外）
- 基类和派生类中的函数参数型别必须完全相同
- 基类和派生类中的函数常量性（constness）必须完全相同
- 基类和派生类中的函数返回值和异常规格必须兼容
- C++ 11 新增：基类和派生类中的函数引用饰词（reference qualifier）必须完全相同

这里解释一下引用饰词（reference qualifier）：

```cpp
class Widget {
public:
    ...
    void doWork() &;   // 仅在 *this 是左值时调用
    void doWork() &&;  // 仅在 *this 是右值时调用
};

Widget makeWidget();  // 工厂函数，返回右值
Widget w;             // 普通函数，是左值

w.dowork();  // 调用 Widget::doWork() &
makeWidget().dowork();  // 调用 Widget::doWork() &&
```

一般而言应用如下。

```cpp
class Widget {
public:
    using DataType = std::vector<double>;
    ...
    DataType& data() &  // 对于左值 Widgets 型别
    { return values; }  // 返回左值

    DataType& data() && // 对于右值 Widgets 型别
    { return std::move(values); }  // 返回右值
private:
    DataType values;
};

// 客户代码如下
auto vals1 = w.data();
auto vals2 = makeWidget().data();
```

#### 加上 override 的必要性

注意，下面没有任何一个派生类中的函数和基类函数是绑定的。

```cpp
class Base {
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    void mf4() const;
};

class Derived : public Base {
public:
    virtual void mf1();
    virtual void mf2(unsigned int x);
    virtual void mf3() &&;
    void mf4() const;  // 基类中不是 virtual
};
```

实际上，如果想要让编译器帮我们判断派生类是不是继承了基类的函数，我们得加上 `override` 声明。

```cpp
class Derived : public Base {
public:
    virtual void mf1() override;  // 无法通过编译
    virtual void mf2(unsigned int x) override;  // 无法通过编译
    virtual void mf3() && override;  // 无法通过编译
    void mf4() const override;  // 无法通过编译
};
```

那么正确的写法应该如下。

```cpp
class Base {
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    virtual void mf4() const;
};

class Derived : public Base {
public:
    virtual void mf1() const override;
    virtual void mf2(int x) override;
    virtual void mf3() & override;
    void mf4() const override;  // 加个 virtual 没问题，但没必要
};
```

### 13 | 优先选用 const_iterator ，而非 iterator

书中只想强调， C++ 98 里 `const_iterator` 不完善，但是 C++ 11 里可以放心用。

```cpp
std::vector<int> values;
auto it =
    std::find(values.cbegin(), values.cend(), 42);  // 注意是 cbegin() 和 cend()
values.insert(it, 42);
```

### 14 | 只要函数不会发射异常，就为其加上 noexcept 声明

#### 关于 throw() 和 noexcept 发射异常以及调用栈开解 stack unwinding

```cpp
int f(int x) throw();  // f 不会发射异常： C++ 98 风格
int f(int x) noexcept; // f 不会发射异常： C++ 11 风格
```

上述两个总体效果相同：告诉编译器，f 不会发射任何异常；如果发生了，则不会被 try 捕获，直接调用 `std::unexpected` 。

一些细节是：
- 在 C++ 98 异常规格下，调用栈会开解至 f 的调用方，然后执行了一些与本条款无关的动作以后，程序执行中止
- 在 C++ 11 异常规格下，运行期行为会稍有不同：程序执行中止之前，栈只是可能会开解

#### 大多数函数都是异常中立的（exception-neutral），那么什么时候用 noexcept

异常中立的函数自身并不抛出异常，但是它们内部调用的函数可能会发射异常。因此，异常中立的函数应该允许发射的异常经由它传至调用栈的更深的一层。因此大多数函数不应该 noexcept 。

那什么时候用 noexcept ？

**C++ 11 中，默认所有内存释放函数和所有的析构函数都隐式地具备 noexcept 性质。**

此外，我们在 C++ 11 中想要应用好移动语义，需要保证移动操作不会发生异常。

注意， C++ 11 中的 `std::vector::push_back` 中利用了一种“能移动则移动，必须复制才能复制”（move if you can, but copy if you must）策略。

但是问题是，如果你准备移动 `vector` 中 5 个元素，在移动第 3 个元素时候出现异常了，而原始的 `vector` 已经修改，这时不可以把之前移动过的异常移动回来（因为移动本身就可能会造成异常）。因此，我们必须保证，移动这个事是 `noexcept` 的。

### 只要有可能使用 constexpr ，就是用它

与 `const` 不同， `constexpr` 是真真正正的常量。

`constexpr` 定义的变量保证在编译阶段就已知。

```cpp
int sz;

constexpr auto sz1 = sz;  // 报错， sz 在编译器未知
std::array<int, sz> data1;  // 错误，同样的问题

constexpr auto sz2 = 10;  // 没问题
std::array<int, sz2> data2;  // 没问题
```

#### constexpr 函数可在编译器计算

如果 `constexpr` 函数可以用在编译器常量语境中，如果传入的实参在编译器已知，则在编译器得到结果。

```cpp
// C++ 11
// C++ 11 只运行 constexpr 中有一个 return 语句
constexpr int pow(int base, int exp) noexcept
{
    return (exp == 0) ? 1 : base * pow(base, exp - 1);
}

// C++ 14
constexpr int pow(int base, int exp) noexcept
{
    auto result = 1;
    for (int i = 0; i < exp; ++ i) result *= base;
    return result;
}
```

但是， `constexpr` 里面有诸多限制，比如不允许 IO 等操作。

因此，引入 `constexpr` 函数还需要谨慎。

### 16 | 保证 const 成员函数的线程安全性

#### 只移型别（move-only type）的 mutex 保证线程安全

```cpp
class Polynomial {
public:
    using RootsType = std::vector<double>;

    RootsType roots() const  // 并不会改边 *this 中除了 mutable 以为内容
    {
        if (!rootsAreValid())
        {
            ...
            rootsAreValid() = true;
        }
        return rootVals;
    }
private:
    // 因为在 const 函数中
    mutable bool rootsAreValid{ false };
    mutable RootsType rootVals{};
};

Polynomial p;

// 线程 1
auto roots1 = p.roots();
// 线程 2
auto roots2 = p.roots();
```

很显然，这里会出现竞态条件，因此，我们需要保证线程安全。

```cpp
class Polynomial {
public:
    using RootsType = std::vector<double>;

    RootsType roots() const  // 并不会改边 *this 中除了 mutable 以为内容
    {
        std::lock_guard<std::mutex> g(m);
        if (!rootsAreValid())
        {
            ...
            rootsAreValid() = true;
        }
        return rootVals;
    }  // 析构 g ， m 被解锁
private:
    mutable std::mutex m;
    // 因为在 const 函数中
    mutable bool rootsAreValid{ false };
    mutable RootsType rootVals{};
};
```

值得注意的是：**std::mutex 是一个只移型别（move-only type），因此加入 mutex 让 Polynomial 市区了可复制性。**

#### std::atomic 取代互斥量的例子

首先来看 `std::atomic` 例子。实现一个变量原子操作。

```cpp
class Point {
public:
    ...
    double distanceFromOrigin() const noexcept
    {
        ++ callCount;  // 原子操作
        return std::sqrt(x * x + y * y);
    }
private:
    mutable std::atomic<unsigned> callCount{ 0 };
    double x, y;
};
```

这是 `atomic` 常规用法。

下面用 `std::atomic` 实现类似 `std::mutex` 的功能。

```cpp
class Widget {
public:
    int magicValue() const
    {
        if (cacheValid) return cacheValue;
        else {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            cacheValue = val1 + val2;  // 第一部分
            cacheValid = true;         // 第二部分
            return cacheValue;
        }
    }
private:
    mutable std::atomic<bool> cacheValid{ false };
    mutable std::atomic<int> cacheValue;
};
```

注意！这样写很危险！一旦你将“第一部分”和“第二部分”顺序调换，将得到不正确的计算结果（比如返回一个空的 `cacheValue` ）。

### 17 | 理解特种成员函数的生成机制

#### 移动构造函数和移动赋值运算符

特种成员函数是指 C++ 自行生成的成员函数：
- 默认构造函数
- 析构函数
- 复制构造函数
- 复制赋值运算符
- 移动构造函数（C++ 11）
- 移动赋值运算符（C++ 11）

```cpp
class Widget {
public:
    ...
    // 移动构造函数
    Widget(Widget&& rhs);
    // 移动赋值运算符
    Widget& operator=(Widget&& rhs);
}
```

注意，他们都是 **仅在需要时才会生成** ，且都是 public 访问层级且是 inline 的、非虚的（除非是析构函数位于派生类中，且基类析构函数是一个虚函数，这样的话自动生成的析构函数是虚函数）。

这里我主要是摘录一些规则：
- 两种复制操作是彼此独立的：声明了其中一个，并不会阻止编译器生成另一个。
- 两种移动操作并不彼此独立：声明了其中一个，就会阻止编译器生成另一个。
- 一旦显式声明了复制操作，这个类也就不再会生成移动操作了
- 一旦声明了移动操作，无论是移动构造还是移动赋值，编译器就会废除复制操作（废除方式是删除它们）

因此，这里有一个指导原则叫做“大三律”（Rule of Three），即如果你声明了复制构造函数、复制赋值运算符，或者析构函数中的任何一个，你就得同时声明所有这三个。

这里总结一下，移动操作的生成条件（如果需要生成），仅当以下三者同时成立：
- 该类未声明任何复制操作
- 该类未声明任何移动操作
- 该类未声明任何析构函数

#### =default关键字

假定你需要编译器帮你生成默认的特种成员函数，那么我们可以使用 `= default` 来实现：

```cpp
class Widget {
public:
    ...
    ~Widget();  // 用户自定义的析构函数
    ...
    Widget(const Widget&) = default;  // 默认复制构造函数的行为是正确的
    // 默认复制赋值运算符的行为是正确的
    Widget& operator=(const Widget&) = default;
};
```

一般而言，我们可以给基类应用如下组合。

```cpp
class Base {
public:
    virtual ~Base() = default;
    Base(Base&&) = default;
    Base& operator=(Base&&) = default;
    Base(const Base&) = default;
    Base& operator=(const Base&) = default;
};
```

来看一个为什么要用 `= default` 的例子。

```cpp
class StringTable {
public:
    StringTable()
    { makeLogEntry("Creating StringTable object"); }
    ~StringTable()
    { makeLogEntry("Destroying StringTable object"); }
private:
    std::map<int, std::string> values;
};
```

这里声明了析构函数，导致不会默认生成移动函数，有个巨大的副作用：代码能够通过测试，甚至包括移动操作的测试；如果我们要“移动” StringTable 对象，将导致复制 `map<int, std::string>` 对象的复制而非移动，这样将导致该操作逼移动操作慢上几个数量级。如果复制和移动操作都已经显示地用 `=default` 来定义了，则不会出现这个问题。
