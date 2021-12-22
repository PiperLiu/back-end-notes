# MySQL 实战 45 讲

https://time.geekbang.org/column/intro/100020801

> 林晓斌，网名“丁奇”，腾讯云数据库负责人，前阿里 P9 技术专家，曾负责阿里云 RDS 内核开发团队和运维团队，并推动了 AliSQL 分支开源。作为活跃的 MySQL 社区贡献者，丁奇专注于数据存储系统、MySQL 源码研究和改进、MySQL 性能优化和功能改进，热衷于解决 MySQL 疑难问题。


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [开篇词 (1讲)](#开篇词-1讲)
- [基础篇 (8讲)](#基础篇-8讲)
  - [01 | 基础架构：一条SQL查询语句是如何执行的？](#01-基础架构一条sql查询语句是如何执行的)
  - [02 | 日志系统：一条SQL更新语句是如何执行的？](#02-日志系统一条sql更新语句是如何执行的)

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
