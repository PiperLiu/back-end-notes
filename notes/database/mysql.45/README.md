# MySQL 实战 45 讲

https://time.geekbang.org/column/intro/100020801

极客时间买的课，不贵，挺多人给他打广告。试听了一下，从基础和底层讲起的，是我想要的东西。

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
  - [11 | 怎么给字符串字段加索引？](#11-怎么给字符串字段加索引)
  - [12 | 为什么我的MySQL会“抖”一下？](#12-为什么我的mysql会抖一下)
  - [13 | 为什么表数据删掉一半，表文件大小不变？](#13-为什么表数据删掉一半表文件大小不变)
  - [14 | count(*)这么慢，我该怎么办？](#14-count这么慢我该怎么办)
  - [15 | 答疑文章（一）：日志和索引相关问题](#15-答疑文章一日志和索引相关问题)
  - [16 | “order by”是怎么工作的？](#16-order-by是怎么工作的)
  - [17 | 如何正确地显示随机消息？](#17-如何正确地显示随机消息)
  - [18 | 为什么这些SQL语句逻辑相同，性能却差异巨大？](#18-为什么这些sql语句逻辑相同性能却差异巨大)
  - [19 | 为什么我只查一行的语句，也执行这么慢？](#19-为什么我只查一行的语句也执行这么慢)

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

#### 11 | 怎么给字符串字段加索引？

[./drafts/mysql.45.02.003.md](./drafts/mysql.45.02.003.md)

- [概述：给邮箱加索引](./drafts/mysql.45.02.003.md#概述给邮箱加索引)
  - [前缀索引对比示例](./drafts/mysql.45.02.003.md#前缀索引对比示例)
  - [如何确定前缀索引长度？](./drafts/mysql.45.02.003.md#如何确定前缀索引长度)
- [前缀索引对覆盖索引的影响](./drafts/mysql.45.02.003.md#前缀索引对覆盖索引的影响)
- [其他方式：倒序存储与hash字段](./drafts/mysql.45.02.003.md#其他方式倒序存储与hash字段)
  - [倒序存储](./drafts/mysql.45.02.003.md#倒序存储)
  - [hash 字段](./drafts/mysql.45.02.003.md#hash-字段)
  - [两种方法对比](./drafts/mysql.45.02.003.md#两种方法对比)
- [小结](./drafts/mysql.45.02.003.md#小结)
- [问题：如何设计学生登入场景邮件索引？](./drafts/mysql.45.02.003.md#问题如何设计学生登入场景邮件索引)
  - [老师的分析](./drafts/mysql.45.02.003.md#老师的分析)
  - [其他同学的思路：不优化/信息论高进制优化](./drafts/mysql.45.02.003.md#其他同学的思路不优化信息论高进制优化)

#### 12 | 为什么我的MySQL会“抖”一下？

[./drafts/mysql.45.02.004.md](./drafts/mysql.45.02.004.md)

- [概述：“抖了一下”](./drafts/mysql.45.02.004.md#概述抖了一下)
- [你的 SQL 语句为什么变“慢”了](./drafts/mysql.45.02.004.md#你的-sql-语句为什么变慢了)
  - [“慢”一下可能是在 flush](./drafts/mysql.45.02.004.md#慢一下可能是在-flush)
  - [什么情况下会 flush ？](./drafts/mysql.45.02.004.md#什么情况下会-flush)
  - [flush 四种场景对性能的影响](./drafts/mysql.45.02.004.md#flush-四种场景对性能的影响)
- [InnoDB 刷脏页的控制策略](./drafts/mysql.45.02.004.md#innodb-刷脏页的控制策略)
  - [查看脏页比例 Innodb_buffer_pool_pages_dirty](./drafts/mysql.45.02.004.md#查看脏页比例-innodb_buffer_pool_pages_dirty)
  - [SSD 不适用机械硬盘时代的 flush_neighbors](./drafts/mysql.45.02.004.md#ssd-不适用机械硬盘时代的-flush_neighbors)
- [小结与问题：不小心将 redo log 设小](./drafts/mysql.45.02.004.md#小结与问题不小心将-redo-log-设小)

#### 13 | 为什么表数据删掉一半，表文件大小不变？

[./drafts/mysql.45.02.005.md](./drafts/mysql.45.02.005.md)

- [概述：如何正确回收空间？](./drafts/mysql.45.02.005.md#概述如何正确回收空间)
- [参数 innodb_file_per_table](./drafts/mysql.45.02.005.md#参数-innodb_file_per_table)
- [数据删除流程](./drafts/mysql.45.02.005.md#数据删除流程)
  - [并非“真的”删除，二是留下复用标记](./drafts/mysql.45.02.005.md#并非真的删除二是留下复用标记)
  - [留下很多“空洞”浪费空间](./drafts/mysql.45.02.005.md#留下很多空洞浪费空间)
- [重建表](./drafts/mysql.45.02.005.md#重建表)
  - [alter table A engine=InnoDB](./drafts/mysql.45.02.005.md#alter-table-a-engineinnodb)
  - [Online DDL](./drafts/mysql.45.02.005.md#online-ddl)
- [Online 和 inplace](./drafts/mysql.45.02.005.md#online-和-inplace)
- [问题：想要收缩表空间，结果适得其反](./drafts/mysql.45.02.005.md#问题想要收缩表空间结果适得其反)

#### 14 | count(*)这么慢，我该怎么办？

[./drafts/mysql.45.02.006.md](./drafts/mysql.45.02.006.md)

- [概述：count(*) 怎么这么慢？](./drafts/mysql.45.02.006.md#概述count-怎么这么慢)
- [count(*) 的实现方式](./drafts/mysql.45.02.006.md#count-的实现方式)
  - [例子：为什么不把 count 存起来？](./drafts/mysql.45.02.006.md#例子为什么不把-count-存起来)
  - [count(*) 中的优化](./drafts/mysql.45.02.006.md#count-中的优化)
- [用缓存系统保存计数](./drafts/mysql.45.02.006.md#用缓存系统保存计数)
  - [使用 Redis 似乎很直观](./drafts/mysql.45.02.006.md#使用-redis-似乎很直观)
  - [使用 Redis 导致逻辑不精确](./drafts/mysql.45.02.006.md#使用-redis-导致逻辑不精确)
- [在数据库保存计数](./drafts/mysql.45.02.006.md#在数据库保存计数)
  - [在数据库保存计数支持崩溃恢复](./drafts/mysql.45.02.006.md#在数据库保存计数支持崩溃恢复)
  - [在数据库保存计数解决计数不精确](./drafts/mysql.45.02.006.md#在数据库保存计数解决计数不精确)
- [不同的 count 用法](./drafts/mysql.45.02.006.md#不同的-count-用法)
  - [count(主键 id)](./drafts/mysql.45.02.006.md#count主键-id)
  - [count(1)](./drafts/mysql.45.02.006.md#count1)
  - [count(字段)](./drafts/mysql.45.02.006.md#count字段)
  - [count(*)](./drafts/mysql.45.02.006.md#count)
- [问题：先插入操作记录还是先更新计数表？](./drafts/mysql.45.02.006.md#问题先插入操作记录还是先更新计数表)

#### 15 | 答疑文章（一）：日志和索引相关问题

[./drafts/mysql.45.02.007.md](./drafts/mysql.45.02.007.md)

- [日志相关问题](./drafts/mysql.45.02.007.md#日志相关问题)
  - [追问 1：MySQL 怎么知道 binlog 是完整的?](./drafts/mysql.45.02.007.md#追问-1mysql-怎么知道-binlog-是完整的)
  - [追问 2：redo log 和 binlog 是怎么关联起来的?](./drafts/mysql.45.02.007.md#追问-2redo-log-和-binlog-是怎么关联起来的)
  - [追问 3：处于 prepare 阶段的 redo log 加上完整 binlog，重启就能恢复，MySQL 为什么要这么设计?](./drafts/mysql.45.02.007.md#追问-3处于-prepare-阶段的-redo-log-加上完整-binlog重启就能恢复mysql-为什么要这么设计)
  - [追问 4：如果这样的话，为什么还要两阶段提交呢？干脆先 redo log 写完，再写 binlog。崩溃恢复的时候，必须得两个日志都完整才可以。是不是一样的逻辑？](./drafts/mysql.45.02.007.md#追问-4如果这样的话为什么还要两阶段提交呢干脆先-redo-log-写完再写-binlog崩溃恢复的时候必须得两个日志都完整才可以是不是一样的逻辑)
  - [追问 5：不引入两个日志，也就没有两阶段提交的必要了。只用 binlog 来支持崩溃恢复，又能支持归档，不就可以了？](./drafts/mysql.45.02.007.md#追问-5不引入两个日志也就没有两阶段提交的必要了只用-binlog-来支持崩溃恢复又能支持归档不就可以了)
  - [追问 6：那能不能反过来，只用 redo log，不要 binlog？](./drafts/mysql.45.02.007.md#追问-6那能不能反过来只用-redo-log不要-binlog)
  - [追问 7：redo log 一般设置多大？](./drafts/mysql.45.02.007.md#追问-7redo-log-一般设置多大)
  - [追问 8：正常运行中的实例，数据写入后的最终落盘，是从 redo log 更新过来的还是从 buffer pool 更新过来的呢？](./drafts/mysql.45.02.007.md#追问-8正常运行中的实例数据写入后的最终落盘是从-redo-log-更新过来的还是从-buffer-pool-更新过来的呢)
  - [追问 9：redo log buffer 是什么？是先修改内存，还是先写 redo log 文件？](./drafts/mysql.45.02.007.md#追问-9redo-log-buffer-是什么是先修改内存还是先写-redo-log-文件)
- [业务设计问题](./drafts/mysql.45.02.007.md#业务设计问题)
- [问题：当 MySQL 去更新一行，但是要修改的值跟原来的值是相同的，这时候 MySQL 会真的去执行一次修改吗？还是看到值相同就直接返回呢？](./drafts/mysql.45.02.007.md#问题当-mysql-去更新一行但是要修改的值跟原来的值是相同的这时候-mysql-会真的去执行一次修改吗还是看到值相同就直接返回呢)

#### 16 | “order by”是怎么工作的？

[./drafts/mysql.45.02.008.md](./drafts/mysql.45.02.008.md)

- [概述：字段排序需求](./drafts/mysql.45.02.008.md#概述字段排序需求)
- [全字段排序](./drafts/mysql.45.02.008.md#全字段排序)
  - [sort_buffer_size 与是否使用外部排序](./drafts/mysql.45.02.008.md#sort_buffer_size-与是否使用外部排序)
- [rowid 排序](./drafts/mysql.45.02.008.md#rowid-排序)
- [全字段排序 VS rowid 排序](./drafts/mysql.45.02.008.md#全字段排序-vs-rowid-排序)
- [问题：where city in ('杭州',"苏州") order by name 如何设计？](./drafts/mysql.45.02.008.md#问题where-city-in-杭州苏州-order-by-name-如何设计)

#### 17 | 如何正确地显示随机消息？

[./drafts/mysql.45.02.009.md](./drafts/mysql.45.02.009.md)

- [概述：单词表变大，随机选择变慢怎么办？](./drafts/mysql.45.02.009.md#概述单词表变大随机选择变慢怎么办)
- [内存临时表](./drafts/mysql.45.02.009.md#内存临时表)
  - [MySQL 用什么绝对一行数据](./drafts/mysql.45.02.009.md#mysql-用什么绝对一行数据)
- [磁盘临时表](./drafts/mysql.45.02.009.md#磁盘临时表)
  - [优先队列算法](./drafts/mysql.45.02.009.md#优先队列算法)
- [随机排序方法](./drafts/mysql.45.02.009.md#随机排序方法)
- [问题：如何继续优化？](./drafts/mysql.45.02.009.md#问题如何继续优化)

#### 18 | 为什么这些SQL语句逻辑相同，性能却差异巨大？

[./drafts/mysql.45.02.010.md](./drafts/mysql.45.02.010.md)

- [案例一：条件字段函数操作](./drafts/mysql.45.02.010.md#案例一条件字段函数操作)
- [案例二：隐式类型转换](./drafts/mysql.45.02.010.md#案例二隐式类型转换)
- [案例三：隐式字符编码转换](./drafts/mysql.45.02.010.md#案例三隐式字符编码转换)
- [小结与其他场景：无效查找中大量回表](./drafts/mysql.45.02.010.md#小结与其他场景无效查找中大量回表)

#### 19 | 为什么我只查一行的语句，也执行这么慢？

[./drafts/mysql.45.02.011.md](./drafts/mysql.45.02.011.md)

- [概述](./drafts/mysql.45.02.011.md#概述)
- [第一类：查询长时间不返回](./drafts/mysql.45.02.011.md#第一类查询长时间不返回)
  - [等 MDL 锁](./drafts/mysql.45.02.011.md#等-mdl-锁)
  - [等 flush](./drafts/mysql.45.02.011.md#等-flush)
  - [等行锁](./drafts/mysql.45.02.011.md#等行锁)
- [第二类：查询慢](./drafts/mysql.45.02.011.md#第二类查询慢)
- [问题：没有索引的语句什么时候加的锁？](./drafts/mysql.45.02.011.md#问题没有索引的语句什么时候加的锁)
