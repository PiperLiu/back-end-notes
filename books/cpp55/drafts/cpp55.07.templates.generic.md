# 7 | 模板与泛型编程

Templates and Generic Programming

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [41 | 了解隐式接口和编译期多态](#41-了解隐式接口和编译期多态)
  - [显示接口 explicit interface 和隐式接口 valid expressions](#显示接口-explicit-interface-和隐式接口-valid-expressions)
- [42 | 了解 typename 的双重意义](#42-了解-typename-的双重意义)
  - [typename 告诉编译器从属名称 dependent name 也是个类型](#typename-告诉编译器从属名称-dependent-name-也是个类型)
  - [不能使用 typename 之处](#不能使用-typename-之处)
- [43 | 学习处理模板化基类内的名称](#43-学习处理模板化基类内的名称)
  - [防止“拒绝在模板化基类”中寻找继承的名称三种方法](#防止拒绝在模板化基类中寻找继承的名称三种方法)
- [44 | 将与参数无关的代码抽离 templates](#44-将与参数无关的代码抽离-templates)
- [45 | 运用成员函数模板接受所有兼容类型](#45-运用成员函数模板接受所有兼容类型)
  - [必须同时声明泛化 copy 构造函数和“正常的” copy 构造函数](#必须同时声明泛化-copy-构造函数和正常的-copy-构造函数)
- [46 | 需要类型转换时请为模板定义非成员函数](#46-需要类型转换时请为模板定义非成员函数)
  - [template 在实参推导过程中不将隐式类型转换纳入考虑](#template-在实参推导过程中不将隐式类型转换纳入考虑)
  - [应用 friend 函数让成员函数成为非成员函数](#应用-friend-函数让成员函数成为非成员函数)
- [47 | 请使用 traits classes 表现类型信息](#47-请使用-traits-classes-表现类型信息)
  - [STL 迭代器分类](#stl-迭代器分类)
  - [Traits 是一个 C++ 协议](#traits-是一个-c-协议)
- [48 | 认识 template 元编程](#48-认识-template-元编程)
  - [使用 TMP 计算阶乘](#使用-tmp-计算阶乘)

<!-- /code_chunk_output -->

### 41 | 了解隐式接口和编译期多态

Understand implicit interfaces and compile-time polymorphism.

在面向对象中，总是以显示接口和运行期多态（通过 virtual 体现）解决问题。但是在 template 以及泛型编程的世界，更多地体现隐式接口和编译期多态。

#### 显示接口 explicit interface 和隐式接口 valid expressions

```cpp
template<typename T>
void doProcessing(T& w)
{
    if (w.size() > 10 && w != someNastyWidget) {
        T temp(w);
        temp.normalize();
        temp.swap(w);
    }
}

class Widget {
public:
    Widget();
    virtual ~Widget();
    virtual std::size_t size() const;
    virtual void normalize();
    void swap(Widget& other);
}
```

如上的 Widget 就是个显示接口（签名式）。但是没有体现隐式接口。隐式接口由有效表达式 valid expression 组成，比如这里的大于号、不等于符号。

### 42 | 了解 typename 的双重意义

Understand the two meanings of typename.

```cpp
template<typename C>
template<class C>
```

如上二者用 typename 与 class 完全一致。

#### typename 告诉编译器从属名称 dependent name 也是个类型

```cpp
template<typename C>
void print2nd(const C& container)
{
    if (container.size() < 2) return ;
    C::const_iterator iter(container.begin());  // 编译错误！
    ++ iter;
    int value = *iter;
    std::cout << value;
}
```

上述会报错，因为编译器并不知道从属名称 dependent name 即 C::const_iterator 是个类型，因此我们通过 typename 告诉它。

```cpp
template<typename C>
void print2nd(const C& container)
{
    if (container.size() < 2) return ;
    typename C::const_iterator iter(container.begin());
    ++ iter;
    int value = *iter;
    std::cout << value;
}
```

注意 typename 只能被用来验明嵌套从属类型名称。

```cpp
template<typename C>               // 使用 typename 或 class
void f(const C& container,         // 不允许使用 typename
       typename C::iterator iter); // 一定要使用 typename
```

也可以用在 typedef 里：

```cpp
template<typename IterT>
void workWithIterator(IterT iter)
{
    typedef typename std::iterator_traits<IterT>::value_type value_type;
    value_type temp(*iter);
    ...
}
```

#### 不能使用 typename 之处

但是注意，不能用在 base class lists （基类列）或 member initialization list （成员初值列）内以它作为 base class 修饰符。

```cpp
template<typename IterT>
class Derived: public Base<T>::Nested {  // T 不允许在基类 Base 中
public:
    explicit Derived(int x)
    : Base<T>::Nested(x)  // 注意不允许 mem.init.list 中出现 typename 嵌套从属名称
    {
        typename Base<T>::Nested temp;  // 注意不允许作为 base class 修饰符加上 typename
        ...
    }
};
```

### 43 | 学习处理模板化基类内的名称

Know how to access names in templatized base classes.

#### 防止“拒绝在模板化基类”中寻找继承的名称三种方法

```cpp
class CompanyA {
public:
    void sendCleartext(const std::string& msg){}
    void sendEncrypted(const std::string& msg){}
};

class CompanyB {
public:
    void sendCleartext(const std::string& msg){}
    void sendEncrypted(const std::string& msg){}
};

template<typename Company>
class MsgSender {
public:
    void sendClear(const std::string& msg)
    {
        Company c;
        c.sendCleartext(msg);
    }

    void sendSecret(const std::string& msg){
        Company c;
        c.sendEncrypted(msg);
    }
};

class CompanyC {
public:
    void sendEncrypted(const std::string& msg){}
};

template<>  // 全特化，遇到 CompanyC 时如下处理
class MsgSender<CompanyC> {
public:
    void sendSecret(const std::string& msg){}
};

template<typename Company>
class LoggingMsgSender: public MsgSender<Company> {
public:
    using MsgSender<Company>::sendClear;
    void sendClearMsg(const std::string& msg)
    {
        //write "before sending" info to the log;
        sendClear(msg); // 编译报错！
        //write "after sending" info to the log;
    }
};
```

如上，因为我们全特化了 CompanyC ，模板化基类中没有 sendClear 方法，因此不会通过编译。

如何通过编译呢？

```cpp
// 第一种方法 using 让编译器去 base class 中寻找函数名
template<typename Company>
class LoggingMsgSender: public MsgSender<Company> {
public:
    using MsgSender<Company>::sendClear;
    void sendClearMsg(const std::string& msg)
    {
        sendClear(msg);
    }
};

// 第二种方法 this-> 假设这个 sendClear 将被继承
template<typename Company>
class LoggingMsgSender: public MsgSender<Company> {
public:
    void sendClearMsg(const std::string& msg)
    {
        this->sendClear(msg);
    }
};

// 注意，第三种方法可能会阻断 virtual 行文
template<typename Company>
class LoggingMsgSender: public MsgSender<Company> {
public:
    void sendClearMsg(const std::string& msg)
    {
        MsgSender<Company>::sendClear(msg);
    }
};
```

### 44 | 将与参数无关的代码抽离 templates

Factor parameter-independent code out of templates.

过度使用 templates 可能导致代码膨胀 code bloat 。

### 45 | 运用成员函数模板接受所有兼容类型

Use member function templates to accept "all compatible types."

如下，基于继承体系可以进行一些隐式转换。

```cpp
class Top {};
class Middle: public Top {};
class Bottom: public Middle {};

Top *pt1 = new Middle;  // 将 Moddle* 转换成 Top*
Top *pt2 = new Bottom;  // 将 Bottom* 转换成 Top*
const Top *pct2 = pt1;  // 将 Top* 转换为 const Top*
```

我们希望在模板间也可以完成上述隐式转换。

```cpp
SmartPtr<Top> pt1 = SmartPtr<Middle>(new Middle);
SmartPtr<Top> pt2 = SmartPtr<Bottom>(new Bottom);
SmartPtr<const Top> pct2 = pt1;
```

但是，用一个 template 的不同具体体现之间并没有什么隐式转换关系。

我们必须应用 Templates 和泛型编程 Generic Programming 声明 copy 函数。

#### 必须同时声明泛化 copy 构造函数和“正常的” copy 构造函数

```cpp
template<typename T>
class SmartPtr {
public:
    explicit SmartPtr(T *realPtr){}
    SmartPtr(SmartPtr const& r){};
    template<typename U>
    SmartPtr(const SmartPtr<U>& other)
    : heldPtr(other.get()) {}

    SmartPtr& operator=(SmartPtr const& r){};
    template<typename Y>
    SmartPtr& operator=(SmartPtr<Y> const& r){};

    T* get() const { return heldPtr; }
private:
    T *heldPtr;
};
```

如上，这里模板的拷贝构造函数 member template 没有被声明为 explicit ，因为我们希望转换是隐式的，而无需写成 cast 。

注意上面还声明了拷贝构造函数与 copy assignment 。因为如果你不声明，编译器就会替你声明。

### 46 | 需要类型转换时请为模板定义非成员函数

Define non-member functions inside templates when type conversions are desired.

#### template 在实参推导过程中不将隐式类型转换纳入考虑

```cpp
class Rational {
public:
    Rational(int numerator = 0,
             int denominator = 1)
    : m_numerator(numerator)
    , m_denominator(denominator){}

    int numerator() const{return m_numerator;}
    int denominator() const{return m_denominator;}

private:
    int m_numerator;
    int m_denominator;
};

template <typename T>
const Rational<T> operator*(const Rational<T> &lhs, const Rational<T> &rhs) {
    return Rational<T>(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
}

int main() {
    Rational oneHalf(1, 2);
    Rational oneEighth(1, 8);
    Rational result = oneHalf * oneEighth;
    result = result * oneEighth;

    result = oneHalf * 2;  // 编译错误！
    result = 2 * oneHalf;

    return 0;
}
```

如上编译不会通过。注意 **template 在实参推导过程中不将隐式类型转换纳入考虑** ，因此尽管编译器推导出操作符乘号第一个参数是 `Rational<int>` ，也不会将第二个参数 `int` 隐式转换为 `Rational<int>` 。

#### 应用 friend 函数让成员函数成为非成员函数

如何解决上述问题，我们把函数声明在 class 内，并且变为 friend ，这样这个成员函数尽管在函数内，还是个 non-member 。

```cpp
class
template <typename T>
class Rational {
public:
    Rational(const T& numerator = 0,
               const T& denominator = 1)
            : m_numerator(numerator)
            , m_denominator(denominator){}

    const T numerator() const{return m_numerator;}
    const T denominator() const{return m_denominator;}

    // 就是 Rational<T> ，但是可以省略这个 T
    friend const Rational operator*(const Rational &lhs, const Rational &rhs) {
        return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
    }
private:
    T m_numerator;
    T m_denominator;
};
```

为什么 class 内的 friend 会让 result = oneHalf * 2; 编译成功呢？

因为当 oneHalf 被声明为一个 `Rational<int>` 时， `class Rational<int>` 就被具现化出来了，而作为过程的一部分， **friend 函数也被自动声明出来了，并且是一个函数而非模板函数。**

这就可以隐式转换了。

注意，在这里 friend 的声明和定义必须在一起，否则无法通过链接过程。为什么？因为当 friend 函数也被自动声明出来了后，这是一个非模板函数，而如果把定义写在外头，则只有模板函数的定义，没有相应的非模板函数的定义。

### 47 | 请使用 traits classes 表现类型信息

User traits classes for information about types.

#### STL 迭代器分类

一共有五种，如下：

```cpp
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};
```

只有 random_access_iterator_tag 是以内置指针为榜样，可以使用 `+=` 运算。

我们希望实现一个函数：

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
{
    if (iter is a random access iterator) {
        iter += d;
    }
    else {
        if (d >= 0) { while (d--) ++iter; }
        else { while (d++) --iter; }
    }
}
```

#### Traits 是一个 C++ 协议

在这里，我们要应用 traits 实现 advance 函数。其要求之一是： **对内置（built-in）类型和用户自定义（uer-defined）类型的表现一样好。**

本例中，如何实现 traits 呢？

如下，各个容器中定义好 iterator ，并且统一 iterator_category 作为 tag 。

```cpp
template <typename T>
class deque {
public:
    class iterator {
    public:
        typedef random_access_iterator_tag iterator_category;
        ...
    }:
    ...
};

template <typename T>
class list {
public:
    class iterator {
    public:
        typedef bidirectional_iterator_tag iterator_category;
        ...
    }:
    ...
};

...
```

然后构造一个 iterator_traits ，其中 iterator_category() 用于承接各个容器的 iterator ：

```cpp
template<typename IterT>
struct iterator_traits {
    typedef typename IterT::iterator_category iterator_category;
    ...
};
```

此外，对于内置类型即内置指针，也要提供一个偏特化的版本接受：

```cpp
template<typename IterT>
struct iterator_traits<IterT*>
{
  typedef random_access_iterator_tag iterator_category;
  ...
};
```

最后，我们定义 doAdvance 针对各个容器：

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
{
    doAdvance(iter, d,
              typename iterator_traits<IterT>::iterator_category;
};
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d,
               random_access_iterator_tag)
{
    iter += d;
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d,
               bidirectional_iterator_tag)
{
    if (d >= 0) { while (d--) ++iter; }
    else { while (d++) --iter;         }
}
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d,
               input_iterator_tag)
{
    if (d < 0 ) {
        throw std::out_of_range("Negative distance");
    }
    while (d--) ++iter;
}
```

追忆承接了各种 _iterator_tag 。

那么最后的 advance 如下。

```cpp
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d)
{
    doAdvance(iter, d,
              typename std::iterator_traits<IterT>::iterator_category());
};
```

### 48 | 认识 template 元编程

Be aware of template metaprogramming.

template 元编程的图灵完备的。

#### 使用 TMP 计算阶乘

```cpp
template<unsigned n>
struct Factorial {
    enum { value = n * Factorial<n-1>::value };
};
template<>
struct Factorial<0> {
    enum { value = 1 };
};
```
