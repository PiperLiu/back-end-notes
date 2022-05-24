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
- [25 | 针对右值引用实施 std::move ，针对万能引用实施 std::forward](#25-针对右值引用实施-stdmove-针对万能引用实施-stdforward)
  - [针对右值引用实施 std::move](#针对右值引用实施-stdmove)
  - [针对万能引用实施 std::forward（为何不用 move）](#针对万能引用实施-stdforward为何不用-move)
  - [返回值优化 return value optimization, RVO](#返回值优化-return-value-optimization-rvo)
- [26 | 避免依万能引用型别进行重载](#26-避免依万能引用型别进行重载)
  - [万能引用与完美转发提高效率的例子](#万能引用与完美转发提高效率的例子)
  - [不要重载万能引用（万能引用匹配的贪婪性，优先匹配非模板函数）](#不要重载万能引用万能引用匹配的贪婪性优先匹配非模板函数)

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

### 25 | 针对右值引用实施 std::move ，针对万能引用实施 std::forward

#### 针对右值引用实施 std::move

针对右值引用实施 `std::move` 实际上是右值引用被发明的缘由。

```cpp
class Widget {
public:
    Widget(Widget&& rhs)
    : name(std::move(rhs.name)),
      p(std::move(rhs.p))
    {}
private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};
```

此外再看一个例子。

```cpp
// 按值返回
Matrix
operator+(Matrix&& lhs, const Matrix& rhs)
{
    lhs += rhs;
    return std::move(lhs);  // 将 lhs 移入返回值
}  // 如果直接 return lhs ，则会将左值 lhs 拷贝到返回值存储位置
```

#### 针对万能引用实施 std::forward（为何不用 move）

```cpp
template<typename T>
Fraction
reduceAndCopy(T&& f)
{
    frac.reduce();
    // 对于右值，是移入返回值
    // 对于左值，是复制入返回值
    return std::forward<T>(f);
}
```

如下是一个万能引用配上 `std::move` 的糟糕例子。

```cpp
class Widget {
public:
    template<typename T>
    void setName(T&& newName)
    { name = std::move(newName); }  // 很糟糕
private:
    std::string name;
    std::shared_ptr<SomeDataStructure> p;
};

std::string getWidgetName();  // 工厂函数

Widget w;
auto n = getWidgetName();  // n 是个局部变量
w.setName(n);  // n 移入了 w ！ n 的值将变得未知
```

好，那么不用万能引用，为常量左值和右值实现出不同的重载就可以用 move 了。

```cpp
class Widget {
public:
    void setName(const std::string& newName)
    { name = newName; }  // 从常量左值取得赋值
    void setName(std::string&& newName)
    { name = std::move(newName); }  // 从右值取得赋值
};
```

好，来看一个例子： `w.setName("Adela Novak");` 。如果没有万能转发，没有万能引用，那么会执行以下内容：一次 `std::string` 构造函数（以创建临时对象），一次 `std::string` 的移动赋值运算符（以移动 `newName` 到 `w.name` ），还有一次 `std::string` 的析构函数（以销毁临时对象）。

但是如果使用万能引用呢？ `void setName(T&& newName) { name = std::foward<T>(newName); }` 将会把字面值 `"Adela Novak"` 移入 `w.name` ，只会仅调用一次 `std::string` 赋值运算符，不会创建一个临时对象。

#### 返回值优化 return value optimization, RVO

```cpp
Widget makeWidget()
{
    Widget w;  // 局部变量
    return w;  // 不要写成 std::move(w) ！
}
```

明明是局部变量，为什么不需要写成 `std::move(w)` 呢？移动岂不是比复制对象效率更高？

实际上，标准化委员已经想到了， `makeWidget` 的“复制”版本可以通过直接在为函数返回值分配的内存上创建局部变量 `w` 来避免复制之，这就是“返回值优化” return value optimization, RVO 。 **这是 C++ 标准一问世就有的。**

RVO 要满足两个条件：
- 局部对象型别和函数返回值型别相同
- 返回的就是局部对象本身

因此在可以 RVO 时不要使用 `std::move` ，这反而可能限制了编译器的优化项。

### 26 | 避免依万能引用型别进行重载

#### 万能引用与完美转发提高效率的例子

如下是一个正确但是低效的实现。

```cpp
std::multiset<std::string> names;

void logAndAdd(const std::string& name)
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(name);
}

std::string petName("Darla");
logAndAdd(petName);  // 传递左值 std::string
logAndAdd(std::string("Persephone"));  // 传递右值 std::string
logAndAdd("Patty Dog");  // 传递字符串字面量
```

为什么说上面的实现是低效率的呢？
- 第一个调用中传入左值，将在 `emplace` 中复制一份进入集合
- 第二个调用中传入右值，但是 `name` 自身是个左值，因此本可以通过移动进入集合，但是最后还是复制进去了
- 第三个调用中传入字符字面量，按理说直接在 `emplace` 中构造就行，都不用移动，但是因为形参要求传入左值，所以甚至得构造一个临时变量

所以高效版本如下。

```cpp
templace<typename T>
void logAndAdd(T&& name)
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}
```

这样效率就达到极致了。

#### 不要重载万能引用（万能引用匹配的贪婪性，优先匹配非模板函数）

还是上面的例子，如果我们重载 `logAndAdd` 的 `int` 参数类型如下。

```cpp
std::string nameFromIdx(int idx);

void logAndAdd(int idx)
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(nameFromIdx(idx));
}

logAndAdd(22);  // 没问题

short nameIdx;
logAndAdd(nameIdx);  // 编译错误
```

如上，我们有两个函数 `void logAndAdd(T&& name)` 和 `void logAndAdd(int idx)` ，但是 `logAndAdd(int idx)` 只能匹配 `int` 参数，如果是 `short` 则会去匹配 `void logAndAdd(T&& name)` ，这就是万能引用的贪婪性。而 `names.emplace(int)` 会报错，因为没有接收 `int` 的 `string` 构造函数。

另外，对于常量性、编译器自己生产的特种函数也是同理。

```cpp
class Person {
public:
    template<typename T>
    explicit Person(T&& n)
    : name(std::forward<T>(n)) {}  // 完美转发构造函数

    explicit Person(int idx);

    Person(const Person& rhs);  // 编译器生成的复制构造函数
    Person(Person&& rhs);  // 编译器生成的移动构造函数
}

Person p("Nancy");
auto cloneOf(p);  // 注意，这里调用的是完美转发函数，编译无法通过！
```

注意， `auto cloneOf(p);` 中的 `p` 因为并没有 `const` ，所以调用的是 `explicit Person(T&& n)` ，而 `name` 无法把 `Person` 对象作为构造函数参数，因此编译报错。再详细一点，如果有 `auto cloneOf(p);` 这句话，将导致编译器生成如下代码。

```cpp
class Person {
public:
    explicit Person(Person& n)  // 完美转发生成的模板实例化代码
    : name(std::forward<Person&>(n)) {}

    explicit Person(int idx);

    Person(const Person& rhs);  // 编译器生成的复制构造函数
    Person(Person&& rhs);  // 编译器生成的移动构造函数
}
```

那么，把常量性加上变为 `const Person cp("Nancy");` 就，调用的就是复制构造函数了。

```cpp
// 编译器生成的代码如下
class Person {
public:
    explicit Person(const Person& n);  // 模板实例化的构造函数
    Person(const Person& rhs);  // 编译器生成的复制构造函数
}
```

最终， `auto cloneOf(cp);` 将调用复制构造函数，而非模板实例化的函数。这是因为 C++ 的重载决议规则： **一个模板实例化函数和一个非函数模板（一个“常规”函数），具备相等的匹配程度，则优先选用常规函数。**
