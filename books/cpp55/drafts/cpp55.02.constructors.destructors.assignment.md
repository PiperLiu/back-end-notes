# 2 | 构造/析构/赋值运算

Constructors, Destructors, and Assignment Operators

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [05 | 了解 C++ 默默编写并调用哪些函数](#05-了解-c-默默编写并调用哪些函数)
  - [C++ 默认实现了构造函数等内容](#c-默认实现了构造函数等内容)
  - [更好地理解 static](#更好地理解-static)
- [06 | 若不想使用编译器自动生成的函数，就该明确拒绝](#06-若不想使用编译器自动生成的函数就该明确拒绝)
  - [将 copy 构造函数或操作符设为 private 以防止拷贝对象](#将-copy-构造函数或操作符设为-private-以防止拷贝对象)
  - [函数声明不写参数名称是可以的](#函数声明不写参数名称是可以的)
- [07 | 为多态基类声明 virtual 析构函数](#07-为多态基类声明-virtual-析构函数)
  - [基类的析构函数 non-virtual 带来灾害](#基类的析构函数-non-virtual-带来灾害)
  - [别无端声明虚函数](#别无端声明虚函数)
  - [不要继承标准容器或者其他带有 non-virtual 析构函数的类！](#不要继承标准容器或者其他带有-non-virtual-析构函数的类)
  - [纯虚函数 pure virtual 导致抽象类 abstract class](#纯虚函数-pure-virtual-导致抽象类-abstract-class)
- [08 | 别让异常逃离析构函数](#08-别让异常逃离析构函数)
  - [把异常反应留给使用者，而非析构函数](#把异常反应留给使用者而非析构函数)
- [09 | 绝不在构造和析构过程中调用 virtual 函数](#09-绝不在构造和析构过程中调用-virtual-函数)
  - [在基类构造期间，虚函数不是虚函数](#在基类构造期间虚函数不是虚函数)
- [10 | 令 operator= 返回一个 reference to *this](#10-令-operator-返回一个-reference-to-this)
  - [不仅适用于=，适用于所有赋值相关运算，是一个协议，合理的协议](#不仅适用于适用于所有赋值相关运算是一个协议合理的协议)
- [11 | 在 operator= 中处理“自我赋值”](#11-在-operator-中处理自我赋值)
  - [自我赋值可能带来的错误](#自我赋值可能带来的错误)
  - [证同测试 identity test](#证同测试-identity-test)
  - [调整语句顺序保证异常安全性](#调整语句顺序保证异常安全性)
  - [copy-and-swap](#copy-and-swap)
- [12 | 复制对象时勿忘其每一个成份](#12-复制对象时勿忘其每一个成份)
  - [Copying 函数应该确保复制所有成份](#copying-函数应该确保复制所有成份)
  - [不要用 copying 函数实现另一个 copying 函数](#不要用-copying-函数实现另一个-copying-函数)

<!-- /code_chunk_output -->

### 05 | 了解 C++ 默默编写并调用哪些函数

Know what functions C++ silently writes and calls.

#### C++ 默认实现了构造函数等内容

```cpp
class Empty { };

// 写上面一行代码，相当于写下下面的代码

class Empty {
public:
  Empty() { ... }
  Empty(const Empty& rhs) { ... }
  ~Empty() { ... }

  Empty& operator=(const Empty& rhs) { ... }
};
```

当然，如果专门声明了构造函数等，那编译器当然不会实现默认版

#### 更好地理解 static

编译器默认实现的函数都干了什么呢？构造函数和析构函数不讲，主要是让程序能跑得通； copy 构造函数和 copy assignment 操作符，编译器创建的版本只是单纯地将来源对象地每一个 non-static 成员变量拷贝到目标对象。

### 06 | 若不想使用编译器自动生成的函数，就该明确拒绝

Explicitly disallow the use of compiler-generated functions you do not want.

#### 将 copy 构造函数或操作符设为 private 以防止拷贝对象

将 copy 构造函数或 copy assignment 操作符声明为 private ，并且故意不实现。

这个技巧在 C++ iostream 中被使用。

```cpp
class HomeForSale {
public:
  ...
private:
  ...
  HomeForSale(const HomeForSale);  // 只有声明
  HomeForSale& operator=(const HomeForSale);
};
```

如果在程序书写时调用了 HomeForSale 对象的拷贝逻辑，会被编译器阻挠。

但是如果你在成员函数 member function 或者友元函数 friend function 中不慎调用，则到连接期才会发现错误。**如何把连接期错误移到编译期呢？** 做一个基类来决定这个性质。

```cpp
class Uncopyable {
protected:
  Uncopyable() { }  // 运行派生类实现
  ~Uncopyable() { }
private:
  Uncopyable(const Uncopyable&);  // 只有声明，私有，阻止派生类实现
  Uncopyable& operator=(const Uncopyable&);
};

// 继承这个 Uncopyable 即可
class HomeForSale: private Uncopyable {
  ...
};
```

#### 函数声明不写参数名称是可以的

如上例子中，函数声明不写参数名称是可以的。

### 07 | 为多态基类声明 virtual 析构函数

Declare destructors virtual in polymorphic base classes.

#### 基类的析构函数 non-virtual 带来灾害

```cpp
class TimeKeeper {
public:
  TimeKeeper();
  ~TimeKeeper();
  ...
};

class AtomicClock: public TimeKeeper { ... };
...

//  getTimeKeeper 将返回一个 AtomicClock
TimeKeeper* ptk = getTimeKeeper();
...
delete ptk;
```

上面的操作很危险！将造成 **局部销毁** ！因为 ptk 是 `TimeKeeper` 的指针，调用的析构函数是 `TimerKeeper` 的，这将导致属于 `AtomicClock` 的部分没有被销毁！

解决方法是给基类一个 virtual 析构函数。

```cpp
class TimeKeeper {
public:
  TimeKeeper();
  virtual ~TimeKeeper();
  ...
};

class AtomicClock: public TimeKeeper { ... };
...

//  getTimeKeeper 将返回一个 AtomicClock
TimeKeeper* ptk = getTimeKeeper();
...
delete ptk;  // 可以
```

#### 别无端声明虚函数

```cpp
class Point {
public:
  Point(int xCoord, int yCoord);
  ~Point():
private:
  int x, y;
};
```

如上，没有虚函数，我们的 Point 对象可以塞入一个 64-bit 的缓存器中，因为只有两个 int 。但是虚函数的对象必须携带一些信息，比如 vptr （virtual table pointer）；因此如果声明为虚函数，则无法再塞入一个 64-bit 的缓存器了。

#### 不要继承标准容器或者其他带有 non-virtual 析构函数的类！

理由同前，这会导致局部销毁。

#### 纯虚函数 pure virtual 导致抽象类 abstract class

```cpp
class AWOV {
public:
  virtual ~AWOV() = 0;  // 纯虚函数声明方法
};

// 记得一定要为纯虚析构函数提供一份定义
AWOV::~AWOV() { }
```

这样这个 AWOV 就不能被实例化。

为什么要记得一定要为纯虚析构函数提供一份定义？因为析构函数的运作方式是：最深层的派生类析构函数被调用，然后每一个基类层层被调用。

### 08 | 别让异常逃离析构函数

Prevent exception from leaving destruction.

#### 把异常反应留给使用者，而非析构函数

关于数据库连接的错误示例：

```cpp
class DBConnection {
public:
  ...
  static DBConnection create();
  void close();
};

class DBConn {  // 用于管理 DBConnection
public:
  ...
  ~DBConn()
  {
    db.close();
  }
private:
  DBConnection db;
};

// 于是使用者会写出如下代码
{
  DBConn dbc(DBConnection::create());

  // 在 block 结束时析构 DBConn
}
```

问题在于，在析构中调用 close ，可能导致执行提前结束或者多个异常（多个数据库连接都 close 异常），这将导致很多不明确的行为产生，非常难处理。

如何正确解决？在 DBConn 中将 DBConnection 的 close 暴露出来，交给使用者；同时自己在析构函数中加入“保险”。

```cpp
class DBConn {
public:
  ...
  void close()  // 使用者可以自己调用 close ，也可以自己设置捕获其中异常
  {
    db.close();
    closed = true;
  }
  ~DBConn()
  {
    if (!closed) {  // 加入保险
      try {
        db.close();
      } catch (...) {
        制作运转记录，记下对 close 的调用失败
        ...
      }
    }
  }
private:
  DBConnection db;
  bool closed;
};
```

### 09 | 绝不在构造和析构过程中调用 virtual 函数

Never call virtual functions during construction or destruction.

#### 在基类构造期间，虚函数不是虚函数

```cpp
class Transaction {
public:
  Transaction();
  /**
   * 如下显然是一个纯虚函数
   * 是希望，子类在继承时，要实现一份自己的日志记录方式
   */
  virtual void logTransaction() const = 0;
};

Transaction::Transaction()
{
  ...
  logTransaction();  // 希望在构造交易时留下记录（但这不对！）
}

class BuyTransaction: public Transaction {
public:
  virtual void logTransaction() const;
  ...
}
```

如上，逻辑上其实很好理解，我的基类如此实现，是为了保证每次构造时，都调用“日志记录”。

但是这会导致错误！

```cpp
BuyTransaction b;
```

如上，我们构造一个子类，首先会调用基类的构造函数，即 Transaction() ，此时调用 logTransaction() ，其是 Transaction 的版本而非 BuyTransaction 的版本。

总结一下： **在构造和析构期间不要调用 virtual 函数，因为这类调用不下降至 derived class 。**

那上面的逻辑怎么才是对的呢？

```cpp
class Transaction {
public:
  explicit Transaction(const std::strng& logInfo);
  void logTransaction(const std::string& logInfo) const;  // non-virtual
};

Transaction::Transaction(const std::strng& logInfo)
{
  ...
  logTransaction();  // 希望在构造交易时留下记录，是个 non-virtual 调用
}

class BuyTransaction: public Transaction {
public:
  BuyTransaction( parameters )
    : Transaction(createLogString( paramters ))
  { ... }
private:
  static std::string createLogString( parameters );
};
```

如上提供了一种参考。

### 10 | 令 operator= 返回一个 reference to *this

Have assignment operators return a reference to *this.

#### 不仅适用于=，适用于所有赋值相关运算，是一个协议，合理的协议

```cpp
class Widget {
public:
  ...
  Widget& operator=(const Widget& rhs)
  {
    ...
    return* this;
  }
  Widget& operator+=(const Widget& rhs)
  {
    ...
    return *this;
  }
  Widget& operator=(int rhs)
  {
    ...
    return *this;
  }
};
```

很好理解，比如 `a += b` ，实际上就是把 `b` 里面的东西与 `a` 里面的东西做一些运算，放到 `a` （也就是 `*this`）身上，之后再把这个值体现为表达式 `a += b` 的返回值。

这是将让一些传递关系成立：

```cpp
int x, y, z;
x = y = z = 15;
// 上述生效因为实际上就是
x = (y = (z = 15));
```

### 11 | 在 operator= 中处理“自我赋值”

Handle assignment to self in operator=.

#### 自我赋值可能带来的错误

首先自我赋值是可能不经意间发生的，比如 `a[i] = a[j]` 。

```cpp
class Bitmap { ... };
class Widget {
  ...
private:
  Bitmap* pb;  // 指向一个 heap 中的对象
};

Widget&
Widget::operator=(const Widget& rhs)
{
  delete pb;  // 可能是 rhs.pb ！ 所以不能直接 delete
  pb = new Bitmap(*rhs.pb);
  return *this;
}
```

#### 证同测试 identity test

```cpp
Widget&
Widget::operator=(const Widget& rhs)
{
  if (this == &rhs) return *this;

  delete pb;
  pb = new Bitmap(*rhs.pb);
  return *this;
}
```

这是一个直观的思路，但可能多产生一个分支预测，降低效率。

#### 调整语句顺序保证异常安全性

```cpp
Widget&
Widget::operator=(const Widget& rhs)
{
  Bitmap* pOrig = pb;  // 记住原先的 pb
  pb = new Bitmap(*rhs.pb);  // 令 pb 指向 *rhs.pb （也可能是 *pb 自己）的一个副本
  delete pOring;  // 删除原来的 pb
  return *this;
}
```

#### copy-and-swap

```cpp
class Widget {
  ...
  void swap(Widget& rhs);
  ...
};

Widget&
Widget::operator=(const Widget& rhs)
{
  Widget temp(rhs);
  swap(temp);
  return *this;  // 这里 block 结束后，会把 temp 也就是原来的 *this 销毁
}
```

而且，当我们传 value 则更加简洁：

```cpp
Widget&
Widget::operator=(Widget rhs)
{
  swap(rhs);
  return *this;
}
```

### 12 | 复制对象时勿忘其每一个成份

Copy all parts of an object.

#### Copying 函数应该确保复制所有成份

这里不举例子了，很直观。

比如构造函数，如果你写了子类的构造函数，那可别忘了调用父类的构造函数。

#### 不要用 copying 函数实现另一个 copying 函数

比如不要用拷贝构造函数实现 `=` 操作符。因为我们的拷贝构造函数是实现一个新对象，而赋值符号则是假设我们构造一个已存在的对象。

如果有重复的逻辑，放在第三个函数中，在 copying 函数中共同调用。
