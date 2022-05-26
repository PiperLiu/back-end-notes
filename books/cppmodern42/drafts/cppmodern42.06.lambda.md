# 6 | lambda 表达式

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [31 | 避免默认的捕获模式](#31-避免默认的捕获模式)
  - [善用引用捕获与值捕获防止空悬](#善用引用捕获与值捕获防止空悬)
  - [捕获只能针对作用域内可见的非静态局部变量（包括形参），成员变量则要考虑函数 this](#捕获只能针对作用域内可见的非静态局部变量包括形参成员变量则要考虑函数-this)
- [32 | 使用初始化捕获将对象移入闭包](#32-使用初始化捕获将对象移入闭包)
  - [C++ 11 中通过重载类括号运算符，“将对象移入闭包”](#c-11-中通过重载类括号运算符将对象移入闭包)
  - [C++ 11 中通过 std::bind ，“将对象移入闭包”](#c-11-中通过-stdbind-将对象移入闭包)
- [33 | 对 auto&& 型别的形参使用 decltype ，以 std::forward 之](#33-对-auto-型别的形参使用-decltype-以-stdforward-之)
  - [编译器眼中的闭包类](#编译器眼中的闭包类)
- [34 | 优先选用 lambda 式，而非 std::bind](#34-优先选用-lambda-式而非-stdbind)
  - [一些 lambda 有 bind 实例对比](#一些-lambda-有-bind-实例对比)
  - [bind 中参数按值存储； bind 常用到大量模板函数辅助 std::ref 等](#bind-中参数按值存储-bind-常用到大量模板函数辅助-stdref-等)
  - [使用 bind 的情况： C++ 11 中移动捕获与多态函数对象](#使用-bind-的情况-c-11-中移动捕获与多态函数对象)

<!-- /code_chunk_output -->

```cpp
std::find_if(container.begin(), container.end(),
             [](int i) { return i % 2 == 0; }  // 这一行就是 lambda 表达式
            );

int x;
auto c1 = [x](int y) { return x + y; };  // c1 是 lambda 产生的闭包副本
auto c2 = c1;  // c2 是 c1 的副本
```

首先要注意：
- **闭包是 lambda 表达式创建的运行期对象，根据不同的捕获模式，闭包会持有数据的副本或引用。**
- **闭包类就是实例化闭包的类。每个 lambda 式都会触发编译器生成一个独一无二的闭包类。而闭包中的语句会变成它的闭包类成员函数的可执行指令。**

注意， lambda 表达式与闭包类存在于编译器，闭包存在于运行期。

### 31 | 避免默认的捕获模式

#### 善用引用捕获与值捕获防止空悬

一旦由 lambda 表达式所创建的闭包越过了该局部变量或形参的生命期，那么闭包内的引用就会空悬。

```cpp
using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer filters;

// cpp 这种没有垃圾回收的语言，与 JavaScript 的闭包完全不同
void addDivisorFilter()
{
    auto calc1 = computeDivisor1();
    auto calc2 = computeDivisor2();

    auto divisor = computeDivisor(calc1, calc2);

    filters.emplace_back(
        [&](int value) { return value % divisor == 0; }  // divisor 可能空悬
    );

    filters.emplace_back(
        [&divisor](int value) { return value % divisor == 0; }  // divisor 可能空悬
    );
}
```

如上，注意无论是否显式捕获 `divisor` ，这个变量的声明周期就在函数中，如上的写法都会造成 `divisor` 空悬。

比较好的方式是采用按值的默认捕获模式。

```cpp
filters.emplace_back(
    [=](int value) { return value % divisor == 0; }  // divisor 不会空悬
);
```

关于引用捕获，在 lambda 不超出作用域内，则是很好的选择如下。

```cpp
template<typename C>
void workWithContainer(const C& container)
{
    auto calc1 = computeDivisor1();
    auto calc2 = computeDivisor2();

    auto divisor = computeDivisor(calc1, calc2);

    using ContElemT = typename C::value_type;

    using std::begin;
    using std::end;

    if (std::all_of(begin(container), end(container),  // 若 C++ 14 可写作 [&](const auto& elem)
                    [&](const ContElemT& elem) { return elem % divisor == 0; })
    ) {
        // ...
    } else {
        // ...
    }
}
```

#### 捕获只能针对作用域内可见的非静态局部变量（包括形参），成员变量则要考虑函数 this

如下是一个错误的（不安全的）例子。

```cpp
class Widget {
public:
    void addFilter() const;
private:
    int divisor;
};

void Widget::addFilter() const
{
    filters.emplace_back(
        [=](int value) { return value % divisor == 0; }  // divisor 可能空悬
    );
}
```

如上，如果 `Widget` 对象析构了，那么 `filters` 中对应的闭包就会 `divisor` 空悬。等等，我传入的只是个 `int` 呀？ `Widget` 对象析构了，关我 `filters` 里的闭包什么事呢？ **实际上我们并没有传入 `int` ，而是 `Widget` 对象的指针。**

```cpp
void Widget::addFilter() const
{
    filters.emplace_back(
        [=](int value) { return value % divisor == 0; }  // divisor 可能空悬
    );
}
// 上面的实现，实际上，在编译器看来，是这样的
void Widget::addFilter() const
{
    auto currentObjectPtr = this;

    filters.emplace_back(
        [currentObjectPtr](int value)  // 我们仅仅捕获了对象指针，而非成员变量副本
        { return value % currentObjectPtr->divisor == 0; }
    );
}
```

因此，我们可以获取我们的成员变量，如下三种实现是对的。

```cpp
void Widget::addFilter() const
{
    auto divisorCopy = divisor;  // 相当于 divisorCopy = this->divisor 复制成员变量

    filters.emplace_back(
        [divisorCopy](int value)  // 捕获副本
        { return value % divisorCopy == 0; }
    );
}

void Widget::addFilter() const
{
    auto divisorCopy = divisor;  // 相当于 divisorCopy = this->divisor

    filters.emplace_back(
        [=](int value)  // 捕获副本
        { return value % divisorCopy == 0; }
    );
}

void Widget::addFilter() const
{
    filters.emplace_back(
        [divisorCopy=divisor](int value)  // 复制入闭包
        { return value % divisorCopy == 0; }
    );
}
```

此外注意， lambda 式可能不仅依赖于局部变量和形参（它们可以被捕获），它们还会依赖于静态存储期 static storage duration 对象。 **这样的对象可以在 lambda 式内部使用但是不能被捕获。**

```cpp
void addDivisorFilter()
{
    static auto calc1 = computeDivisor1();
    static auto calc2 = computeDivisor2();

    static auto divisor = computeDivisor(calc1, calc2);

    filters.emplace_back(
        [=](int value)  // 未捕获到任何东西！
        { return value % divisor == 0; }
    );

    ++ divisor;  // 将影响已经生成的闭包！
}
```

### 32 | 使用初始化捕获将对象移入闭包

C++ 11 不支持将只移对象放入闭包， C++ 14 支持。

#### C++ 11 中通过重载类括号运算符，“将对象移入闭包”

```cpp
// C++ 14
class Widget {
public:
    bool isValidated() const;
    bool isArchived() const;
};

auto pw = std::make_unique<Widget>();
...  // 配置 *pw
auto func = [pw = std::move(pw)]
            { return pw->isValidated() && pw->isArchived(); };

// 如果不配置 *pw ，则无需把 pw 建立在外面
auto func = [pw = std::make_unique<Widget>()]
            { return pw->isValidated() && pw->isArchived(); };
```

C++ 11 无法做到上述功能（因为 `make_unique` 的智能指针是只移型别 ）。 C++ 11 的捕获也不能捕获一个表达式的结果。 C++ 14 中初始化捕获也称为广义 lambda 捕获 generalized lambda capture 。

如何用 C++ 11 实现上述功能呢？写一个类。

```cpp
class IsValAndArch {
public:
    using DataType = std::unique_ptr<Widget>;

    explicit IsValAndArch(DataType&& pw)  // 移动构造函数
    : pw(std::move(pw)) {}

    bool operator()() const {
        return pw->isValidated() && pw->isArchived();
    }
private:
    DataType pw;
};

// 如下，现在 func 也是一个 callable 对象，且传入右值对象
auto func = IsValAndArch(std::make_unique<Widget>());
```

#### C++ 11 中通过 std::bind ，“将对象移入闭包”

```cpp
// C++ 14
std::vector<double> data;

auto func = [data = std::move(data)]
            { ... };
```

上述功能，如何用 C++ 11 实现呢？

```cpp
auto func = std::bind(
    [](const std::vector<double>& data)
    { ... },
    std::move(data)
);
```

`std::bind` 的第一个实参是个可调用对象，接下来的所有实参表示传给该对象的值。绑定对象含有传递给 `std::bind` 所有实参的副本，对于每个左值传参，在绑定对象内的对应的对象内对其实施的是复制构造；而对于每个右值实参，实施的则是移动构造。

此外，注意， lambda 生成的闭包类中的 `operator()` 成员函数会带有 `const` 饰词，如果我们修改某个闭包类对象，则可以加一个 `mutable` 如下。

```cpp
auto func = std::bind(
    [](std::vector<double>& data) mutable
    { ... },
    std::move(data)
);
```

### 33 | 对 auto&& 型别的形参使用 decltype ，以 std::forward 之

#### 编译器眼中的闭包类

```cpp
// 如下这个 lambda 表达式
auto f = [](auto x) { return func(normalize(x)); };
// 编译器编译出来的闭包类如下
class SomeCompilerGeneratedClass {
public:
    template <typename T>
    auto operator()(T x) const
    { return func(normalize(x)); }

    ...
};
```

可见，实际上是生成了模板类。

这就想到了完美转发在这里是个很好的应用。

```cpp
auto f = [](auto&& param)
         {
             return
                func(normalize(std::forward<decltype(param)>(param)));
         }
// 如果是多个参数， C++ 14 中 lambda 也接受可变长形参
auto f = [](auto&&... params)
         {
             return
                func(normalize(std::forward<decltype(params)>(params)...));
         }
```

### 34 | 优先选用 lambda 式，而非 std::bind

我们称 `std::bind` 返回的函数对象为绑定对象。

#### 一些 lambda 有 bind 实例对比

```cpp
using Time = std::chrono::system_clock::time_point;
enum class Sound { Beep, Siren, Whistle };
using Duration = std::chrono::steady_clock::duration;
// 在时刻 t 发出声音 s 持续时长 d
void setAlarm(Time t, Sound s, Duration d);
```

现在我们需要实现一个函数，这个函数接受一个指定的声音，该声音将在设定后一小时发出，持续三十秒。

```cpp
// lambda
auto setSoundL =
    [](Sound s)
    {
        using namespace std::chrono;
        setAlarm(steady_clock::now() + hours(1),
                 s,
                 second(30));
    };

auto setSoundL =
    [](Sound s)
    {
        using namespace std::chrono;
        using namespace std::literals;  // C++ 14 后缀
        setAlarm(steady_clock::now() + 1h,
                 s,
                 30s);
    };
```

上面两个 lambda 表达式都没问题，但是像如下的 bind 实现，则功能上产生了问题。

```cpp
using namespace std::chrono;
using namespace std::literals;

using namespace std::placeholders;  // 是因为要使用 _1
auto setSoundB = 
    std::bind(setAlarm,
              steady_clock::now() + 1h,
              _1,  // 表示接收 bind 对象传入的第一个参数
              30s);
```

上面的 `_1` 表示 `setSoundB(x)` 中的 `x` 。

**注意！上面的功能产生问题，在我们调用 `bind` 时， `steady_clock::now()` 就已经被调用，而不是 `setSoundB` 调用时取当前时间。**

因此，正确的实现如下。

```cpp
// C++ 11
auto setSoundB = 
    std::bind(setAlarm,
              std::bind(std::plus<steady_clock::time_point>(),
                        steady_clock::now(),
                        hours(1)),
              _1,  // 表示接收 bind 对象传入的第一个参数
              sound(30));

// C++ 14
auto setSoundB = 
    std::bind(setAlarm,
              std::bind(std::plus<>(),
                        steady_clock::now(),
                        1h),
              _1,  // 表示接收 bind 对象传入的第一个参数
              30s);
```

如果我们重载 `setAlarm` ，则上述版本的 `bind` 实现也不对。

```cpp
// 另一个 setAlarm 接收四个参数
void setAlarm(Time t, Sound s, Duration d, Volume v);
```

我们的 `bind` 并不知道该绑定哪个 `setAlarm` （而 lambda 中则是正常调用函数，可以自动匹配到需要的函数），因此应该如下实现。

```cpp
using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);

auto setSoundB = 
    std::bind(static_cast<SetAlarm3ParamType>(setAlarm),
              std::bind(std::plus<>(),
                        steady_clock::now(),
                        1h),
              _1,
              30s);
```

最后一点是， lambda 中闭包类调用 `setAlarm` 可被编译器应用到内联，会提升效率，而 `bind` 不会。

#### bind 中参数按值存储； bind 常用到大量模板函数辅助 std::ref 等

上面我们讨论过，不能在 `bind` 内直接调用函数，因为这很可能会造成逻辑上的错误。下面的例子也阐述了 `bind` 有多么麻烦。

```cpp
// C++ 11
auto betweenL =
    [lowVal, highVal]
    (int val)
    { return lowVal <= val && val <= highVal; };

// C++ 14
auto betweenL =
    [lowVal, highVal]
    (const auto& val)
    { return lowVal <= val && val <= highVal; };

// bind 实现
using namespace std::placeholders;

// C++ 11
auto betweenB =
    set::bind(std::logical_and<bool>(),
                std::bind(std::less_equal<int>(), lowVal, _1),
                std::bind(std::less_equal<int>(), _1, highVal));

// C++ 14
auto betweenB =
    set::bind(std::logical_and<>(),
                std::bind(std::less_equal<>(), lowVal, _1),
                std::bind(std::less_equal<>(), _1, highVal));
```

此外，注意 `std::bind` 总是复制其实参，如果想要达成引用存储，可以用 `std::ref` ，如下。

```cpp
auto compressRateB = std::bind(compress, std::ref(w), _1);
```

#### 使用 bind 的情况： C++ 11 中移动捕获与多态函数对象

C++ 11 移动捕获使用 `bind` 的情况之前的条款讨论过。

C++ 11 无法给 lambda 传入多态函数对象，因此可以用 `bind` 实现如下。

```cpp
class PolyWidget {
public:
    template<typename T>
    void operator()(const T& param);
};

PolyWidget pw;
auto boundPW = std::bind(pw, _1);

boundPW(1930);
boundPW(nullptr);
boundPW("Rosebud");
```

但是如果 C++ 14 ，则根本不需要 `bind` ，可以如下书写 lambda 。

```cpp
// C++ 14
auto boundPW = [pw](const auto& param)
               { pw(param); };
```
