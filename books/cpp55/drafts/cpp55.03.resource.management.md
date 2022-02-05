# 3 | 资源管理

Resource Management

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [13 | 以对象管理资源](#13-以对象管理资源)
  - [智能指针 std::auto_ptr](#智能指针-stdauto_ptr)
  - [RCSP 的 shared_ptr 解决 auto_ptr 唯一所有权问题](#rcsp-的-shared_ptr-解决-auto_ptr-唯一所有权问题)
- [14 | 在资源管理类中小心 copying 行为](#14-在资源管理类中小心-copying-行为)
  - [资源取得时机便是初始化时机](#资源取得时机便是初始化时机)
  - [mutex 为例： RAII 复制行为](#mutex-为例-raii-复制行为)
- [15 | 在资源管理类中提供对原始资源的访问](#15-在资源管理类中提供对原始资源的访问)
  - [智能指针重载了指针取值操作符](#智能指针重载了指针取值操作符)
  - [重载隐式转换函数](#重载隐式转换函数)
- [16 | 成对使用 new 和 delete 时要采取相同形式](#16-成对使用-new-和-delete-时要采取相同形式)
- [17 | 以独立语句将 newed 对象置入智能指针](#17-以独立语句将-newed-对象置入智能指针)

<!-- /code_chunk_output -->

### 13 | 以对象管理资源

Use objects to management resources.

#### 智能指针 std::auto_ptr

```cpp
Investment* createInvestment();

void f1()
{
  Investment* pInv = createInvestment();
  ...
  delete pInv;  // 有可能忘记或者运行不到这里
}

void f2()
{
  std::auto_ptr<Investment> pInv(createInvestment());
  ...
  // 自动地调用 Investment 析构
}
```

#### RCSP 的 shared_ptr 解决 auto_ptr 唯一所有权问题

```cpp
std::auto_ptr<Investment> pInv1(createInvestment());

std::auto_ptr<Investment> pInv2(pInv1);
// 此时 Pinv2 被指向对象，而 Pinv1 是 null

pInv1 = pInv2;  // 此时 pInv2 是 null
```

如果希望有多个智能指针指向同一对象，则替代方案是“引用技术型智慧指针（reference-counting smart pointer; RCSP）”。在 C++ 里使用 tr1::shared_ptr 。

```cpp
// 书中， shared_ptr 还属于 std::tr1::
std::shared_ptr<Investment> pInv1(createInvestment());

std::shared_ptr<Investment> pInv2(pInv1);  // 都指向对象

pInv1 = pInv2;
```

### 14 | 在资源管理类中小心 copying 行为

Think carefully about copying behavior in resource-management classes.

#### 资源取得时机便是初始化时机

资源取得时机便是初始化时机， Resource Acquisition Is Initialization, RAII 。

这个观念被智能指针表现在 heap-based 资源上。

#### mutex 为例： RAII 复制行为

RAII 对象被复制，大部分情况下希望有如下两种事发生：
- 禁止复制
- 对底层资源使用“引用计数法（reference-count）”

这里以 mutex 为例。

```cpp
class Lock {
public:
  explicit Lock(mutex* pm) : mutexPtr(pm)
  { lock(mutexPtr); }
  ~Lock() { unlock(mutexPtr); }
private:
  mutex *mutexPtr;
};

Mutex m;
{
  Lock m1(&m);
  ... // 最末尾自动解除互斥量锁定
}
```

如上，如果发生以下赋值行为，该怎么办？

```cpp
Lock ml1(&m);
Lock ml2(ml1);  // 该怎么办？
```

为了防止不确定行为，我们重写 Lock 类，这里使用基于 shared_ptr 的 “引用计数法（reference-count）”：

```cpp
class Lock {
public:
  explicit Lock(mutex* pm)
   : mutexPtr(pm, unlock)
  { lock(mutexPtr.get()); }
private:
  std::shared_ptr<mutex> mutexPtr;
};
```

如上：
- 给 shared_ptr 传入一个 unlock 函数作为删除器
- 不需要声明析构函数，使用编译器默认的，这里 mutexPtr 的析构函数会在互斥量的引用次数为 0 时自动调用 shared_ptr 的删除器（即 unlock）

### 15 | 在资源管理类中提供对原始资源的访问

Provide access to raw resources in resource-managing classes.

#### 智能指针重载了指针取值操作符

智能指针重载了指针取值（pointer dereferencing）操作符（`->`和`*`）。

因此，不需要 `(ptr.get()) ->` 或者 `*(ptr.get())` 这样了，直接 `ptr->` 或者 `*ptr` 就好。

#### 重载隐式转换函数

```cpp
class Font {
  operator FontHandle() const
  { return f; }
}
```

这样，如果调用 `changeFontSize(FontHandle f, int newSize)` ，则可以直接输入 `Font` 对象。如下：

```cpp
Font f(getFont());

changeFontSize(f, newFontSize);  // f 将会隐式转换为 FontHandle
```

但注意，这样可能会出现错误。

```cpp
Font f1(getFont());
FontHandle f2 = f1;  // 本来想拷贝一个 Font 对象，结果却被转换为了 FontHandle 才复制
```

### 16 | 成对使用 new 和 delete 时要采取相同形式

Use the same form in corresponding uses of new and delete.

```cpp
string* stringArray = new string[100];
...
delete stringArray;
```

这会导致程序出现不明确行为，可能有 99 string 没有被适当删除（析构函数可能没被调用）。

new 会发生什么？
- 内存被分配出来
- 一个或更多构造函数出现

```cpp
string* stringPtr1 = new string[100];
string* stringPtr2 = new string;
...
delete [ ] stringPtr1;
delete stringPtr2;
```

### 17 | 以独立语句将 newed 对象置入智能指针

Store newed objects in smart pointers in standalone statements.

```cpp
int priority();
void processWidget(std::shared_ptr<Widget> pw, int prioriy);

processWidget(new Widget, priority());
```

如上会编译错误，因为 shared_ptr 构造函数需要一个原始指针，但是该其构造函数是个 explicit 构造函数，不允许进行隐式转换，因此要写成如下形式：

```cpp
processWidget(
  std::shared_ptr<Widget>(new Widget)
  , priority());
```

如上可行，但是会出现问题。

考虑调用 processWidget 之前，编译器必须创建代码，做以下三件事：
- 调用 priority
- 执行 "new Widget"
- 调用 std::shared_ptr 构造函数

C++ 并不是 java 或者 C# ，参数中任务的执行顺序并不确定，有可能有这样的序列：
- 执行 "new Widget"
- 调用 priority
- 调用 std::shared_ptr 构造函数

如果 priority 调用异常， "new Widget" 返回的指针可能会遗失，导致 share_ptr 中并不会被置入该指针，导致资源泄漏。

因此还是用以下方式为好：

```cpp
std::shared_ptr<Widget> pw(new Widget);
processWidget(pw, priority());
```
