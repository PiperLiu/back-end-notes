# 第 4 章 字典

Redis 自己实现了哈希表。代码见 https://github.com/redis/redis/blob/unstable/src/dict.h

书中让我觉得有趣的地方在于「在线 `rehash` 」， **是否线程安全** 以及 **如何加锁** 是我想通过观察源码学习的，书中没提。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [4.1 字典的实现](#41-字典的实现)
- [4.2 哈希算法](#42-哈希算法)
- [4.3 解决键冲突](#43-解决键冲突)
- [4.4 rehash](#44-rehash)
- [4.5 渐进式 rehash](#45-渐进式-rehash)
- [阅读源码（C语言中的“多态”）](#阅读源码c语言中的多态)

<!-- /code_chunk_output -->

### 4.1 字典的实现

```c
typedef struct dictEntry {
    void *key;
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct dictEntry *next;     /* Next entry in the same hash bucket. */
    void *metadata[];           /* An arbitrary number of bytes (starting at a
                                 * pointer-aligned address) of size as returned
                                 * by dictType's dictEntryMetadataBytes(). */
} dictEntry;

typedef struct dict dict;

typedef struct dictType {
    uint64_t (*hashFunction)(const void *key);
    void *(*keyDup)(dict *d, const void *key);
    void *(*valDup)(dict *d, const void *obj);
    int (*keyCompare)(dict *d, const void *key1, const void *key2);
    void (*keyDestructor)(dict *d, void *key);
    void (*valDestructor)(dict *d, void *obj);
    int (*expandAllowed)(size_t moreMem, double usedRatio);
    /* Allow a dictEntry to carry extra caller-defined metadata.  The
     * extra memory is initialized to 0 when a dictEntry is allocated. */
    size_t (*dictEntryMetadataBytes)(dict *d);
} dictType;

#define DICTHT_SIZE(exp) ((exp) == -1 ? 0 : (unsigned long)1<<(exp))
#define DICTHT_SIZE_MASK(exp) ((exp) == -1 ? 0 : (DICTHT_SIZE(exp))-1)

struct dict {
    dictType *type;

    // c 语言读的少，可能有些反直觉
    // 可以理解为 var ht_table [2][]dictEntry
    // ht_table[0] 的 length 就是哈希表的键的数量
    dictEntry **ht_table[2];
    unsigned long ht_used[2];

    long rehashidx; /* rehashing not in progress if rehashidx == -1 */

    /* Keep small vars at end for optimal (minimal) struct padding */
    int16_t pauserehash; /* If >0 rehashing is paused (<0 indicates coding error) */
    signed char ht_size_exp[2]; /* exponent of size. (size = 1<<exp) */
};

/* If safe is set to 1 this is a safe iterator, that means, you can call
 * dictAdd, dictFind, and other functions against the dictionary even while
 * iterating. Otherwise it is a non safe iterator, and only dictNext()
 * should be called while iterating. */
typedef struct dictIterator {
    dict *d;
    long index;
    int table, safe;
    dictEntry *entry, *nextEntry;
    /* unsafe iterator fingerprint for misuse detection. */
    unsigned long long fingerprint;
} dictIterator;
```

### 4.2 哈希算法

Redis 3.0 采用的 MurmurHash2 ： https://github.com/aappleby/smhasher

### 4.3 解决键冲突

可以理解为拉链法。

### 4.4 rehash

这里会在负载因子（ `load_factor = ht[0].used / ht[0].size` ）在特定情况下达到一定数值时自动进行扩容或收缩。

### 4.5 渐进式 rehash

![](../images/20221108.jpg)

主要在于 `rehashidx` 设置得比较巧妙，记录了两类信息：
- `rehashidx == -1` 表示目前没有在进行 rehash ，可以安全读写 `ht[0]`
- `rehashidx == x (x >= 0)` 表示目前 `0..x` 都已经转移到新开辟的区域了，你要访问的话去 `ht[1]` ，其他的在 `ht[0]`

### 阅读源码（C语言中的“多态”）
