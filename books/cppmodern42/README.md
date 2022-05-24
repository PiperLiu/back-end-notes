# Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14

Scott Meyers. Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14. 中国电力出版社. 高博译. 第一版

第二本 Effective C++ ，现在 C++ 11 已经十分普遍，因此先读 modern 版本。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1 | 型别推导](#1-型别推导)
- [2 | auto](#2-auto)
- [3 | 转向现代 C++](#3-转向现代-c)
- [4 | 智能指针](#4-智能指针)
- [5 | 右值引用、移动语义和完美转发](#5-右值引用-移动语义和完美转发)

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

[./drafts/cppmodern42.03.feats.md](./drafts/cppmodern42.03.feats.md)
- [7 | 在创建对象时注意区分 () 和 {}](./drafts/cppmodern42.03.feats.md#7-在创建对象时注意区分-和)
  - [使用大括号的调用构造函数（喜忧参半）](./drafts/cppmodern42.03.feats.md#使用大括号的调用构造函数喜忧参半)
  - [关于 std::vector 接口设计的败笔（与可变模板配合）](./drafts/cppmodern42.03.feats.md#关于-stdvector-接口设计的败笔与可变模板配合)
- [8 | 优先选用 nullptr ，而非 0 或 NULL](./drafts/cppmodern42.03.feats.md#8-优先选用-nullptr-而非-0-或-null)
- [9 | 优先选用别名声明，而非 typedef](./drafts/cppmodern42.03.feats.md#9-优先选用别名声明而非-typedef)
  - [别名模板 alias template](./drafts/cppmodern42.03.feats.md#别名模板-alias-template)
  - [关于 remove_reference 与 remove_reference_t](./drafts/cppmodern42.03.feats.md#关于-remove_reference-与-remove_reference_t)
- [10 | 优先选用限定作用域的枚举型别，而非不限定作用域的枚举型别](./drafts/cppmodern42.03.feats.md#10-优先选用限定作用域的枚举型别而非不限定作用域的枚举型别)
  - [enum class 的前置声明](./drafts/cppmodern42.03.feats.md#enum-class-的前置声明)
- [11 | 优先选用删除函数，而非 private 未定义函数](./drafts/cppmodern42.03.feats.md#11-优先选用删除函数而非-private-未定义函数)
  - [通过删除函数拒绝型别与（指针）模板特化](./drafts/cppmodern42.03.feats.md#通过删除函数拒绝型别与指针模板特化)
- [12 | 为意在改写的函数添加 override 声明](./drafts/cppmodern42.03.feats.md#12-为意在改写的函数添加-override-声明)
  - [引用饰词（reference qualifier）](./drafts/cppmodern42.03.feats.md#引用饰词reference-qualifier)
  - [加上 override 的必要性](./drafts/cppmodern42.03.feats.md#加上-override-的必要性)
- [13 | 优先选用 const_iterator ，而非 iterator](./drafts/cppmodern42.03.feats.md#13-优先选用-const_iterator-而非-iterator)
- [14 | 只要函数不会发射异常，就为其加上 noexcept 声明](./drafts/cppmodern42.03.feats.md#14-只要函数不会发射异常就为其加上-noexcept-声明)
  - [关于 throw() 和 noexcept 发射异常以及调用栈开解 stack unwinding](./drafts/cppmodern42.03.feats.md#关于-throw-和-noexcept-发射异常以及调用栈开解-stack-unwinding)
  - [大多数函数都是异常中立的（exception-neutral），那么什么时候用 noexcept](./drafts/cppmodern42.03.feats.md#大多数函数都是异常中立的exception-neutral那么什么时候用-noexcept)
  - [只要有可能使用 constexpr ，就是用它](./drafts/cppmodern42.03.feats.md#只要有可能使用-constexpr-就是用它)
  - [constexpr 函数可在编译器计算](./drafts/cppmodern42.03.feats.md#constexpr-函数可在编译器计算)
- [16 | 保证 const 成员函数的线程安全性](./drafts/cppmodern42.03.feats.md#16-保证-const-成员函数的线程安全性)
  - [只移型别（move-only type）的 mutex 保证线程安全](./drafts/cppmodern42.03.feats.md#只移型别move-only-type的-mutex-保证线程安全)
  - [std::atomic 取代互斥量的例子](./drafts/cppmodern42.03.feats.md#stdatomic-取代互斥量的例子)
- [17 | 理解特种成员函数的生成机制](./drafts/cppmodern42.03.feats.md#17-理解特种成员函数的生成机制)
  - [移动构造函数和移动赋值运算符](./drafts/cppmodern42.03.feats.md#移动构造函数和移动赋值运算符)
  - [=default关键字](./drafts/cppmodern42.03.feats.md#default关键字)

### 4 | 智能指针

[./drafts/cppmodern42.04.sptr.md](./drafts/cppmodern42.04.sptr.md)
- [18 | 使用 std::unique_ptr 管理具备专属所有权的资源](./drafts/cppmodern42.04.sptr.md#18-使用-stdunique_ptr-管理具备专属所有权的资源)
  - [只移型别 unique_ptr 应用：工厂函数返回型别](./drafts/cppmodern42.04.sptr.md#只移型别-unique_ptr-应用工厂函数返回型别)
  - [析构型别是 unique_ptr 型别的一部分，影响 unique_ptr 尺寸](./drafts/cppmodern42.04.sptr.md#析构型别是-unique_ptr-型别的一部分影响-unique_ptr-尺寸)
  - [std::unique_ptr 数组不常用；可隐式转为 std::shared_ptr](./drafts/cppmodern42.04.sptr.md#stdunique_ptr-数组不常用可隐式转为-stdshared_ptr)
- [19 | 使用 std::shared_ptr 管理具备共享所有权的资源](./drafts/cppmodern42.04.sptr.md#19-使用-stdshared_ptr-管理具备共享所有权的资源)
  - [std::shared_ptr 结构（与 std::unique_ptr 不同，析构器不是型别一部分）](./drafts/cppmodern42.04.sptr.md#stdshared_ptr-结构与-stdunique_ptr-不同析构器不是型别一部分)
  - [std::shared_ptr 控制块的讨论](./drafts/cppmodern42.04.sptr.md#stdshared_ptr-控制块的讨论)
  - [enable_shared_from_this 和奇妙递归模板模式 The Curiously Recurring Template Pattern, CRTP](./drafts/cppmodern42.04.sptr.md#enable_shared_from_this-和奇妙递归模板模式-the-curiously-recurring-template-pattern-crtp)
- [20 | 对于类似 std::shared_ptr 但有可能空悬的指针使用 std::weak_ptr](./drafts/cppmodern42.04.sptr.md#20-对于类似-stdshared_ptr-但有可能空悬的指针使用-stdweak_ptr)
  - [API 如 expired() 和 lock() 等](./drafts/cppmodern42.04.sptr.md#api-如-expired-和-lock-等)
  - [std::weak_ptr 用例与观察者设计模式 Observer design pattern](./drafts/cppmodern42.04.sptr.md#stdweak_ptr-用例与观察者设计模式-observer-design-pattern)
- [21 | 优先选用 std::make_unique 和 std::make_shared ，而非直接使用 new](./drafts/cppmodern42.04.sptr.md#21-优先选用-stdmake_unique-和-stdmake_shared-而非直接使用-new)
  - [make 系列好处：异常安全性，速度更快（动态分配内存次数少)](./drafts/cppmodern42.04.sptr.md#make-系列好处异常安全性速度更快动态分配内存次数少)
  - [不能使用 make 系列的情形（使用其的劣势）](./drafts/cppmodern42.04.sptr.md#不能使用-make-系列的情形使用其的劣势)
- [22 | 使用 Pimpl 习惯用法时，将特殊成员函数的定义放到实现文件中](./drafts/cppmodern42.04.sptr.md#22-使用-pimpl-习惯用法时将特殊成员函数的定义放到实现文件中)
  - [非完整型别与 pImpl 的样例](./drafts/cppmodern42.04.sptr.md#非完整型别与-pimpl-的样例)
  - [unique_ptr 实现 pImpl 挑战](./drafts/cppmodern42.04.sptr.md#unique_ptr-实现-pimpl-挑战)

### 5 | 右值引用、移动语义和完美转发

[./drafts/cppmodern42.05.move.md](./drafts/cppmodern42.05.move.md)
