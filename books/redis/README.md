# 黄健宏. Redis 设计与实现. 机械工业出版社. 第一版

黄健宏. Redis 设计与实现. 机械工业出版社. 第一版

公司图书订购问卷我填了这本书，有必要趁机好好了解 Redis 。

![](./images/20220823.jpg)

本书官网见 [http://redisbook.com/](http://redisbook.com/) 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [第 1 章 引言](#第-1-章-引言)
- [第 2 章 简单动态字符串](#第-2-章-简单动态字符串)

<!-- /code_chunk_output -->

### 第 1 章 引言

介绍了本书各章节的内容及关系。

## 第一部分 数据结构与对象

### 第 2 章 简单动态字符串

[./drafts/ch.02.md](./drafts/ch.02.md)

- [2.1 SDS 的定义](./drafts/ch.02.md#21-sds-的定义)
- [2.2 SDS 与 C 字符串的区别](./drafts/ch.02.md#22-sds-与-c-字符串的区别)
  - [2.2.1 常数复杂度获取字符串的长度](./drafts/ch.02.md#221-常数复杂度获取字符串的长度)
  - [2.2.2 杜绝缓冲区溢出](./drafts/ch.02.md#222-杜绝缓冲区溢出)
  - [2.2.3 减少修改字符串时带来的内存重分配次数](./drafts/ch.02.md#223-减少修改字符串时带来的内存重分配次数)
  - [2.2.4 二进制安全](./drafts/ch.02.md#224-二进制安全)
  - [2.2.5 兼容部分 C 字符串函数](./drafts/ch.02.md#225-兼容部分-c-字符串函数)
- [2.3 SDS API](./drafts/ch.02.md#23-sds-api)
- [2.4 重点回顾](./drafts/ch.02.md#24-重点回顾)
- [2.5 参考资料](./drafts/ch.02.md#25-参考资料)
- [sds 源码阅读（重要）](./drafts/ch.02.md#sds-源码阅读重要)
  - [c 语言丰富的宏预处理](./drafts/ch.02.md#c-语言丰富的宏预处理)
  - [c 语言结构体中数组指针不占用 struct size](./drafts/ch.02.md#c-语言结构体中数组指针不占用-struct-size)

## 第二部分 单机数据库的实现

## 第三部分 多机数据库的实现

## 第四部分 独立功能的实现
