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

