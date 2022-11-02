# 第 3 章 链表

Redis 自己实现了链表。代码见 https://github.com/redis/redis/blob/unstable/src/adlist.h

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [3.1 链表和链表节点的实现](#31-链表和链表节点的实现)
- [3.2 链表和链表节点的 API](#32-链表和链表节点的-api)
- [3.3 重点回顾](#33-重点回顾)

<!-- /code_chunk_output -->

### 3.1 链表和链表节点的实现

是双向的链表。

```c
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;
```

一般而言，使用 `list` 持有链表。更加方便管理。

```c
typedef struct listIter {
    listNode *next;
    int direction;
} listIter;

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);  // dup 用于复制链表节点所保存的值，因此返回类型为 void*
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;
```

注意这里的实现也是先做一个 interface 的（是多态的），而具体“成员方法”采用什么实现，还需要把具体的函数指针赋值过来，如下。

```c
#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)
```

### 3.2 链表和链表节点的 API

### 3.3 重点回顾
