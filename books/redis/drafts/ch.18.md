# 第 18 章 发布与订阅

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [18.1 频道的订阅与退订](#181-频道的订阅与退订)
  - [18.1.1 订阅频道](#1811-订阅频道)
  - [18.1.2 退订频道](#1812-退订频道)
- [18.2 模式的订阅与退订](#182-模式的订阅与退订)
  - [18.2.1 订阅模式](#1821-订阅模式)
  - [18.2.2 退订模式](#1822-退订模式)
- [18.3 发送消息](#183-发送消息)
  - [18.3.1 将消息发送给频道订阅者](#1831-将消息发送给频道订阅者)
  - [18.3.2 将消息发送给模式订阅者](#1832-将消息发送给模式订阅者)
- [18.4 查看订阅信息](#184-查看订阅信息)
  - [18.4.1 PUBSUB CHANNELS](#1841-pubsub-channels)
  - [18.4.2 PUBSUB NUMSUB](#1842-pubsub-numsub)
  - [18.4.3 PUBSUB NUMPAT](#1843-pubsub-numpat)

<!-- /code_chunk_output -->

这个 sub/pub 功能在股票行情订阅中还挺常用。

### 18.1 频道的订阅与退订

```c
struct redisServer {
    dict *pubsub_channels;  // channel -> list of subscribers
}
```

#### 18.1.1 订阅频道

#### 18.1.2 退订频道

### 18.2 模式的订阅与退订

```c
struct redisServer {
    list *pubsub_patterns;
}

typedef struct pubsubPattern {
    redisClient *client;
    robj *pattern;
} pubsubPattern;
```

#### 18.2.1 订阅模式

#### 18.2.2 退订模式

### 18.3 发送消息

#### 18.3.1 将消息发送给频道订阅者

#### 18.3.2 将消息发送给模式订阅者

对于一个频道，遍历所有模式判断是否匹配后发给相应的客户端。效率似乎不高。

### 18.4 查看订阅信息

#### 18.4.1 PUBSUB CHANNELS

#### 18.4.2 PUBSUB NUMSUB

#### 18.4.3 PUBSUB NUMPAT



