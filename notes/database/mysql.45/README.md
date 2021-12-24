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
