# 第 17 章 集群

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [17.1 节点](#171-节点)
  - [17.1.1 启动节点](#1711-启动节点)
  - [17.1.2 集群数据结构](#1712-集群数据结构)
  - [17.1.3 CLUSTER MEET命令的实现](#1713-cluster-meet命令的实现)
- [17.2 槽指派](#172-槽指派)
  - [17.2.1 纪录节点的槽指派信息](#1721-纪录节点的槽指派信息)
  - [17.2.2 传播节点的槽指派信息](#1722-传播节点的槽指派信息)
  - [17.2.3 纪录集群所有槽的指派信息](#1723-纪录集群所有槽的指派信息)
  - [17.2.4 CLUSTER ADDSLOTS命令的实现](#1724-cluster-addslots命令的实现)
- [17.3 在集群中执行命令](#173-在集群中执行命令)
  - [17.3.1 计算键属于哪个槽](#1731-计算键属于哪个槽)
  - [17.3.2 判断槽是否由当前节点负责处理](#1732-判断槽是否由当前节点负责处理)
  - [17.3.3 MOVED错误](#1733-moved错误)
  - [17.3.4 节点数据库的实现](#1734-节点数据库的实现)
- [17.4 重新分片](#174-重新分片)
- [17.5 ASK错误](#175-ask错误)
  - [17.5.1 CLUSTER SETSLOT IMPORTING命令的实现](#1751-cluster-setslot-importing命令的实现)
  - [17.5.2 CLUSTER SETSLOT MIGRATING命令的实现](#1752-cluster-setslot-migrating命令的实现)
  - [17.5.3 ASK错误](#1753-ask错误)
  - [17.5.4 ASKING命令](#1754-asking命令)
  - [17.5.5 ASK错误和MOVED错误的区别](#1755-ask错误和moved错误的区别)
- [17.6 复制与故障转移](#176-复制与故障转移)
  - [17.6.1 设置从节点](#1761-设置从节点)
  - [17.6.2 故障检测](#1762-故障检测)
  - [17.6.3 故障转移](#1763-故障转移)
  - [17.6.4 选举新的主节点](#1764-选举新的主节点)
- [17.7 消息](#177-消息)
  - [17.7.1 消息头](#1771-消息头)
  - [17.7.2 MEET、PING、PONG消息的实现](#1772-meet-ping-pong消息的实现)
  - [17.7.3 FAIL消息的实现](#1773-fail消息的实现)
  - [17.7.4 PUBLISH消息的实现](#1774-publish消息的实现)

<!-- /code_chunk_output -->

分布式数据库方案。

### 17.1 节点

```bash
127.0.0.1:7000> CLUSTER MEET 127.0.0.1 7001
```

如上将节点 7001 加入到集群中。

#### 17.1.1 启动节点

#### 17.1.2 集群数据结构

#### 17.1.3 CLUSTER MEET命令的实现

### 17.2 槽指派

集群的整个数据库被分为 16384 个槽，每个节点负责一部分槽，每个节点最多负责 16384 个槽，最少 0 个。

所有槽都至少由一个节点负责。

```bash
127.0.0.1:7000> CLUSTER ADDSLOTS 0-5461 10922-16383
```

如上将槽 0 到 5461 和 10922 到 16383 指派给节点 7000。

#### 17.2.1 纪录节点的槽指派信息

```c
int CLUSTER_SLOTS = 16384;

struct clusterNode {
    // ...
    unsigned char slots[CLUSTER_SLOTS/8]; // 每个字节表示 8 个槽
}
```

#### 17.2.2 传播节点的槽指派信息

#### 17.2.3 纪录集群所有槽的指派信息

```c
typedef struct clusterState {
    // ...
    struct clusterNode nodes[CLUSTER_SLOTS]; // 记录每个槽的指派信息
    // ...
} clusterState;
```

#### 17.2.4 CLUSTER ADDSLOTS命令的实现

### 17.3 在集群中执行命令

节点计算键属于哪个槽，若不是本节点，则向客户端返回 `MOVED` 错误，指引客户端到正确的节点。

#### 17.3.1 计算键属于哪个槽

```c
CRC16(key) & 16383
```

#### 17.3.2 判断槽是否由当前节点负责处理

#### 17.3.3 MOVED错误

#### 17.3.4 节点数据库的实现

### 17.4 重新分片

关键词： `redis-trib` 负责管理集群

### 17.5 ASK错误

客户端向源节点发送关于 key 的命令，若源节点正在迁移相关槽，且 key 可能处于源节点迁移的目标节点，则源节点会返回 `ASK` 错误，指引客户端到目标节点。

> 如上的 ASK 错误在 Redis 3.0 中未实现。

#### 17.5.1 CLUSTER SETSLOT IMPORTING命令的实现

#### 17.5.2 CLUSTER SETSLOT MIGRATING命令的实现

#### 17.5.3 ASK错误

#### 17.5.4 ASKING命令

#### 17.5.5 ASK错误和MOVED错误的区别

### 17.6 复制与故障转移

#### 17.6.1 设置从节点

#### 17.6.2 故障检测

#### 17.6.3 故障转移

#### 17.6.4 选举新的主节点

### 17.7 消息

#### 17.7.1 消息头

#### 17.7.2 MEET、PING、PONG消息的实现

Gossip 协议，正文实现可见 `cluster.h/clusterMsgDataGossip` 。

#### 17.7.3 FAIL消息的实现

#### 17.7.4 PUBLISH消息的实现
