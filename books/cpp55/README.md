# Effective C++: 55 Specific Ways to Improve Your Programs and Designs

Scott Meyers. Effective C++: 55 Specific Ways to Improve Your Programs and Designs. 电子工业出版社. 侯捷译. 第三版

本书局限于 C++ 98 说实在有点过时。

C++ 11 是十分有必要学习的，快速把本书过完吧，然后再学 `STL -> More Effective C++ -> Modern Effective C++` 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1 | 让自己习惯 C++](#1-让自己习惯-c)
- [2 | 构造/析构/赋值运算](#2-构造析构赋值运算)

<!-- /code_chunk_output -->

### 1 | 让自己习惯 C++

[./drafts/cpp55.01.know.md](./drafts/cpp55.01.know.md)

- [导读部分](./drafts/cpp55.01.know.md#导读部分)
  - [定义一定调用构造函数](./drafts/cpp55.01.know.md#定义一定调用构造函数)
  - [命名习惯 Naming Conventions](./drafts/cpp55.01.know.md#命名习惯-naming-conventions)
- [01 | 视 C++ 为一个语言联邦](./drafts/cpp55.01.know.md#01-视-c-为一个语言联邦)
- [02 | 尽量以 const, enum, inline 替换 ./drafts/cpp55.01.know.md#define](#02-尽量以-const-enum-inline-替换-define)
  - [声明与定义的区别](./drafts/cpp55.01.know.md#声明与定义的区别)
  - [class 专属常量被特殊处理](./drafts/cpp55.01.know.md#class-专属常量被特殊处理)
  - [滥用 ./drafts/cpp55.01.know.md#define 的坏处](#滥用-define-的坏处)
- [03 | 尽可能使用 const](./drafts/cpp55.01.know.md#03-尽可能使用-const)
  - [const 与指针](./drafts/cpp55.01.know.md#const-与指针)
  - [const 在声明中的作用](./drafts/cpp55.01.know.md#const-在声明中的作用)
  - [const 成员函数](./drafts/cpp55.01.know.md#const-成员函数)
  - [mutable 之于 const 成员函数](./drafts/cpp55.01.know.md#mutable-之于-const-成员函数)
  - [在 const 和 non-const 中避免重复](./drafts/cpp55.01.know.md#在-const-和-non-const-中避免重复)
- [04 | 确定对象被使用前已先被初始化](./drafts/cpp55.01.know.md#04-确定对象被使用前已先被初始化)
  - [成员初值列 member initialization list](./drafts/cpp55.01.know.md#成员初值列-member-initialization-list)
  - [关于 static 以及 local static](./drafts/cpp55.01.know.md#关于-static-以及-local-static)

### 2 | 构造/析构/赋值运算

[./drafts/cpp55.02.constructors.destructors.assignment.md](./drafts/cpp55.02.constructors.destructors.assignment.md)

- [05 | 了解 C++ 默默编写并调用哪些函数](./drafts/cpp55.02.constructors.destructors.assignment.md#05-了解-c-默默编写并调用哪些函数)
  - [C++ 默认实现了构造函数等内容](./drafts/cpp55.02.constructors.destructors.assignment.md#c-默认实现了构造函数等内容)
  - [更好地理解 static](./drafts/cpp55.02.constructors.destructors.assignment.md#更好地理解-static)
- [06 若不想使用编译器自动生成的函数，就该明确拒绝](./drafts/cpp55.02.constructors.destructors.assignment.md#06-若不想使用编译器自动生成的函数就该明确拒绝)
  - [将 copy 构造函数或操作符设为 private 以防止拷贝对象](./drafts/cpp55.02.constructors.destructors.assignment.md#将-copy-构造函数或操作符设为-private-以防止拷贝对象)
  - [函数声明不写参数名称是可以的](./drafts/cpp55.02.constructors.destructors.assignment.md#函数声明不写参数名称是可以的)
- [07 | 为多态基类声明 virtual 析构函数](./drafts/cpp55.02.constructors.destructors.assignment.md#07-为多态基类声明-virtual-析构函数)
  - [基类的析构函数 non-virtual 带来灾害](./drafts/cpp55.02.constructors.destructors.assignment.md#基类的析构函数-non-virtual-带来灾害)
  - [别无端声明虚函数](./drafts/cpp55.02.constructors.destructors.assignment.md#别无端声明虚函数)
  - [不要继承标准容器或者其他带有 non-virtual 析构函数的类！](./drafts/cpp55.02.constructors.destructors.assignment.md#不要继承标准容器或者其他带有-non-virtual-析构函数的类)
  - [纯虚函数 pure virtual 导致抽象类 abstract class](./drafts/cpp55.02.constructors.destructors.assignment.md#纯虚函数-pure-virtual-导致抽象类-abstract-class)
- [08 | 别让异常逃离析构函数](./drafts/cpp55.02.constructors.destructors.assignment.md#08-别让异常逃离析构函数)
  - [把异常反应留给使用者，而非析构函数](./drafts/cpp55.02.constructors.destructors.assignment.md#把异常反应留给使用者而非析构函数)
- [09 | 绝不在构造和析构过程中调用 virtual 函数](./drafts/cpp55.02.constructors.destructors.assignment.md#09-绝不在构造和析构过程中调用-virtual-函数)
  - [在基类构造期间，虚函数不是虚函数](./drafts/cpp55.02.constructors.destructors.assignment.md#在基类构造期间虚函数不是虚函数)
- [10 | 令 operator= 返回一个 reference to *this](./drafts/cpp55.02.constructors.destructors.assignment.md#10-令-operator-返回一个-reference-to-this)
  - [不仅适用于=，适用于所有赋值相关运算，是一个协议，合理的协议](./drafts/cpp55.02.constructors.destructors.assignment.md#不仅适用于适用于所有赋值相关运算是一个协议合理的协议)
- [11 | 在 operator= 中处理“自我赋值”](./drafts/cpp55.02.constructors.destructors.assignment.md#11-在-operator-中处理自我赋值)
  - [自我赋值可能带来的错误](./drafts/cpp55.02.constructors.destructors.assignment.md#自我赋值可能带来的错误)
  - [证同测试 identity test](./drafts/cpp55.02.constructors.destructors.assignment.md#证同测试-identity-test)
  - [调整语句顺序保证异常安全性](./drafts/cpp55.02.constructors.destructors.assignment.md#调整语句顺序保证异常安全性)
  - [copy-and-swap](./drafts/cpp55.02.constructors.destructors.assignment.md#copy-and-swap)
- [12 | 复制对象时勿忘其每一个成份](./drafts/cpp55.02.constructors.destructors.assignment.md#12-复制对象时勿忘其每一个成份)
  - [Copying 函数应该确保复制所有成份](./drafts/cpp55.02.constructors.destructors.assignment.md#copying-函数应该确保复制所有成份)
  - [不要用 copying 函数实现另一个 copying 函数](./drafts/cpp55.02.constructors.destructors.assignment.md#不要用-copying-函数实现另一个-copying-函数)
