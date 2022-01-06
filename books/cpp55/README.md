# Effective C++: 55 Specific Ways to Improve Your Programs and Designs

Scott Meyers. Effective C++: 55 Specific Ways to Improve Your Programs and Designs. 电子工业出版社. 侯捷译. 第三版

本书局限于 C++ 98 说实在有点过时。

C++ 11 是十分有必要学习的，快速把本书过完吧，然后再学 `STL -> More Effective C++ -> Modern Effective C++` 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1 | 让自己习惯 C++](#1-让自己习惯-c)

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
