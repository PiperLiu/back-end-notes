# 第 19 章 事务

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [19.1 事务的实现](#191-事务的实现)
  - [19.1.1 事务开始](#1911-事务开始)
  - [19.1.2 命令入队](#1912-命令入队)
  - [19.1.3 事务队列](#1913-事务队列)
  - [19.1.4 执行事务](#1914-执行事务)
- [19.2 WATCH 命令的实现](#192-watch-命令的实现)
  - [19.2.1 使用 WATCH 命令监视数据库键](#1921-使用-watch-命令监视数据库键)
  - [19.2.2 监视机制的触发](#1922-监视机制的触发)
  - [19.2.3 判断事务是否安全](#1923-判断事务是否安全)
  - [19.2.4 一个完整的 WATCH 事务执行过程](#1924-一个完整的-watch-事务执行过程)
- [19.3 事务的 ACID 性质](#193-事务的-acid-性质)
  - [19.3.1 原子性](#1931-原子性)
  - [19.3.2 一致性](#1932-一致性)
  - [19.3.3 隔离性](#1933-隔离性)
  - [19.3.4 耐久性](#1934-耐久性)

<!-- /code_chunk_output -->

感觉 Redis3.0 的事务 transaction 不是标准的数据库事务。

```bash
redis > MULTI
OK

redis > SET "name" "Practical Common Lisp"
QUEUED

redis > GET "name"
QUEUED

...

redis > EXEC
1) OK
2) "Practical Common Lisp"
...
```

### 19.1 事务的实现

#### 19.1.1 事务开始

#### 19.1.2 命令入队

#### 19.1.3 事务队列

#### 19.1.4 执行事务

EXEC 命令执行过程大概如下：
- 挨个执行命令（各个命令执行结果入队）
- `client.flags &= ~REDIS_MULTI` 取消客户端的事务状态
- 清零入队命令计数器 / 释放事务队列
- 返回执行结果的队列

### 19.2 WATCH 命令的实现

WATCH 是一个乐观锁，在 EXEC 执行命令队列前，检测被操作的键是否有过修改。

注意， WATCH 是主动的，并不是说输入 MULTI 开始编写事务命令时，所有涉及到的键就都自动被 WATCH 了。使用方法如下。

```bash
redis > WATCH "key1" "key2"
redis > MULTI
redis > SET "key1" "value1"
其他客户端 > SET "key1" "value2"  # 其他客户端修改 key1 值
redis > EXEC  # 失败
```

#### 19.2.1 使用 WATCH 命令监视数据库键

#### 19.2.2 监视机制的触发

```
def touchWatch(db, key):
    if key in db.watched_keys:
        for client in db.watched_keys[key]:
            client.flags |= REDIS_DIRTY_CAS
```

#### 19.2.3 判断事务是否安全

#### 19.2.4 一个完整的 WATCH 事务执行过程

### 19.3 事务的 ACID 性质

#### 19.3.1 原子性

#### 19.3.2 一致性

1. 入队错误：如果入队命令是错的，那么整个事务都不会执行
2. 执行错误：执行时才发现的错误，事务不会回滚，而是继续执行
3. 服务器停机：根据持久化模式 RDB/AOF 帮助恢复

#### 19.3.3 隔离性

各个事务之间不会相互影响？
- Redis 用单线程保证这一点

#### 19.3.4 耐久性

事务没有额外的持久化功能，需要持久化机制来帮助。
