# 第 5 章 跳表

Redis 自己实现了哈希表。代码见 src/server.h ，在 3.0 版本中还在 redis.h 中。
- sorted set API 实现： src/t_zset.c

跳表的算法实现以及总结可以见我的算法笔记 https://github.com/PiperLiu/ACMOI_Journey/blob/master/etc/drafts/20220921lc.md
- 实际上就是加上概率的平衡二叉树？只不过不需要加入节点自平衡的过程
- 平衡树可以保证 $O(log_2 N)$ （在理论上）；而跳表因为是靠概率添加的 level ，所以理论上无法做保证，但是其优势可以概括为：实际上实操简单、可以范围查找

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [5.1 跳跃表的实现](#51-跳跃表的实现)
  - [5.1.1 跳跃表节点](#511-跳跃表节点)
  - [5.1.2 跳跃表](#512-跳跃表)
- [5.2 跳跃表 API](#52-跳跃表-api)
- [5.3 重点回顾](#53-重点回顾)

<!-- /code_chunk_output -->

### 5.1 跳跃表的实现

跳表结构：
- redis.h `zskiplistNode`
- redis.h `zskiplist`

#### 5.1.1 跳跃表节点

#### 5.1.2 跳跃表

### 5.2 跳跃表 API

### 5.3 重点回顾
