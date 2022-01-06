# 1 | 让自己习惯 C++

Accustoming Yourself to C++

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [导读部分](#导读部分)
  - [定义一定调用构造函数](#定义一定调用构造函数)
  - [命名习惯 Naming Conventions](#命名习惯-naming-conventions)
- [01 | 视 C++ 为一个语言联邦](#01-视-c-为一个语言联邦)
- [02 | 尽量以 const, enum, inline 替换 #define](#02-尽量以-const-enum-inline-替换-define)
  - [声明与定义的区别](#声明与定义的区别)
  - [class 专属常量被特殊处理](#class-专属常量被特殊处理)
  - [滥用 #define 的坏处](#滥用-define-的坏处)
- [03 | 尽可能使用 const](#03-尽可能使用-const)
  - [const 与指针](#const-与指针)
  - [const 在声明中的作用](#const-在声明中的作用)
  - [const 成员函数](#const-成员函数)
  - [mutable 之于 const 成员函数](#mutable-之于-const-成员函数)
  - [在 const 和 non-const 中避免重复](#在-const-和-non-const-中避免重复)
- [04 | 确定对象被使用前已先被初始化](#04-确定对象被使用前已先被初始化)
  - [成员初值列 member initialization list](#成员初值列-member-initialization-list)
  - [关于 static 以及 local static](#关于-static-以及-local-static)

<!-- /code_chunk_output -->

### 导读部分

#### 定义一定调用构造函数

```cpp
class Widget {
public:
  Widget();                             // default 构造函数
  Widget(const Widget& rhs);            // copy 构造函数
  Widget& operator=(const Widget& rhs); // copy assignment
  ...
};

Widget w1;      // 调用 default 构造函数
Widget w2(w1);  // 调用 copy 构造函数
w1 = w2;        // 调用 copy assignment
Widget w3 = w2; // 调用 copy 构造函数！！
```

注意，如果一个新的对象被 **定义** （如 `w3`），则一定会有一个构造函数被调用；反正，如果没有新对象被定义，就不会有构造函数被调用。

#### 命名习惯 Naming Conventions

常见的：
- `lhs` left-hand side 左手边
- `rhs` right-hand side 右手边

### 01 | 视 C++ 为一个语言联邦

View C++ as a federation of languages.

C++ 是多重范型编程语言（`multiparadigm programming language`）。

主要的次语言有四个：
- C
- Object-Oriented C++
- Template C++
- STL

这些内容太哲学了，目前没必要纠结。

### 02 | 尽量以 const, enum, inline 替换 #define

Prefer `consts, enums, and inlines` to `#defines`.

#### 声明与定义的区别

参考：
- [C++ 声明与定义 - 风之飒的文章 - 知乎](https://zhuanlan.zhihu.com/p/74528787)
- [C++中“声明”和“定义”是不是有一些重复了？如果不重复，那么为什么需要把“声明”和“定义”分开？ - oobleck的回答 - 知乎](https://www.zhihu.com/question/19809432/answer/13029289)

- 声明：是指出存储类型，并给存储单元指定名称（告诉 **编译器** 这个名字是什么类型，是函数，还是类 还是其他）。
- 定义：是分配内存空间，还可为变量指定初始值（定义是为这个名字分配存储空间）。
- `extern` 关键字：通过使用 `extern` 关键字声明变量名，而不是定义它。

```cpp
// extern 告诉编译器变量在其他地方定义了，有分配内存空间，所以不是定义
extern int val; // 只是声明了变量 val ，并没有分配内存空间，所以不是定义

// 定义变量的时候，同时给变量指出了存储类型，并给变量的存储单元指定了名称
// 所以定义也是声明
int val; // 声明了变量 val ，也会分配内存空间，所以也是定义

// extern 关键字标识的才是声明
extern int val; // 声明
int val; // 定义
```

如上，注意：
- 声明不一定是定义
- 定义也是声明
- 一个变量的定义永远只能有一个，但是可以有多个声明

**以下是关于函数的内容：**

函数原型（又称函数声明）：函数原型之于函数，相当于变量声明之于变量，只有函数头的就是函数原型。

函数定义：带有函数体的就是定义。

```cpp
int function();  // 声明
int function() { return 0; }  // 定义
```

注意：

```cpp
// C语言中
int func(); // 表示可以有很多个参数
int func(void); // 表示没有参数
// C++中，上述两行代码等价，且都表示没有参数。
```

在函数被调用之前必须先有函数原型，函数定义可以放在调用函数的后面。

#### class 专属常量被特殊处理

如果想在 class 中用常量替换 `#define` ：

```cpp
class GamePlayer {
private:
  static const int NumTurns = 5;  // 常量声明式
  int scores[NumTurns];           // 使用该常量
}
```

如上：
- 为了确保此常量至多只有一份实体，使用 `static`
- 注意这里是声明而非定义，这是 C++ 中声明式的特殊情况：是 `class` 专属常量且 `static` 且是整数类型 `ints, chars, bools`
- 注意这种专属常量不应被取地址，如果需要取地址，则应该在一个`头文件`中提供`定义式`：`const int GamePlayer::NumTurns;`

此外，对于纯常量，用 `enum` 也可取代 `define` ，其与 `define` 一样，也不可获取地址。

```cpp
class GamePlayer {
private:
  enum { NumTurns = 5 };

  int scores[NumTurns];
}
```

#### 滥用 #define 的坏处

观察下面的例子：
```cpp
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

int main()
{
  int a = 5, b = 0;
  CALL_WITH_MAX(++ a, b);      // a 累加 2 次
  CALL_WITH_MAX(++ a, b + 10); // a 累加 1 次
}
```

如上，你根本不知道 `a` 会被累加几次（无法控制）。

**这样导致行为不可预料。**

可以用 `inline` 来获取 `define` 带来的效率，同时让行为可以预料：

```cpp
template<typename T>
inline void callWithMax(const T& a, const T& b)
{
  f(a > b ? a : b);
}
```

### 03 | 尽可能使用 const

Use `const` whenever possible.

注意 `const` 无关乎性能。

#### const 与指针

```cpp
char greeting[] = "Hello";
char *p = greeting;             // non-const pointer, non-const data
const char* p = greeting;       // non-const pointer, cosnt data
char* const p = greeting;       // const pointer, not-const data
const char* const p = greeting; // const pointer, const data
```

如果关键字 `const` 出现在星号左边，表示被指物是常量，如果出现在星号右边，表示指针自身是常量。

以 STL 迭代器为例：

```cpp
std::vector<int> vec;

...

// iter 的作用像个 T* const
const std::vector<int>::iterator iter = vec.begin();
*iter = 10;  // 没问题，改变 iter 所指物
++ iter;     // 错误！ iter 是 const

// cIter 的作用像个 const T*
std::vector<int>::const_iterator cIter = vec.begin();
*cIter = 10; // 错位！ *cIter 是 const
++ cIter;    // 没问题，改变 cIter
```

#### const 在声明中的作用

```cpp
class Rational { ... };
const Rational operator* (const Rational& lhs, const Rational& rhs);

Rational a, b, c;
(a *b) = c; // 这是不允许的，因为 operator* 返回 const
```

也能防止如下笔误：
```cpp
// 本来想写
if (a * b == c) { ... }
// 如下会报错
if (a * b = c) { ... }
```

#### const 成员函数

**两个成员函数如果只是常量性（`constness`）不同，可以被重载。**

```cpp
class TextBlock {
public:
  ...
  /** const char& operator[](std::size_t position) const
   * 前面这个 const 让返回值是 const
   * 后面这个 const 用于重载！
   * 表示成员变量不可变， this 指针不可被修改
   */
  // operator[] for const 对象
  const char & operator[](std::size_t position) const
  { return text[position]; }
  // operator[] for non-const 对象
  char& operator[](std::size_t position)
  { return text[position]; }
private:
  std::string text;
};

// 下面调用两个不同的 operator[]
TextBlock tb("Hello");  // 调用 non-const
std::cout << tb[0];
const TextBlock ctb("World");  // 调用 const
std::cout << ctb[0];

void print(const TextBlock& ctb)
{ // 调用 operator[] for const 对象
  std::cout << ctb[0];
  ...
}

std::cout << tb[0];
tb[0] = 'x';
std::cout << ctb[0];
// 如下报错，因为 operator[] for const 对象 返回一个 const char
ctb[0] = 'x';
```

#### mutable 之于 const 成员函数

```cpp
class CTextBlock {
public:
  ...
  std::size_t length() const;
private:
  char* pText;
  std::size_t textLength;
  bool lengthIsValid;
};

std::size_t CTextBlock::length() const
{
  /**
   * const 成员函数内不能给他 this.xxx 赋值
  */
  if (!lengthIsValid) {
    textLength = std::strlen(pText);  // 错
    lengthIsValid = true;             // 错
  }
  return textLength;
}
```

如何改？

```cpp
class CTextBlock {
public:
  ...
  std::size_t length() const;
private:
  char* pText;
  /**
   * 如下没有问题
  */
  mutable std::size_t textLength;
  mutable bool lengthIsValid;
};

std::size_t CTextBlock::length() const
{
  if (!lengthIsValid) {
    textLength = std::strlen(pText);
    lengthIsValid = true;
  }
  return textLength;
}
```

#### 在 const 和 non-const 中避免重复

```cpp

class TextBlock {
public:
  ...
  const char &
  operator[](std::size_t position) const
  {
    ... // 很多事
    return text[position];
  }
  char &operator[](std::size_t position)
  {
    ... // 很多事
    return text[position];
  }

private:
  std::string text;
};
```

如上，我们要把一段重复的逻辑写两遍。

如何避免？

答案是 `cast` ：令 `non-const` 版本调用 `const` 逻辑。不应该反向去做，因为这样有违 `const` 初衷。

```cpp

class TextBlock {
public:
  ...
  const char &
  operator[](std::size_t position) const
  {
    ... // 很多事
    return text[position];
  }
  char &operator[](std::size_t position)
  {
    /**
     * const_cast 将 op[] 返回值的 const 移除
     * static_cast<const TextBlock&>(*this) 为 *this 加上 const
     * 调用 const op[]
     */
    return
      const_cast<char&>(
        static_cast<const TextBlock&>(*this)
          [position]
      );
  }

private:
  std::string text;
};
```

### 04 | 确定对象被使用前已先被初始化

Make sure that object are initialized before they're used.

#### 成员初值列 member initialization list

```cpp
class PhoneNumber { ... };
class ABEntry {
public:
  ABEntry(const std::string& name, const std::string& address,
          const std::list<PhoneNumber>& phones);
private:
  std::string theName;
  std::string theAddress;
  std::list<PhoneNumber> thePhones;
  int numTimesConsulted;
};

ABEntry::ABEntry(const std::string& name, const std::string& address,
                 const std::list<PhoneNumber>& phones)
{
  theName = name;        // 这些都是复制 assignments
  theAddress = address;  // 而非初始化 initializations
  thePhones = phones;
  numTimesConsulted = 0;
}
```

**对象的成员变量的初始化动作发生在进入构造函数本体之前。**

但是内置类型的变量比如 `numTimesConsulted` 不会被自动初始化，因此不要遗漏。

如果想要效率更高、并且初始化成员变量而非赋值，使用 成员初值列 member initialization list 。

```cpp
ABEntry::ABEntry(const std::string &name, const std::string &address,
                 const std::list<PhoneNumber> &phones)
  :theName(name),      // 这些都是初始化
  theAddress(address),
  thePhones(Phones),
  numTimesConsulted(0) // 注意别遗漏了内置类型的变量
{ }
```

#### 关于 static 以及 local static

所谓 `static` 对象，其寿命从被构造出来直到程序结束位置。

函数内的 `static` 对象为 `local static` 对象，其他的 `static` 对象为 `non-local static` 。

```cpp
// 你提供给客户的
// 位于 global 或 namespace 作用域内
class FileSystem {
public:
  ...
  std::size_t numDisks() const;
  ...
};
// 预留给客户使用的对象
// the file system
extern FileSystem tfs;

// 客户建立的程序
class Directory {
public:
  Directory( params );
  ...
};
Directory::Directory( params )
{
  ...
  std::size_t disks = tfs.numDisks();
  ...
}
/**
 * 危险！因为我们无法控制初始化的顺序！
 * 在调用 tfs 时无法确定其是否已经被初始化！
 */
Directory tempDir( params );
```

如何设计呢？用 `local static` 。因为：**C++ 保证，函数内的 `local static` 对象会在该函数被调用期间首次遇上该对象的定义式时被初始化。** 因此一定可以保证被初始化。

```cpp
// 你提供给客户的
// 位于 global 或 namespace 作用域内
class FileSystem {
public:
  ...
  std::size_t numDisks() const;
  ...
};
// 预留给客户使用的对象
// the file system
FileSystem& tfs()
{
  static FileSystem fs;
  return fs;
}

// 客户建立的程序
class Directory {
public:
  Directory( params );
  ...
};
Directory::Directory( params )
{
  ...
  std::size_t disks = tfs().numDisks();
  ...
}
Directory& tempDir()
{
  static Directory td;
  return td;
}
```
