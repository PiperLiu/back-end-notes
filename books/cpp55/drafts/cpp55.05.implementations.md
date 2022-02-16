# 5 | 实现

Implementations

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [26 | 尽可能延后变量定义式的出现时间](#26-尽可能延后变量定义式的出现时间)
- [27 | 尽量少做转型动作](#27-尽量少做转型动作)
  - [四种新式转型动作](#四种新式转型动作)
  - [在子类中乱用转型案例及纠错](#在子类中乱用转型案例及纠错)
  - [在容器迭代中乱用转型案例及纠错](#在容器迭代中乱用转型案例及纠错)
- [28 | 避免返回 handles 指向对象内部成份](#28-避免返回-handles-指向对象内部成份)
- [29 | 为“异常安全”而努力是值得的](#29-为异常安全而努力是值得的)
  - [什么是 copy-and-swap 策略？](#什么是-copy-and-swap-策略)
- [30 | 透彻了解 inlining 的里里外外](#30-透彻了解-inlining-的里里外外)
- [31 | 将文件间的编译依存关系降至最低](#31-将文件间的编译依存关系降至最低)
  - [Handle classes / pimpl idiom](#handle-classes-pimpl-idiom)
  - [Interface classes 与 factory method](#interface-classes-与-factory-method)

<!-- /code_chunk_output -->

### 26 | 尽可能延后变量定义式的出现时间

Postpone variable definitions as long as possible.

```cpp
// A
Widget w;
for (int i = 0; i < n; ++ i) {
    w = ...i;
    ...
}

// B
for (int i = 0; i < n; ++ i) {
    Widget w(...i);
    ...
}
```

上述有两个方法， A 会调用 1 次构造函数、 1 次析构函数和 n 次赋值操作； B 会调用 n 次构造函数和 n 次赋值操作。

性能上需要自己权衡。

在本节，他的意思是将定义式出现时间延后，以增加程序清晰度。

### 27 | 尽量少做转型动作

Minimize casting.

#### 四种新式转型动作

C++ 有三种转型：

```cpp
// C 风格
(T) expression

// 函数风格
T(expression)
```

以及有以下四种 C++ 的 new-style 转型：

```cpp
const_cast<T>(expression)
dynamic_cast<T>(expression)
reinterpret_cast<T>(expression)
static_cast<T>(expression)
```

- const_cast 将 const 移除
- dynamic_cast 安全向下转型
- reinterpret_cast 低级转型
- static_cast 强迫隐式转换，例如将 non-const 转为 const

#### 在子类中乱用转型案例及纠错

```cpp
class Window {
public:
    virtual void onResize() { ... }
    ...
};
class SpecialWindow: public Window {
public:
    virtual void onResize() {
        static_cast<Window>(*this).onResize();  // 严重错误
    }
}
```

如上导致严重错误。使用 `static_cast<Window>(*this)` 会创建一个新的副本，而非在本 this 上调用父类方法。

正确做法如下。

```cpp
class Window {
public:
    virtual void onResize() { ... }
    ...
};
class SpecialWindow: public Window {
public:
    virtual void onResize() {
        Window::onResize();  // 正确调用父类方法，作用于 *hits 身上
    }
}
```

#### 在容器迭代中乱用转型案例及纠错

```cpp
class Window { ... };
class SpecialWindow: public Window {
public:
    void blink();
    ...
}

typedef
std::vector<std::shared_ptr<Window>> VPW;
VPW winPtrs;
...
for (VPW::iterator iter = winPtrs.begin();
     iter != winPtrs.end(); ++ iter) {
    if (SpecialWindow *psw = dynamic_cast<SpecialWindow *> (iter->get()))
        psw->blink();
}
```

如上，想要调用 SpecialWindow 独有的 blink 方法。上述做法使用 dynamic_cast ，导致效率低下。

正确做法如下。

```cpp
class Window {
public:
    virtual void blink() { }
};
class SpecialWindow: public Window {
public:
    void blink();
    ...
}

typedef
std::vector<std::shared_ptr<Window>> VPW;
VPW winPtrs;
...
for (VPW::iterator iter = winPtrs.begin();
     iter != winPtrs.end(); ++ iter) {
    (*iter)->blink();
}
```

如上，在父类中就实现一个“什么都没做”的 blink 。

### 28 | 避免返回 handles 指向对象内部成份

Avoid returning "handles" to object internals.

```cpp
class Rectangle {
public:
    ...
    /**
     * 注意这里返回的是私有变量的引用&
     */
    const Point& upperLeft() const { return pData-> ulhc; }
    const Point& lowerRight() const { return pData-> lrhc; }
    ...
}

// 客户使用：
class GUIobject{ ... };
const Rectangle boundingBox(const GUIobject &obj); // 返回一个 Rectangle
// 如下代码有严重错误
GUIobject* pgp;
...
const Point* pUpperLeft = &(boundingBox(*pgo).upperLeft());
```

最后一句话有严重错误： boundingBox(*pgo) 返回的 Rectangle 没有名称，这里只是在 Rectangle 被创建出来时调用了 upperLeft ；而这个语句结束之后， Rectangle 将被析构，这将导致其内部的数据 pData 等也被析构，这样 pUpperLeft 就成了一个空指针。

因此，避免返回 handles （包括 references 、指针、迭代器）指向对象内部。

### 29 | 为“异常安全”而努力是值得的

Strive for exception-safe code.

异常安全函数 Exception-safe functions 即使发生异常也不会泄露资源或允许任何数据结构败坏。这样的函数区分为三种可能的保证：基本型、强烈型、不抛异常型。

“强烈保证”往往能够以 copy-and-swap 实现出来，但“强烈保证”并非对所有函数都可实现或具备现实意义。

函数提供的“异常安全保证”通常只等于其所调用之各个函数的“异常安全保证”中的最弱者。

#### 什么是 copy-and-swap 策略？

```cpp
class PrettyMenu {
public:
    ...
    void changeBackground(std::istream& imgSrc);
    ...
private:
    std::mutex mutex;
    Image* bgImage;
    int imageChanges;
};

// A
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    lock(&mutex);
    delete bgImage;
    ++ imageChanges;
    bgImage = new Image(imgSrc);
    unlock(&mutex);
}
```

上述的实现很糟糕，因为其在异常被抛出时，既不能保证不泄露任何资源（ newImage(imgSrc) 导致异常将让 unlock 不会被执行），又不能保证数据不被败坏（ newImage(imgSrc) 异常将导致 bgImage 指向一个已被删除对象，此外 imageChanges 也被累加 ）。

解决方法如下：

```cpp
class PrettyMenu {
        ...
    std::shared_ptr<Image> bgImage;
    ...
};
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    Lock m1(&mutex);
    delete bgImage;
    ++ imageChanges;
    bgImage = new Image(imgSrc);
}
```

更好的方案如下（更加符合异常安全函数）：

```cpp
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    Lock m1(&mutex);
    bgImage.reset(new Image(imgSrc));
    ++ imageChanges;
}
```

注意这里有个故有问题，输入的 imgSrc 可能因为读取记号被移走而导致状态改变。假设这个问题已经被解决。

我们来看更一般化的设计策略 copy and swap ：为打算修改的对象做出一份副本，然后在副本身上做一些必要的修改；若有任何修改动作抛出异常，原对象仍保持未改变状态。

是线上，将所有“隶属对象的数据”从原对象放进另一个对象内，然后赋予该对象一个指针，指向那个所谓的实现对象（ implementation object ，即副本）。

示例如下。

```cpp
struct PMImpl {
    std::shared_ptr<Image> bgImage;
    int imageChanges;
};

class PrettyMenu {
public:
    ...
    void changeBackground(std::istream& imgSrc);
    ...
private:
    std::mutex mutex;
    std::shared_ptr<PMImpl> pImpl;
};

// A
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    using std::swap;
    Lock m1(&mutex);
    std::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));
    pNew->bgImage.reset(new Image(imgSrc));
    ++ pNew->imageChanges;
    swap(pImpl, pNew);
}
```

我觉得很麻烦。异常安全性没有最好，是与时俱进的。

### 30 | 透彻了解 inlining 的里里外外

Understand the ins and outs of inlining.

注意 inline 只是对编译器的一个申请，而非强制命令。

一般而言， virtual 意味“等待，直到运行期才确定调用哪个函数”，而 inline 意味“执行前，先将调用动作替换为被调用函数的本题”。

不要乱用 inline ，比如过度使用 inline 可能造成过强的编译依赖；非 inline 函数可以让客户端在很多情况下重新连接，而非再次编译。

参考[inline function members inside a class](https://stackoverflow.com/questions/9370493/inline-function-members-inside-a-class/9370539)：

只有两种方法，让编译器提供 inline 函数：

（1）在头文件内，在声明类的时候提供成员方法的定义。

```cpp
class Human {

public:

    Human(const char* name);
    Human();

    // is implicit inline
    void lookAt(const char* name) const {
        std::cout << "I'm looking at " << name << std::endl;

    }

private:
    char _name[30]; 

};
```

（2）在头文件内，明确指出 inline 函数。

```cpp
    // is explicit inline 
    inline void lookAt(const char* name) const {
        std::cout << "I'm looking at " << name << std::endl;

    }
```

### 31 | 将文件间的编译依存关系降至最低

Minimize compilation dependencies between files.

一般地，方法是让文件相依于声明式，不要相依于定义式。

比如 STL 头文件 `<iosfwd>` 里面就全都是声明式，定义分布在 iostream 、 sstream 和 fstream 等中。

什么时候用到 iosfwd 呢？如果你自己定义的头文件，里面都是声明式，那么则 include iosfwd 就行，因为这样预处理后文件更小、编译更快、且如果 iostream 里面定义细节变了，也不需要重新编译你写的文件，重新连接就行。具体参考 [stackoverflow](https://stackoverflow.com/questions/4300696/what-is-the-iosfwd-header)：

```bash
# The following commands create a source file that includes a single header
# file (on stdout), preprocess it with g++ -E, and then count how many lines
# are in the resulting preprocessed output
$ echo '#include <iosfwd>' | g++ -E -xc++ - | wc
    2598    6534   57875
$ echo '#include <iostream>' | g++ -E -xc++ - | wc
   25631   59613  631998
```

一般地，如果我们是写库给别人调用的话，我们也先将声明式整理在 fwd 里面，然后其他声明式用到这个 fwd 。

```cpp
#include "datefwd.h"            // header file declaring (but not
                                // defining) class Date
Date today();                   // perfectly fine
void clearAppointments(Date d); // perfectly fine
```

这里记录两种方式， Handle classes 和 Interface classes 。参考 [Effective C++ item 31: Minimize Compilation Dependencies Between Files](https://blog.ycshao.com/2017/11/18/effective-c-item-31-minimize-compilation-dependencies-between-files/) 。

#### Handle classes / pimpl idiom

这个又叫做 pimpl idiom 。

如下文件必须引入三个头文件。

```cpp
#include <string>
#include "date.h"
#include "address.h"
class Person {
public:
  Person(const std::string& name, const Date& birthday,
         const Address& addr);
  std::string name() const;
  std::string birthDate() const;
  std::string address() const;
  ...
private:
      std::string theName; // has to include <string>
      Date theBirthDate; // has to include "date.h"
      Address theAddress; // has to include "address.h"
};
```

使用 handle classes 修改如下。

```cpp
#include <string>
class PersonImpl; // forward declaration
class Date; // forward declaration
class Address; // forward declaration
class Person {
public:
  Person(const std::string& name, const Date& birthday,
         const Address& addr);
  std::string name() const;
  std::string birthDate() const;
  std::string address() const;
  ...
private:
    std::shared_ptr<PersonImpl> pImpl;
};
```

这里使用了 pimpl idiom 的 Person 被称为 Handle class ，有什么好处呢？

这样，使用 Person 的客户就完全与 Data 、 Address 以及 Person 的实现细节分离了，除了降低编译依存性，这样也真正实现了“接口与实现分离”。

实现一个 Person 的成员函数：

```cpp
#include "Person.h"  // 实现 Person class 必须包含其 class 定义式
#include "PersonImpl.h"  // 同上
Person::Person(const std::string& name, const Date& birthday,
               const Address& addr)
  : pImpl(new PersonImpl(name, birthday, addr))
{ }
std::string Person::name() const
{
    return pImpl->name();
}
```

#### Interface classes 与 factory method

C++ 的接口比 java 等自由度更大。

```cpp
#include <string>
class Date;    // forward declaration
class Address; // forward declaration
class Person {
public:
    virtual ~Person();
    static std::share_ptr<Person>
        create(const std::string &name,
               const Date &birthday, const Address &addr); // factory methond
    virtual std::string name() const = 0;
    virtual Date birthDate() const = 0;
    virtual Address address() const = 0;
    ...
};
```

客户可能会这样使用：

```cpp
std::string name;
Data dataOfBirth;
Address address;
...
std::shared_ptr<Person> pp(Person::create(name, dataOfBirth, address));
...
std::cout << pp->name();
```

如下具象类则体现了接口的特性：

```cpp
class RealPerson: public Person {
public:
    RealPerson(const std::string &name,
               const Date &birthday, const Address &addr)
      : theName(name), theBirthDate(birthday), theAddress(addr)
      { }
    virtual ~RealPerson() { }
    std::string name() const { ... } ;
    std::string birthDate() const { ... } ;
    std::string address() const { ... } ;
private:
    std::string theName;
    Data theBirthDate;
    Address theAddress;
};
```

这里应用了接口的常见机制：继承接口规格，然后实现出接口覆盖函数。
