# 后端工程师的高阶面经

https://time.geekbang.org/column/intro/100551601

极客时间买的课。

> 邓明（大明），前 Shopee 高级工程师，Beego PMC，极客时间训练营明星讲师，主讲《初级 Go 工程师训练营》《Go 实战训练营》，曾辅导学员 2000 余人。他长期奋战在互联网一线，擅长中间件设计和实现，如 Web、ORM、微服务框架、网关、分库分表、IM 等，造高并发大流量轮子的经验非常丰富。他还是开源社区的活跃贡献者，游走在多个开源社区之间，是 Beego PMC、Apache Dubbo Committer，热衷带人参与开源。


<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [开篇词 (1讲)](#开篇词-1讲)
- [微服务架构 (10讲)](#微服务架构-10讲)
  - [01 | 服务注册与发现：AP和CP，你选哪个？](#01--服务注册与发现ap和cp你选哪个)

<!-- /code_chunk_output -->

### 开篇词 (1讲)

### 微服务架构 (10讲)

#### 01 | 服务注册与发现：AP和CP，你选哪个？

[./drafts/01.microservices.01.md)](./drafts/01.microservices.01.md)

- [前置知识](./drafts/01.microservices.01.md#前置知识)
- [面试准备](./drafts/01.microservices.01.md#面试准备)
- [基本模型](./drafts/01.microservices.01.md#基本模型)
- [高可用](./drafts/01.microservices.01.md#高可用)
  - [服务端崩溃检测](./drafts/01.microservices.01.md#服务端崩溃检测)
  - [客户端容错](./drafts/01.microservices.01.md#客户端容错)
  - [注册中心选型](./drafts/01.microservices.01.md#注册中心选型)
  - [面试思路总结](./drafts/01.microservices.01.md#面试思路总结)
- [深入对比：CP 型注册中心 ZooKeeper 与 etcd](./drafts/01.microservices.01.md#深入对比cp-型注册中心-zookeeper-与-etcd)
  - [ZooKeeper 与 etcd 的特性权衡](./drafts/01.microservices.01.md#zookeeper-与-etcd-的特性权衡)
- [代码示例：基于 Go 的服务注册与发现](./drafts/01.microservices.01.md#代码示例基于-go-的服务注册与发现)
  - [基于 ZooKeeper 的服务注册与发现 Go 实现示例](./drafts/01.microservices.01.md#基于-zookeeper-的服务注册与发现-go-实现示例)
  - [基于 etcd 的服务注册与发现 Go 实现示例](./drafts/01.microservices.01.md#基于-etcd-的服务注册与发现-go-实现示例)
- [AP vs. CP：可用性与一致性的抉择](./drafts/01.microservices.01.md#ap-vs-cp可用性与一致性的抉择)
  - [为什么说 ZooKeeper (和 etcd) 是 CP 的？](./drafts/01.microservices.01.md#为什么说-zookeeper-和-etcd-是-cp-的)
  - [Eureka/Nacos 如何做到 AP？它们舍弃了什么？](./drafts/01.microservices.01.md#eurekanacos-如何做到-ap它们舍弃了什么)
