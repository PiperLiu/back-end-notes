# MySQL 实战 45 讲

https://time.geekbang.org/column/intro/100020801

> 林晓斌，网名“丁奇”，腾讯云数据库负责人，前阿里 P9 技术专家，曾负责阿里云 RDS 内核开发团队和运维团队，并推动了 AliSQL 分支开源。作为活跃的 MySQL 社区贡献者，丁奇专注于数据存储系统、MySQL 源码研究和改进、MySQL 性能优化和功能改进，热衷于解决 MySQL 疑难问题。


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [开篇词 (1讲)](#开篇词-1讲)
- [基础篇 (8讲)](#基础篇-8讲)
  - [01 | 基础架构：一条SQL查询语句是如何执行的？](#01-基础架构一条sql查询语句是如何执行的)
  - [02 | 日志系统：一条SQL更新语句是如何执行的？](#02-日志系统一条sql更新语句是如何执行的)
  - [03 | 事务隔离：为什么你改了我还看不见？](#03-事务隔离为什么你改了我还看不见)
  - [04 | 深入浅出索引（上）](#04-深入浅出索引上)
  - [05 | 深入浅出索引（下）](#05-深入浅出索引下)
  - [06 | 全局锁和表锁 ：给表加个字段怎么有这么多阻碍？](#06-全局锁和表锁-给表加个字段怎么有这么多阻碍)
  - [07 | 行锁功过：怎么减少行锁对性能的影响？](#07-行锁功过怎么减少行锁对性能的影响)
  - [08 | 事务到底是隔离的还是不隔离的？](#08-事务到底是隔离的还是不隔离的)
- [实践篇 (37讲)](#实践篇-37讲)
  - [09 | 普通索引和唯一索引，应该怎么选择？](#09-普通索引和唯一索引应该怎么选择)
  - [10 | MySQL为什么有时候会选错索引？](#10-mysql为什么有时候会选错索引)

<!-- /code_chunk_output -->

### 开篇词 (1讲)

老师讲了他的实践经历以及了解底层原理的重要性，引出本课程要讲解内容：
- 为什么某些操作是“最佳实践”？
- 运行一条语句，背后会发生什么？
- 我们是把操作交给数据库系统还是交给缓存好呢？

### 基础篇 (8讲)

#### 01 | 基础架构：一条SQL查询语句是如何执行的？

[./drafts/mysql.45.01.001.md](./drafts/mysql.45.01.001.md)

- [MySQL 基本架构](./drafts/mysql.45.01.001.md#mysql-基本架构)
- [连接器](./drafts/mysql.45.01.001.md#连接器)
  - [长连接与短连接](./drafts/mysql.45.01.001.md#长连接与短连接)
  - [长连接积压导致OOM](./drafts/mysql.45.01.001.md#长连接积压导致oom)
- [查询换存](./drafts/mysql.45.01.001.md#查询换存)
- [分析器](./drafts/mysql.45.01.001.md#分析器)
- [优化器](./drafts/mysql.45.01.001.md#优化器)
- [执行器](./drafts/mysql.45.01.001.md#执行器)
- [问题：不存在列在哪个阶段检查](./drafts/mysql.45.01.001.md#问题不存在列在哪个阶段检查)

#### 02 | 日志系统：一条SQL更新语句是如何执行的？

[./drafts/mysql.45.01.002.md](./drafts/mysql.45.01.002.md)

- [概述](./drafts/mysql.45.01.002.md#概述)
- [redo log（InnoDB特有）](./drafts/mysql.45.01.002.md#redo-loginnodb特有)
- [重要的日志模块：binlog](./drafts/mysql.45.01.002.md#重要的日志模块binlog)
- [InnoDB 中 update 内部流程](./drafts/mysql.45.01.002.md#innodb-中-update-内部流程)
- [两阶段提交](./drafts/mysql.45.01.002.md#两阶段提交)
- [参数：innodb_flush_log_at_trx_commit 和 sync_binlog](./drafts/mysql.45.01.002.md#参数innodb_flush_log_at_trx_commit-和-sync_binlog)

#### 03 | 事务隔离：为什么你改了我还看不见？

[./drafts/mysql.45.01.003.md](./drafts/mysql.45.01.003.md)

- [什么是事务](./drafts/mysql.45.01.003.md#什么是事务)
- [隔离性与隔离级别](./drafts/mysql.45.01.003.md#隔离性与隔离级别)
  - [隔离级别示例](./drafts/mysql.45.01.003.md#隔离级别示例)
  - [视图](./drafts/mysql.45.01.003.md#视图)
  - [什么时候用可重复读？](./drafts/mysql.45.01.003.md#什么时候用可重复读)
- [事务隔离的实现](./drafts/mysql.45.01.003.md#事务隔离的实现)
- [事务的启动方式（应对长事务）](./drafts/mysql.45.01.003.md#事务的启动方式应对长事务)

#### 04 | 深入浅出索引（上）

[./drafts/mysql.45.01.004.md](./drafts/mysql.45.01.004.md)

- [概述](./drafts/mysql.45.01.004.md#概述)
- [索引的常见模型](./drafts/mysql.45.01.004.md#索引的常见模型)
  - [哈希表](./drafts/mysql.45.01.004.md#哈希表)
  - [有序数组](./drafts/mysql.45.01.004.md#有序数组)
  - [（平衡）搜索树](./drafts/mysql.45.01.004.md#平衡搜索树)
- [InnoDB 的索引模型](./drafts/mysql.45.01.004.md#innodb-的索引模型)
  - [B+树](./drafts/mysql.45.01.004.md#b树)
  - [主键索引和非主键索引](./drafts/mysql.45.01.004.md#主键索引和非主键索引)
- [索引维护](./drafts/mysql.45.01.004.md#索引维护)
- [重建索引分析](./drafts/mysql.45.01.004.md#重建索引分析)

#### 05 | 深入浅出索引（下）

[./drafts/mysql.45.01.005.md](./drafts/mysql.45.01.005.md)

- [引言：如何避免回表？](./drafts/mysql.45.01.005.md#引言如何避免回表)
- [覆盖索引](./drafts/mysql.45.01.005.md#覆盖索引)
  - [覆盖索引例子](./drafts/mysql.45.01.005.md#覆盖索引例子)
  - [覆盖索引实际场景](./drafts/mysql.45.01.005.md#覆盖索引实际场景)
- [最左前缀原则](./drafts/mysql.45.01.005.md#最左前缀原则)
  - [最左前缀原理](./drafts/mysql.45.01.005.md#最左前缀原理)
  - [如何安排索引内的字段顺序](./drafts/mysql.45.01.005.md#如何安排索引内的字段顺序)
- [索引下推](./drafts/mysql.45.01.005.md#索引下推)
- [问题：联合索引与单个索引并存的意义](./drafts/mysql.45.01.005.md#问题联合索引与单个索引并存的意义)

#### 06 | 全局锁和表锁 ：给表加个字段怎么有这么多阻碍？

[./drafts/mysql.45.01.006.md](./drafts/mysql.45.01.006.md)

- [概述](./drafts/mysql.45.01.006.md#概述)
- [全局锁](./drafts/mysql.45.01.006.md#全局锁)
  - [加全局读锁FTWRL](./drafts/mysql.45.01.006.md#加全局读锁ftwrl)
  - [为什么不用可重复读事务隔离](./drafts/mysql.45.01.006.md#为什么不用可重复读事务隔离)
  - [为什么不用readonly=true](./drafts/mysql.45.01.006.md#为什么不用readonlytrue)
- [表级锁](./drafts/mysql.45.01.006.md#表级锁)
  - [表锁](./drafts/mysql.45.01.006.md#表锁)
  - [元数据锁MDL](./drafts/mysql.45.01.006.md#元数据锁mdl)
  - [小表加字段导致库挂了例子](./drafts/mysql.45.01.006.md#小表加字段导致库挂了例子)
  - [如何安全地给小表加字段？](./drafts/mysql.45.01.006.md#如何安全地给小表加字段)
- [小结](./drafts/mysql.45.01.006.md#小结)
- [问题：备库上备份，主库DDL会发生什么](./drafts/mysql.45.01.006.md#问题备库上备份主库ddl会发生什么)

#### 07 | 行锁功过：怎么减少行锁对性能的影响？

[./drafts/mysql.45.01.007.md](./drafts/mysql.45.01.007.md)

- [概述](./drafts/mysql.45.01.007.md#概述)
- [从两阶段锁说起](./drafts/mysql.45.01.007.md#从两阶段锁说起)
  - [例子：两阶段锁协议](./drafts/mysql.45.01.007.md#例子两阶段锁协议)
  - [例子：最可能影响并发度的锁尽量往后放](./drafts/mysql.45.01.007.md#例子最可能影响并发度的锁尽量往后放)
- [死锁和死锁检测](./drafts/mysql.45.01.007.md#死锁和死锁检测)
  - [主动死锁检测](./drafts/mysql.45.01.007.md#主动死锁检测)
  - [解决由这种热点行更新导致的性能问题](./drafts/mysql.45.01.007.md#解决由这种热点行更新导致的性能问题)
- [问题：如何删除一个表前 10000 行数据](./drafts/mysql.45.01.007.md#问题如何删除一个表前-10000-行数据)

#### 08 | 事务到底是隔离的还是不隔离的？

[./drafts/mysql.45.01.008.md](./drafts/mysql.45.01.008.md)

- [概述](./drafts/mysql.45.01.008.md#概述)
  - [例子：事务自己获取到行锁](./drafts/mysql.45.01.008.md#例子事务自己获取到行锁)
  - [视图概念](./drafts/mysql.45.01.008.md#视图概念)
- [“快照”在 MVCC 里是怎么工作的](./drafts/mysql.45.01.008.md#快照在-mvcc-里是怎么工作的)
  - [InnoDB 引擎中的事务ID transaction id](./drafts/mysql.45.01.008.md#innodb-引擎中的事务id-transaction-id)
  - [事务的一致性视图 read-view](./drafts/mysql.45.01.008.md#事务的一致性视图-read-view)
  - [解释最开头例子：事务自己获取到行锁](./drafts/mysql.45.01.008.md#解释最开头例子事务自己获取到行锁)
- [更新逻辑](./drafts/mysql.45.01.008.md#更新逻辑)
  - [当前读current read](./drafts/mysql.45.01.008.md#当前读current-read)
  - [一致性读、当前读和行锁](./drafts/mysql.45.01.008.md#一致性读-当前读和行锁)
- [小结](./drafts/mysql.45.01.008.md#小结)
- [问题：事务中“字段 c 和 id 值相等的行”无法清零](./drafts/mysql.45.01.008.md#问题事务中字段-c-和-id-值相等的行无法清零)

### 实践篇 (37讲)

#### 09 | 普通索引和唯一索引，应该怎么选择？

[./drafts/mysql.45.02.001.md](./drafts/mysql.45.01.002.md)

- [概述：选择普通索引还是唯一索引？](./drafts/mysql.45.01.002.md#概述选择普通索引还是唯一索引)
- [查询过程](./drafts/mysql.45.01.002.md#查询过程)
- [更新过程](./drafts/mysql.45.01.002.md#更新过程)
  - [change buffer](./drafts/mysql.45.01.002.md#change-buffer)
  - [什么条件下可以使用 change buffer](./drafts/mysql.45.01.002.md#什么条件下可以使用-change-buffer)
  - [坑：别把普通索引改成唯一索引](./drafts/mysql.45.01.002.md#坑别把普通索引改成唯一索引)
- [change buffer 的使用场景](./drafts/mysql.45.01.002.md#change-buffer-的使用场景)
- [索引选择和实践](./drafts/mysql.45.01.002.md#索引选择和实践)
- [change buffer 和 redo log](./drafts/mysql.45.01.002.md#change-buffer-和-redo-log)
- [业务中是否使用唯一索引](./drafts/mysql.45.01.002.md#业务中是否使用唯一索引)
- [问题：断电是否会丢失 change buffer](./drafts/mysql.45.01.002.md#问题断电是否会丢失-change-buffer)

#### 10 | MySQL为什么有时候会选错索引？

[./drafts/mysql.45.02.002.md](./drafts/mysql.45.02.002.md)

- [概述：选错索引例子（explain与慢日志的使用）](./drafts/mysql.45.02.002.md#概述选错索引例子explain与慢日志的使用)
- [优化器的逻辑](./drafts/mysql.45.02.002.md#优化器的逻辑)
  - [索引的基数 cardinality](./drafts/mysql.45.02.002.md#索引的基数-cardinality)
  - [MySQL 采样统计的方法](./drafts/mysql.45.02.002.md#mysql-采样统计的方法)
  - [扫描行数预估](./drafts/mysql.45.02.002.md#扫描行数预估)
  - [用 analyze 修正](./drafts/mysql.45.02.002.md#用-analyze-修正)
  - [相对复杂语句索引选错案例](./drafts/mysql.45.02.002.md#相对复杂语句索引选错案例)
- [索引选择异常和处理](./drafts/mysql.45.02.002.md#索引选择异常和处理)
- [问题：例子中为什么会得到错误的扫描行数](./drafts/mysql.45.02.002.md#问题例子中为什么会得到错误的扫描行数)
