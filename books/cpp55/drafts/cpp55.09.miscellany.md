# 9 | 杂项讨论

Miscellany

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [53 | 不要轻忽编译器的警告](#53-不要轻忽编译器的警告)
- [54 | 让自己熟悉包括 TR1 在内的标准程序库](#54-让自己熟悉包括-tr1-在内的标准程序库)
- [55 | 让自己熟悉 Boost](#55-让自己熟悉-boost)

<!-- /code_chunk_output -->

### 53 | 不要轻忽编译器的警告

Pay attention to compiler warnings.

严肃对待编译器发出的警告信息。

不要过度倚赖编译器的报警能力，因为不同的编译器对待事情的态度并不相同。一旦移植到另一个编译器上，你原本倚赖的警告信息有可能消失。

### 54 | 让自己熟悉包括 TR1 在内的标准程序库

Familiarize yourself with the standard library, including TR1.

TR1 代表 Technical Report 1 。

C++ 标准程序库的主要机能由 STL 、 iostream 、 locales 组成。并包括 C99 标准程序库。

TR1 添加了智能指针（例如 tr1::shared_ptr ）、一般化函数指针（ tr1::function ）、 hash-table 容器、正则表达式（ regular expressions ）以及另外 10 个组件的支持。

TR1 自身只是一份规范。为获得 TR1 提供的好处，需要一份实物，一个好的来源是 Boost 。

### 55 | 让自己熟悉 Boost

Familiarize yourself with Boost.

Boost 是一个社群，也是一个网站。致力于免费、源码开放、同僚复审的 C++ 程序库开发。 Boost 在 C++ 标准化过程中扮演深具影响力的角色。

Boost 提供许多 TR1 组件实现品，以及其他许多程序库。
