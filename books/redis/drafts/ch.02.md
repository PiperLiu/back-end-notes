# 第 2 章 简单动态字符串

Redis 自己定义了一个字符串数据类型，叫做简单动态字符串（simple dynamic string, SDS）。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [2.1 SDS 的定义](#21-sds-的定义)
- [2.2 SDS 与 C 字符串的区别](#22-sds-与-c-字符串的区别)
  - [2.2.1 常数复杂度获取字符串的长度](#221-常数复杂度获取字符串的长度)
  - [2.2.2 杜绝缓冲区溢出](#222-杜绝缓冲区溢出)
  - [2.2.3 减少修改字符串时带来的内存重分配次数](#223-减少修改字符串时带来的内存重分配次数)
  - [2.2.4 二进制安全](#224-二进制安全)
  - [2.2.5 兼容部分 C 字符串函数](#225-兼容部分-c-字符串函数)
- [2.3 SDS API](#23-sds-api)
- [2.4 重点回顾](#24-重点回顾)
- [2.5 参考资料](#25-参考资料)
- [sds 源码阅读（重要）](#sds-源码阅读重要)
  - [c 语言丰富的宏预处理](#c-语言丰富的宏预处理)
  - [c 语言结构体中数组指针不占用 struct size](#c-语言结构体中数组指针不占用-struct-size)

<!-- /code_chunk_output -->

### 2.1 SDS 的定义

```c
struct sdshdr {
    int len;   // buf 中已使用的
    int free;  // 记录 buf 中未使用的字节长度
    char buf[];
};

// 可直接
printf("%s", s->buf);
```

注意，还是要保证 `\0` 结尾的，因此要有 `len + free + 1 = buf 对应区域的长度` 。

### 2.2 SDS 与 C 字符串的区别

#### 2.2.1 常数复杂度获取字符串的长度

#### 2.2.2 杜绝缓冲区溢出

有 API 提供如 `sdscat(s, "Cluster");` 。

注意这里可以动态分配内存。

#### 2.2.3 减少修改字符串时带来的内存重分配次数

不可以每次操作都重新分配空间，毕竟 redis 是数据库。有两个策略：
1. 空间预分配（这里直接有个阈值，小于 1 MB 就二倍空间（free 分配同样大的空间）；大于 1 MB 就 free 为 1 MB）
2. 惰性释放空间

#### 2.2.4 二进制安全

并不是保存字符，而是保存一系列二进制数据。

#### 2.2.5 兼容部分 C 字符串函数

比如 `strcasecmp(sds->buf, "hello world");` 。

### 2.3 SDS API

### 2.4 重点回顾

### 2.5 参考资料

### sds 源码阅读（重要）

主要是阅读：
- https://github.com/huangz1990/redis-3.0-annotated/blob/unstable/src/sds.c
- https://github.com/huangz1990/redis-3.0-annotated/blob/unstable/src/sds.h
- https://github.com/redis/redis/blob/unstable/src/sds.c
- https://github.com/redis/redis/blob/unstable/src/sds.h

先读头文件。无论是在 3.0 还是 7.0 版本中，头文件 `.h` 负责结构体声明、类型声明、函数声明，以及 inline 函数的定义。

#### c 语言丰富的宏预处理

其中 7.0 版本的代码包含了更加丰富的宏预处理，如下。

```c
// 编译器的选择
#define HI_SDS_MAX_PREALLOC (1024*1024)
#ifdef _MSC_VER  // 如果是微软编译器 ...
#define __attribute__(x)  // 则不使用 __attribute__(x)
typedef long long ssize_t;
#define SSIZE_MAX (LLONG_MAX >> 1)
#endif

// __attribute__ ((__packed__)) 结构体，按照紧凑排列的方式，占用内存
struct __attribute__ ((__packed__)) hisdshdr5 {
    unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
    char buf[];
};
struct __attribute__ ((__packed__)) hisdshdr8 {
    uint8_t len; /* used */
    uint8_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};

// 根据 flag 确定类别
static inline void hi_sdssetalloc(hisds s, size_t newlen) {
    unsigned char flags = s[-1];
    switch(flags&HI_SDS_TYPE_MASK) {
        case HI_SDS_TYPE_5:
            /* Nothing to do, this type has no total allocation info. */
            break;
        case HI_SDS_TYPE_8:
            HI_SDS_HDR(8,s)->alloc = (uint8_t)newlen;
            break;
        case HI_SDS_TYPE_16:
            HI_SDS_HDR(16,s)->alloc = (uint16_t)newlen;
            break;
        case HI_SDS_TYPE_32:
            HI_SDS_HDR(32,s)->alloc = (uint32_t)newlen;
            break;
        case HI_SDS_TYPE_64:
            HI_SDS_HDR(64,s)->alloc = (uint64_t)newlen;
            break;
    }
}
```

问：
1. 为什么要有多种数据类型？ **因为比如小字符串的 `len` 只需要 `uint8_t` 类型就好**
2. 为什么 `hisds` 是 `typedef char *hisds;` ？这样我们的元数据（字符串）可以服用所有 c 语言标准库中的函数（`string.h` http://www.gnu.org/software/libc） 此外可以通过 `hisds[-1]` 访问 `struct` 中其他数据如 `flag` 等

在声明动态字符串 sds 的过程中，通过下面的函数判断到底用哪个类型的 sds 。

```c
static inline char hi_sdsReqType(size_t string_size) {
    if (string_size < 32)
        return HI_SDS_TYPE_5;
    if (string_size < 0xff)
        return HI_SDS_TYPE_8;
    if (string_size < 0xffff)
        return HI_SDS_TYPE_16;
    if (string_size < 0xffffffff)
        return HI_SDS_TYPE_32;
    return HI_SDS_TYPE_64;
}
```

问：那我想扩大一个字符串怎么办呢？可参考 `hisds hi_sdsMakeRoomFor(hisds s, size_t addlen)` 函数，如果发现扩容后 `type` 是一致的，就增加 free 大小，并且使用 realloc 系统调用（也有可能是别的函数）；如果 `type` 需要变化，就重新生成一个 sds （malloc）。

#### c 语言结构体中数组指针不占用 struct size

```c
/*
 * 类型别名，用于指向 sdshdr 的 buf 属性
 */
typedef char *sds;

/*
 * 保存字符串对象的结构
 */
struct sdshdr {
    
    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[];
};

/*
 * 返回 sds 实际保存的字符串的长度
 *
 * T = O(1)
 */
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}
```

函数 `sdslen` 让我很困惑。

首先我们明确， `struct sdshdr` 的指针地址就应该是 `len` 的指针。除非 `sizeof(struct sdshdr)` 的值为 `2 * sizeof(int)` ，否则道理上说不通。自己写代码试了一下，果然如此。

```c
int main()
{
    printf("sizeof(struct sdshdr) %lu\n", sizeof(struct sdshdr));
    // sizeof(struct sdshdr) 8
    struct sdshdr *sh = malloc(sizeof(struct sdshdr));
    char s[10] = "ok";
    printf("sh          %p\n", sh);
    // sh          0x14b606740
    printf("&(sh->len)  %p\n", &(sh->len));
    // &(sh->len)  0x14b606740
    printf("&(sh->free) %p\n", &(sh->free));
    // &(sh->free) 0x14b606744
    printf("sh->buf     %p\n", sh->buf);
    // sh->buf     0x14b606748

    return 0;
}
```

并且，在 c 中，如果想给 struct 一个数组指针，必须将其放在元素中的最后一个。
