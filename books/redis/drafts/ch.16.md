# 第 16 章 Sentinel

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [16.1 启动并初始化 Sentinel](#161-启动并初始化-sentinel)
  - [16.1.1 初始化服务器](#1611-初始化服务器)
  - [16.1.2 使用 Sentinel 专用代码](#1612-使用-sentinel-专用代码)
  - [16.1.3 初始化 Sentinel 状态](#1613-初始化-sentinel-状态)
  - [16.1.4 初始化 Sentinel 状态的 masters 属性](#1614-初始化-sentinel-状态的-masters-属性)
  - [16.1.5 创建连向主服务器的网络连接](#1615-创建连向主服务器的网络连接)
- [16.2 获取主服务器信息](#162-获取主服务器信息)
- [16.3 获取从服务器信息](#163-获取从服务器信息)
- [16.4 向主服务器和从服务器发送信息](#164-向主服务器和从服务器发送信息)
- [16.5 接收来自主服务器和从服务器的频道信息](#165-接收来自主服务器和从服务器的频道信息)
  - [16.5.1 更新sentinels字典](#1651-更新sentinels字典)
  - [16.5.2 创建连向其他Sentinel的命令连接](#1652-创建连向其他sentinel的命令连接)
- [16.6 检测主观下线状态](#166-检测主观下线状态)
- [16.7 检测客观下线状态](#167-检测客观下线状态)
  - [16.7.1 发送SENTINEL is-master-down-by-addr命令](#1671-发送sentinel-is-master-down-by-addr命令)
  - [16.7.2 接收SENTINEL is-master-down-by-addr命令](#1672-接收sentinel-is-master-down-by-addr命令)
  - [16.7.3 接收SENTINEL is-master-down-by-addr命令的回复](#1673-接收sentinel-is-master-down-by-addr命令的回复)
- [16.8 选举领头Sentinel](#168-选举领头sentinel)
- [16.9 故障转移](#169-故障转移)
  - [16.9.1 选出新的主服务器](#1691-选出新的主服务器)
  - [16.9.2 修改从服务器的复制目标](#1692-修改从服务器的复制目标)
  - [16.9.3 将旧的主服务器变为从服务器](#1693-将旧的主服务器变为从服务器)

<!-- /code_chunk_output -->

Sentinel 是高可用解决方案， Sentinel 哨兵实例组成系统，监听任意多个主服务器，以及这些主服务器属下的所有从服务器。当主服务器出现故障时， Sentinel 会自动将其中一个从服务器升级为新的主服务器，并通知应用程序。

### 16.1 启动并初始化 Sentinel

```bash
redis-sentinel /path/to/sentinel.conf
# or
redis-server /path/to/sentinel.conf --sentinel
```

#### 16.1.1 初始化服务器

Sentinel 本质上是一个 redis 服务器。

#### 16.1.2 使用 Sentinel 专用代码

#### 16.1.3 初始化 Sentinel 状态

#### 16.1.4 初始化 Sentinel 状态的 masters 属性

#### 16.1.5 创建连向主服务器的网络连接

### 16.2 获取主服务器信息

### 16.3 获取从服务器信息

### 16.4 向主服务器和从服务器发送信息

### 16.5 接收来自主服务器和从服务器的频道信息

#### 16.5.1 更新sentinels字典

#### 16.5.2 创建连向其他Sentinel的命令连接

### 16.6 检测主观下线状态

默认每秒一次发送 PING 命令。

`down-after-milliseconds` 配置项指定了 Sentinel 认为主服务器已下线的时长，比如将其配置为 50000 毫秒，表示 Sentinel 会在 50 秒内没有收到主服务器的有效回复时，将其标记为主服务器已下线。

### 16.7 检测客观下线状态

Sentinel 确认某个主服务器主观下线时，会向其他 Sentinel 询问，是否有足够数量的 Sentinel 同意其主观下线。

#### 16.7.1 发送SENTINEL is-master-down-by-addr命令

#### 16.7.2 接收SENTINEL is-master-down-by-addr命令

#### 16.7.3 接收SENTINEL is-master-down-by-addr命令的回复

### 16.8 选举领头Sentinel

### 16.9 故障转移

#### 16.9.1 选出新的主服务器

基于 Raft 实现。这里略过学习。具体可参考 MIT 6.824 课程。

#### 16.9.2 修改从服务器的复制目标

#### 16.9.3 将旧的主服务器变为从服务器


