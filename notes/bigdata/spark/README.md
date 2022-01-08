# Spark学习笔记

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [辅助资料](#辅助资料)
  - [参考资料](#参考资料)
  - [MapReduce](#mapreduce)
- [历史、生态概述以及工具基础](#历史-生态概述以及工具基础)
  - [大数据技术生态略写与Spark历史](#大数据技术生态略写与spark历史)
  - [scala语言思想与基础](#scala语言思想与基础)
  - [简单认识 Spark SQL 以及 Hive 、 Parquet 列式存储](#简单认识-spark-sql-以及-hive-parquet-列式存储)
  - [DataFrame/DataSet 常用操作如多种 join 及其算法](#dataframedataset-常用操作如多种-join-及其算法)
  - [Spark 自定义函数和开窗函数](#spark-自定义函数和开窗函数)
  - [Nginx访问日志分析完整实战](#nginx访问日志分析完整实战)
- [Spark 核心内容](#spark-核心内容)
- [Spark 组件](#spark-组件)
- [Spark 经验总结](#spark-经验总结)
  - [新手杂记](#新手杂记)

<!-- /code_chunk_output -->

学习 Spark 是建立这个项目的契机：[准备开始系统学习后端：以 Spark 项目为契机](../../journey/drafts/20210625spark.md)

### 辅助资料

#### 参考资料
[./drafts/0001参考资料.md](./drafts/0001参考资料.md)

- [阅读书目](./drafts/0001参考资料.md#阅读书目)
- [经典论文](./drafts/0001参考资料.md#经典论文)
- [课程学习](./drafts/0001参考资料.md#课程学习)

#### MapReduce
[./drafts/0002MapReduce.md](./drafts/0002MapReduce.md)

- [MapReduce 思想](./drafts/0002MapReduce.md#mapreduce-思想)
- [MapReduce 实例](./drafts/0002MapReduce.md#mapreduce-实例)
  - [词频统计和倒排索引](./drafts/0002MapReduce.md#词频统计和倒排索引)

### 历史、生态概述以及工具基础

#### 大数据技术生态略写与Spark历史
[./drafts/0101大数据技术生态略写与Spark历史.md](./drafts/0101大数据技术生态略写与Spark历史.md)

- [从『脚本 + MySQL』到『Hadoop + Hive』](./drafts/0101大数据技术生态略写与Spark历史.md#从脚本-mysql到hadoop-hive)
- [比 Hadoop 更快的 Spark](./drafts/0101大数据技术生态略写与Spark历史.md#比-hadoop-更快的-spark)
- [为何 Spark 会比 Hadoop 快？](./drafts/0101大数据技术生态略写与Spark历史.md#为何-spark-会比-hadoop-快)
- [为什么大数据领域技术大多基于 Java 而非 C++ ？](./drafts/0101大数据技术生态略写与Spark历史.md#为什么大数据领域技术大多基于-java-而非-c)
  - [解释一下：宁可移动计算，不要移动数据](./drafts/0101大数据技术生态略写与Spark历史.md#解释一下宁可移动计算不要移动数据)
  - [简单聊一下 Java 序列化](./drafts/0101大数据技术生态略写与Spark历史.md#简单聊一下-java-序列化)
- [Spark 简单历史](./drafts/0101大数据技术生态略写与Spark历史.md#spark-简单历史)
- [Spark 与大数据生态](./drafts/0101大数据技术生态略写与Spark历史.md#spark-与大数据生态)

#### scala语言思想与基础
[./drafts/0102scala语言思想与基础.md](./drafts/0102scala语言思想与基础.md)

- [Scala 基本思想与注意事项](./drafts/0102scala语言思想与基础.md#scala-基本思想与注意事项)
  - [注意事项速查！](./drafts/0102scala语言思想与基础.md#注意事项速查)
- [最基本的语法示例](./drafts/0102scala语言思想与基础.md#最基本的语法示例)
  - [类型的声明、控制结构（for、模式匹配、case）](./drafts/0102scala语言思想与基础.md#类型的声明-控制结构for-模式匹配-case)
  - [Scala 语法独特的地方](./drafts/0102scala语言思想与基础.md#scala-语法独特的地方)
  - [Scala 的面向对象与一等公民“函数”](./drafts/0102scala语言思想与基础.md#scala-的面向对象与一等公民函数)
- [基本数据结构](./drafts/0102scala语言思想与基础.md#基本数据结构)
  - [注意事项与 Scala 奇技淫巧](./drafts/0102scala语言思想与基础.md#注意事项与-scala-奇技淫巧)
  - [关于数据结构的讨论（List or Array?）](./drafts/0102scala语言思想与基础.md#关于数据结构的讨论list-or-array)

#### 简单认识 Spark SQL 以及 Hive 、 Parquet 列式存储

[./drafts/0201sparksql_hive_parquet.md](./drafts/0201sparksql_hive_parquet.md)

- [Spark SQL 与 Hive](./drafts/0201sparksql_hive_parquet.md#spark-sql-与-hive)
  - [Spark SQL 是什么](./drafts/0201sparksql_hive_parquet.md#spark-sql-是什么)
  - [Spark SQL 的入口以及支持 Hive](./drafts/0201sparksql_hive_parquet.md#spark-sql-的入口以及支持-hive)
  - [Spark SQL 与 Hive 区别](./drafts/0201sparksql_hive_parquet.md#spark-sql-与-hive-区别)
- [Spark SQL 运行原理](./drafts/0201sparksql_hive_parquet.md#spark-sql-运行原理)
  - [逻辑计划](./drafts/0201sparksql_hive_parquet.md#逻辑计划)
  - [物理计划](./drafts/0201sparksql_hive_parquet.md#物理计划)
  - [Catalyst 优化器](./drafts/0201sparksql_hive_parquet.md#catalyst-优化器)
- [DataFrame 与 DataSet](./drafts/0201sparksql_hive_parquet.md#dataframe-与-dataset)
  - [RDD 、 DataFrame 与 DataSet](./drafts/0201sparksql_hive_parquet.md#rdd-dataframe-与-dataset)
  - [实例：创建 DataFrame 与 DataSet](./drafts/0201sparksql_hive_parquet.md#实例创建-dataframe-与-dataset)
- [Parquet 列式存储](./drafts/0201sparksql_hive_parquet.md#parquet-列式存储)

#### DataFrame/DataSet 常用操作如多种 join 及其算法

[./drafts/0202sparksql_join.md](./drafts/0202sparksql_join.md)

- [查找和过滤](./drafts/0202sparksql_join.md#查找和过滤)
  - [select 与 col](./drafts/0202sparksql_join.md#select-与-col)
  - [条件查询（注意col使用===与=!=）以及逻辑运算方法](./drafts/0202sparksql_join.md#条件查询注意col使用与以及逻辑运算方法)
- [聚合操作： groupby 和 agg](./drafts/0202sparksql_join.md#聚合操作-groupby-和-agg)
- [列的增删与空值处理](./drafts/0202sparksql_join.md#列的增删与空值处理)
  - [新增自增序号列 monotonically_increasing_id 等](./drafts/0202sparksql_join.md#新增自增序号列-monotonically_increasing_id-等)
  - [行转列，透视 pivot ，空值判断 isNull](./drafts/0202sparksql_join.md#行转列透视-pivot-空值判断-isnull)
- [多表操作 Join](./drafts/0202sparksql_join.md#多表操作-join)
  - [Hash join 算法](./drafts/0202sparksql_join.md#hash-join-算法)
  - [Broadcast Hash Join](./drafts/0202sparksql_join.md#broadcast-hash-join)
  - [Shuffle Hash Join](./drafts/0202sparksql_join.md#shuffle-hash-join)
  - [Sort Merge Join](./drafts/0202sparksql_join.md#sort-merge-join)
  - [Spark SQL 支持的 Join](./drafts/0202sparksql_join.md#spark-sql-支持的-join)

#### Spark 自定义函数和开窗函数

#### Nginx访问日志分析完整实战

### Spark 核心内容

### Spark 组件



### Spark 经验总结

#### 新手杂记

下面这个是我比较新手时写的，没太大参考意义。

[./drafts/0103关于Spark工作日志.md](./drafts/0103关于Spark工作日志.md)

- [数据『读写』以及『分列』操作](./drafts/0103关于Spark工作日志.md#数据读写以及分列操作)
  - [『分列』](./drafts/0103关于Spark工作日志.md#分列)
  - [读写](./drafts/0103关于Spark工作日志.md#读写)
- [一些技巧](./drafts/0103关于Spark工作日志.md#一些技巧)
  - [.zipWithIndex.foreach](./drafts/0103关于Spark工作日志.md#zipwithindexforeach)
- [一些疑惑](./drafts/0103关于Spark工作日志.md#一些疑惑)
  - [如何从 DataFrame 中取出具体某一行！](./drafts/0103关于Spark工作日志.md#如何从-dataframe-中取出具体某一行)
