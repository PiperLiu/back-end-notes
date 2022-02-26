# Effective C++: 55 Specific Ways to Improve Your Programs and Designs

Scott Meyers. Effective C++: 55 Specific Ways to Improve Your Programs and Designs. 电子工业出版社. 侯捷译. 第三版

本书局限于 C++ 98 说实在有点过时。

C++ 11 是十分有必要学习的，快速把本书过完吧，然后再学 `STL -> More Effective C++ -> Modern Effective C++` 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1 | 让自己习惯 C++](#1-让自己习惯-c)
- [2 | 构造/析构/赋值运算](#2-构造析构赋值运算)
- [3 | 资源管理](#3-资源管理)
- [4 | 设计与声明](#4-设计与声明)
- [5 | 实现](#5-实现)
- [6 | 继承与面型对象设计](#6-继承与面型对象设计)
- [7 | 模板与泛型编程](#7-模板与泛型编程)
- [8 | 定制 new 和 delete](#8-定制-new-和-delete)
- [9 | 杂项讨论](#9-杂项讨论)

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
- [06 | 若不想使用编译器自动生成的函数，就该明确拒绝](./drafts/cpp55.02.constructors.destructors.assignment.md#06-若不想使用编译器自动生成的函数就该明确拒绝)
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

### 3 | 资源管理

[./drafts/cpp55.03.resource.management.md](./drafts/cpp55.03.resource.management.md)

- [13 | 以对象管理资源](./drafts/cpp55.03.resource.management.md#13-以对象管理资源)
  - [智能指针 std::auto_ptr](./drafts/cpp55.03.resource.management.md#智能指针-stdauto_ptr)
  - [RCSP 的 shared_ptr 解决 auto_ptr 唯一所有权问题](./drafts/cpp55.03.resource.management.md#rcsp-的-shared_ptr-解决-auto_ptr-唯一所有权问题)
- [14 | 在资源管理类中小心 copying 行为](./drafts/cpp55.03.resource.management.md#14-在资源管理类中小心-copying-行为)
  - [资源取得时机便是初始化时机](./drafts/cpp55.03.resource.management.md#资源取得时机便是初始化时机)
  - [mutex 为例： RAII 复制行为](./drafts/cpp55.03.resource.management.md#mutex-为例-raii-复制行为)
- [15 | 在资源管理类中提供对原始资源的访问](./drafts/cpp55.03.resource.management.md#15-在资源管理类中提供对原始资源的访问)
  - [智能指针重载了指针取值操作符](./drafts/cpp55.03.resource.management.md#智能指针重载了指针取值操作符)
  - [重载隐式转换函数](./drafts/cpp55.03.resource.management.md#重载隐式转换函数)
- [16 | 成对使用 new 和 delete 时要采取相同形式](./drafts/cpp55.03.resource.management.md#16-成对使用-new-和-delete-时要采取相同形式)
- [17 | 以独立语句将 newed 对象置入智能指针](./drafts/cpp55.03.resource.management.md#17-以独立语句将-newed-对象置入智能指针)

### 4 | 设计与声明

[./drafts/cpp55.04.designs.declarations.md](./drafts/cpp55.04.designs.declarations.md)

- [18 | 让接口容易被正确使用，不易被误用](./drafts/cpp55.04.designs.declarations.md#18-让接口容易被正确使用不易被误用)
- [19 | 设计 class 犹如设计 type](./drafts/cpp55.04.designs.declarations.md#19-设计-class-犹如设计-type)
- [20 | 宁以 pass-by-reference-to-const 替换 pass-by-value](./drafts/cpp55.04.designs.declarations.md#20-宁以-pass-by-reference-to-const-替换-pass-by-value)
  - [传引用可以防止参数被切割](./drafts/cpp55.04.designs.declarations.md#传引用可以防止参数被切割)
- [21 | 必须返回对象时，别妄想返回其 reference](./drafts/cpp55.04.designs.declarations.md#21-必须返回对象时别妄想返回其-reference)
  - [函数创建新对象之 stack-based 与 heap-based 的问题](./drafts/cpp55.04.designs.declarations.md#函数创建新对象之-stack-based-与-heap-based-的问题)
- [22 | 将成员变量声明为 private](./drafts/cpp55.04.designs.declarations.md#22-将成员变量声明为-private)
- [23 | 宁以 non-member 、 non-friend 替换 member 函数](./drafts/cpp55.04.designs.declarations.md#23-宁以-non-member-non-friend-替换-member-函数)
  - [使用命名空间函数替代成员函数](./drafts/cpp55.04.designs.declarations.md#使用命名空间函数替代成员函数)
- [24 | 若所有参数皆需类型转换，请为此次采用 non-member 函数](./drafts/cpp55.04.designs.declarations.md#24-若所有参数皆需类型转换请为此次采用-non-member-函数)
  - [成员函数中的函数难以对外部变量转换类型](./drafts/cpp55.04.designs.declarations.md#成员函数中的函数难以对外部变量转换类型)
- [25 | 考虑写出一个不抛出异常的 swap 函数](./drafts/cpp55.04.designs.declarations.md#25-考虑写出一个不抛出异常的-swap-函数)
  - [典型的 std::swap 可能浪费性能](./drafts/cpp55.04.designs.declarations.md#典型的-stdswap-可能浪费性能)
  - [全特化 total template specialization](./drafts/cpp55.04.designs.declarations.md#全特化-total-template-specialization)
  - [偏特化 paritally specialize 与 swap class template 而非 classes](./drafts/cpp55.04.designs.declarations.md#偏特化-paritally-specialize-与-swap-class-template-而非-classes)

### 5 | 实现

[./drafts/cpp55.05.implementations.md](./drafts/cpp55.05.implementations.md)

- [26 | 尽可能延后变量定义式的出现时间](./drafts/cpp55.05.implementations.md)#26-尽可能延后变量定义式的出现时间)
- [27 | 尽量少做转型动作](./drafts/cpp55.05.implementations.md)#27-尽量少做转型动作)
  - [四种新式转型动作](./drafts/cpp55.05.implementations.md)#四种新式转型动作)
  - [在子类中乱用转型案例及纠错](./drafts/cpp55.05.implementations.md)#在子类中乱用转型案例及纠错)
  - [在容器迭代中乱用转型案例及纠错](./drafts/cpp55.05.implementations.md)#在容器迭代中乱用转型案例及纠错)
- [28 | 避免返回 handles 指向对象内部成份](./drafts/cpp55.05.implementations.md)#28-避免返回-handles-指向对象内部成份)
- [29 | 为“异常安全”而努力是值得的](./drafts/cpp55.05.implementations.md)#29-为异常安全而努力是值得的)
  - [什么是 copy-and-swap 策略？](./drafts/cpp55.05.implementations.md)#什么是-copy-and-swap-策略)
- [30 | 透彻了解 inlining 的里里外外](./drafts/cpp55.05.implementations.md)#30-透彻了解-inlining-的里里外外)
- [31 | 将文件间的编译依存关系降至最低](./drafts/cpp55.05.implementations.md)#31-将文件间的编译依存关系降至最低)
  - [Handle classes / pimpl idiom](./drafts/cpp55.05.implementations.md)#handle-classes-pimpl-idiom)
  - [Interface classes 与 factory method](./drafts/cpp55.05.implementations.md)#interface-classes-与-factory-method)

### 6 | 继承与面型对象设计

[./drafts/cpp55.06.inheritance.md](./drafts/cpp55.06.inheritance.md)

- [32 | 确定你的 public 继承塑模出 is-a 关系](./drafts/cpp55.06.inheritance.md#32-确定你的-public-继承塑模出-is-a-关系)
- [33 | 避免遮掩继承而来的名称](./drafts/cpp55.06.inheritance.md#33-避免遮掩继承而来的名称)
  - [子类的名称会遮掩基类的名称](./drafts/cpp55.06.inheritance.md#子类的名称会遮掩基类的名称)
  - [使用 using 声明或者转交函数 forwarding functions 可以让名称取消遮掩](./drafts/cpp55.06.inheritance.md#使用-using-声明或者转交函数-forwarding-functions-可以让名称取消遮掩)
- [34 | 区分接口继承和实现继承](./drafts/cpp55.06.inheritance.md#34-区分接口继承和实现继承)
- [35 | 考虑 virtual 函数以外的其他选择](./drafts/cpp55.06.inheritance.md#35-考虑-virtual-函数以外的其他选择)
  - [基类中最常规的 virtual 方法](./drafts/cpp55.06.inheritance.md#基类中最常规的-virtual-方法)
  - [非虚接口实现 Template Method 模式](./drafts/cpp55.06.inheritance.md#非虚接口实现-template-method-模式)
  - [由 Function Pointers 实现 Strategy 模式](./drafts/cpp55.06.inheritance.md#由-function-pointers-实现-strategy-模式)
  - [由 tr1::function 完成 Strategy 模式](./drafts/cpp55.06.inheritance.md#由-tr1function-完成-strategy-模式)
  - [古典的 Strategy 模式](./drafts/cpp55.06.inheritance.md#古典的-strategy-模式)
- [36 | 绝不重新定义继承而来的 non-virtual 函数](./drafts/cpp55.06.inheritance.md#36-绝不重新定义继承而来的-non-virtual-函数)
  - [non-virtual 函数带来静态绑定 statically bound](./drafts/cpp55.06.inheritance.md#non-virtual-函数带来静态绑定-statically-bound)
- [37 | 绝不重新定义继承而来的缺省参数值](./drafts/cpp55.06.inheritance.md#37-绝不重新定义继承而来的缺省参数值)
  - [虽然 virtual 函数是动态绑定 dynamically bound ，但是参数值却是静态绑定](./drafts/cpp55.06.inheritance.md#虽然-virtual-函数是动态绑定-dynamically-bound-但是参数值却是静态绑定)
- [38 | 通过复合塑模出 has-a 或“根据某物实现出”](./drafts/cpp55.06.inheritance.md#38-通过复合塑模出-has-a-或根据某物实现出)
- [39 | 明智而审慎地使用 private 继承](./drafts/cpp55.06.inheritance.md#39-明智而审慎地使用-private-继承)
  - [为什么宁用符合也不用 private 继承](./drafts/cpp55.06.inheritance.md#为什么宁用符合也不用-private-继承)
  - [什么时候用 private ？ EBO empty base optimization](./drafts/cpp55.06.inheritance.md#什么时候用-private-ebo-empty-base-optimization)
- [40 | 明智而审慎地使用多重继承](./drafts/cpp55.06.inheritance.md#40-明智而审慎地使用多重继承)
  - [多重继承可能导致歧义](./drafts/cpp55.06.inheritance.md#多重继承可能导致歧义)
  - [virtual 继承](./drafts/cpp55.06.inheritance.md#virtual-继承)

### 7 | 模板与泛型编程

[./drafts/cpp55.07.templates.generic.md](./drafts/cpp55.07.templates.generic.md)

- [41 | 了解隐式接口和编译期多态](./drafts/cpp55.07.templates.generic.md#41-了解隐式接口和编译期多态)
  - [显示接口 explicit interface 和隐式接口 valid expressions](./drafts/cpp55.07.templates.generic.md#显示接口-explicit-interface-和隐式接口-valid-expressions)
- [42 | 了解 typename 的双重意义](./drafts/cpp55.07.templates.generic.md#42-了解-typename-的双重意义)
  - [typename 告诉编译器从属名称 dependent name 也是个类型](./drafts/cpp55.07.templates.generic.md#typename-告诉编译器从属名称-dependent-name-也是个类型)
  - [不能使用 typename 之处](./drafts/cpp55.07.templates.generic.md#不能使用-typename-之处)
- [43 | 学习处理模板化基类内的名称](./drafts/cpp55.07.templates.generic.md#43-学习处理模板化基类内的名称)
  - [防止“拒绝在模板化基类”中寻找继承的名称三种方法](./drafts/cpp55.07.templates.generic.md#防止拒绝在模板化基类中寻找继承的名称三种方法)
- [44 | 将与参数无关的代码抽离 templates](./drafts/cpp55.07.templates.generic.md#44-将与参数无关的代码抽离-templates)
- [45 | 运用成员函数模板接受所有兼容类型](./drafts/cpp55.07.templates.generic.md#45-运用成员函数模板接受所有兼容类型)
  - [必须同时声明泛化 copy 构造函数和“正常的” copy 构造函数](./drafts/cpp55.07.templates.generic.md#必须同时声明泛化-copy-构造函数和正常的-copy-构造函数)
- [46 | 需要类型转换时请为模板定义非成员函数](./drafts/cpp55.07.templates.generic.md#46-需要类型转换时请为模板定义非成员函数)
  - [template 在实参推导过程中不将隐式类型转换纳入考虑](./drafts/cpp55.07.templates.generic.md#template-在实参推导过程中不将隐式类型转换纳入考虑)
  - [应用 friend 函数让成员函数成为非成员函数](./drafts/cpp55.07.templates.generic.md#应用-friend-函数让成员函数成为非成员函数)
- [47 | 请使用 traits classes 表现类型信息](./drafts/cpp55.07.templates.generic.md#47-请使用-traits-classes-表现类型信息)
  - [STL 迭代器分类](./drafts/cpp55.07.templates.generic.md#stl-迭代器分类)
  - [Traits 是一个 C++ 协议](./drafts/cpp55.07.templates.generic.md#traits-是一个-c-协议)
- [48 | 认识 template 元编程](./drafts/cpp55.07.templates.generic.md#48-认识-template-元编程)
  - [使用 TMP 计算阶乘](./drafts/cpp55.07.templates.generic.md#使用-tmp-计算阶乘)

### 8 | 定制 new 和 delete

[./drafts/cpp55.08.new.md](./drafts/cpp55.08.new.md)

- [49 | 了解 new-handler 的行为](./drafts/cpp55.08.new.md#49-了解-new-handler-的行为)
  - [new 的错误处理函数 new-handler](./drafts/cpp55.08.new.md#new-的错误处理函数-new-handler)
  - [设计专属的 set_new_handler](./drafts/cpp55.08.new.md#设计专属的-set_new_handler)
  - [设计模板化的 new_handler](./drafts/cpp55.08.new.md#设计模板化的-new_handler)
  - [nothrow 形式：分配失败返回 null](./drafts/cpp55.08.new.md#nothrow-形式分配失败返回-null)
- [50 | 了解 new 和 delete 的合理替换时机](./drafts/cpp55.08.new.md#50-了解-new-和-delete-的合理替换时机)
- [51 | 编写 new 和 delete 时需固守常规](./drafts/cpp55.08.new.md#51-编写-new-和-delete-时需固守常规)
  - [即便客户要求 0 bytes ， new 也得返回一个合法指针](./drafts/cpp55.08.new.md#即便客户要求-0-bytes-new-也得返回一个合法指针)
- [52 | 写了 placement new 也要写 placement delete](./drafts/cpp55.08.new.md#52-写了-placement-new-也要写-placement-delete)
  - [考虑 placement new 异常的情况](./drafts/cpp55.08.new.md#考虑-placement-new-异常的情况)
  - [placement new 会覆盖正常的 new](./drafts/cpp55.08.new.md#placement-new-会覆盖正常的-new)

### 9 | 杂项讨论

[./drafts/cpp55.09.miscellany.md](./drafts/cpp55.09.miscellany.md)

- [53 | 不要轻忽编译器的警告](./drafts/cpp55.09.miscellany.md#53-不要轻忽编译器的警告)
- [54 | 让自己熟悉包括 TR1 在内的标准程序库](./drafts/cpp55.09.miscellany.md#54-让自己熟悉包括-tr1-在内的标准程序库)
- [55 | 让自己熟悉 Boost](./drafts/cpp55.09.miscellany.md#55-让自己熟悉-boost)
