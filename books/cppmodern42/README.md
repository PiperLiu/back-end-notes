# Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14

Scott Meyers. Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14. 中国电力出版社. 高博译. 第一版

第二本 Effective C++ ，现在 C++ 11 已经十分普遍，因此先读 modern 版本。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1 | 型别推导](#1-型别推导)
- [2 | auto](#2-auto)

<!-- /code_chunk_output -->

### 1 | 型别推导

[./drafts/cppmodern42.01.typeinfer.md](./drafts/cppmodern42.01.typeinfer.md)
- [1 | 理解模板型别推导](./drafts/cppmodern42.01.typeinfer.md#1-理解模板型别推导)
  - [ParamType 是个指针或引用，但不是个万能引用](./drafts/cppmodern42.01.typeinfer.md#paramtype-是个指针或引用但不是个万能引用)
  - [ParamType 是个万能引用](./drafts/cppmodern42.01.typeinfer.md#paramtype-是个万能引用)
  - [ParamType 既非指针也非引用](./drafts/cppmodern42.01.typeinfer.md#paramtype-既非指针也非引用)
  - [数组实参](./drafts/cppmodern42.01.typeinfer.md#数组实参)
- [函数实参](./drafts/cppmodern42.01.typeinfer.md#函数实参)
- [2 | 理解 auto 型别推导](./drafts/cppmodern42.01.typeinfer.md#2-理解-auto-型别推导)
  - [C++ 11 支持统一初始化 uniform initialization （大括号）](./drafts/cppmodern42.01.typeinfer.md#c-11-支持统一初始化-uniform-initialization-大括号)
  - [auto 型别推导就是模板推导，只有一个例外](./drafts/cppmodern42.01.typeinfer.md#auto-型别推导就是模板推导只有一个例外)
  - [C++ 14 运行 auto 作为返回值，但使用的是模板推导](./drafts/cppmodern42.01.typeinfer.md#c-14-运行-auto-作为返回值但使用的是模板推导)
- [3 | 理解 decltype](./drafts/cppmodern42.01.typeinfer.md#3-理解-decltype)
  - [C++ 11 返回值型别尾序语法 trailing return type syntax](./drafts/cppmodern42.01.typeinfer.md#c-11-返回值型别尾序语法-trailing-return-type-syntax)
  - [decltype(auto) 不会抹去型别信息](./drafts/cppmodern42.01.typeinfer.md#decltypeauto-不会抹去型别信息)
  - [考虑万能引用的 decltype(auto)](./drafts/cppmodern42.01.typeinfer.md#考虑万能引用的-decltypeauto)
  - [decltype 特殊情况](./drafts/cppmodern42.01.typeinfer.md#decltype-特殊情况)
- [4 | 掌握查看型别推导结果的方法](./drafts/cppmodern42.01.typeinfer.md#4-掌握查看型别推导结果的方法)
  - [Boost.TypeIndex](./drafts/cppmodern42.01.typeinfer.md#boosttypeindex)

### 2 | auto

[./drafts/cppmodern42.02.auto.md](./drafts/cppmodern42.02.auto.md)
- [5 | 优先选用 auto ，而非显式型别声明](./drafts/cppmodern42.02.auto.md#5-优先选用-auto-而非显式型别声明)
  - [auto 对 std::function 声明的优化（更好地应用闭包）](./drafts/cppmodern42.02.auto.md#auto-对-stdfunction-声明的优化更好地应用闭包)
  - [auto 性能优化实例（unordered_map）](./drafts/cppmodern42.02.auto.md#auto-性能优化实例unordered_map)
- [6 | 当 auto 推到的型别不符合要求时，使用带显式型别的初始化物习惯用法](./drafts/cppmodern42.02.auto.md#6-当-auto-推到的型别不符合要求时使用带显式型别的初始化物习惯用法)

### 3 | 转向现代 C++

[]()