# 第 7 章 压缩列表

也是连续的内存，也是为了节约内存而开发的顺序型数据结构，但是每个 entry （节点）中保存了如下信息，似乎 entry 的大小更加灵活：
```c
typedef struct zlentry {
    unsigned int prevrawlensize, prevrawlen;
    unsigned int lensize, len;
    unsigned int headersize;
    unsigned char encoding;
    unsigned char *p;
} zlentry;
```

“用作列表键和哈希键的底层实现之一”。每个节点可以用来存储字节数组或者整数值。

**对比前几节的数据结构，以及链表、双链表，难免要有“既生瑜，何生亮”的疑问？**
- 相比我们常见的链表、双链表；如此设计压缩列表/整数集合有什么意义呢？
  - 前两者和后两者插入的时间复杂度都是 O(N) ，但是压缩链表可能达到 O(N^2) ，且后两者要做更多的移动内存操作
  - 后者内存排布更加紧密，而链表非常容易产生内存碎片
  - 对于插入不频繁的场景，保持内存的干净、增加一些编码难度似乎是更好的选择

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [7.1 压缩列表的构成](#71-压缩列表的构成)
- [7.2 压缩列表节点的构成](#72-压缩列表节点的构成)
  - [7.2.1 previous_entry_length](#721-previous_entry_length)
  - [7.2.2 encoding](#722-encoding)
  - [7.2.3 content](#723-content)
- [7.3 连锁更新](#73-连锁更新)
- [7.4 压缩列表 API](#74-压缩列表-api)

<!-- /code_chunk_output -->

### 7.1 压缩列表的构成

### 7.2 压缩列表节点的构成

#### 7.2.1 previous_entry_length

#### 7.2.2 encoding

#### 7.2.3 content

### 7.3 连锁更新

### 7.4 压缩列表 API
