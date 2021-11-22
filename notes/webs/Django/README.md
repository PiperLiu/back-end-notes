# Django框架课

框架其实还是自学为好，毕竟，以后工作中根据业务探索新技术时，不可能指望有人手把手带你用框架。 **必须有读文档自学的能力。** 那我为啥要买这个课呢？急于吸取经验：看看一个熟练用户是如何理解 Django 的，看看 Django 有哪些注意事项，看看一些其他开发的技巧与知识点，比如 web socket 等等。

前端主要是用 `jQuery` ，页面用 `js` 在客户端渲染。路由等内容还是用 `Django` 在控制。

y 总：本课程为【AcWing工程课系列——Level-2 第一篇】《Django框架课》，讲解Python语法和Django框架。

课程期间会带领大家开发一个`Django`项目。

本课程提供`AC Terminal`应用，以及配置好环境的`docker`镜像，大家无需配置任何本地和服务器环境。

课程内容：
- `Python`语法
- `Django`框架
- `Django Channels：websocket`通信协议
- 做一个`Django`实战项目

课程链接：https://www.acwing.com/activity/content/introduction/72/

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [课程概论和Python3语法](#课程概论和python3语法)
- [配置docker、git环境与项目创建](#配置docker-git环境与项目创建)
- [创建菜单界面（Django基础）](#创建菜单界面django基础)
- [创建游戏界面（js制作游戏）](#创建游戏界面js制作游戏)
- [部署nginx与对接acapp（y总干货不多，我补充了一些）](#部署nginx与对接acappy总干货不多我补充了一些)

<!-- /code_chunk_output -->

### 课程概论和Python3语法
[./drafts/01_python3.md](./drafts/01_python3.md)

- [课程体系](./drafts/01_python3.md#课程体系)
- [Django简介](./drafts/01_python3.md#django简介)
- [实战项目](./drafts/01_python3.md#实战项目)
- [开发环境](./drafts/01_python3.md#开发环境)
- [Python3语法](./drafts/01_python3.md#python3语法)

### 配置docker、git环境与项目创建
[./drafts/02_docker.md](./drafts/02_docker.md)

- [配置 docker 环境](./drafts/02_docker.md#配置-docker-环境)
- [知识点拾遗](./drafts/02_docker.md#知识点拾遗)

### 创建菜单界面（Django基础）
[./drafts/03_django.md](./drafts/03_django.md)

- [项目结构与素材](./drafts/03_django.md#项目结构与素材)
- [操作路径记录](./drafts/03_django.md#操作路径记录)
- [总结](./drafts/03_django.md#总结)

### 创建游戏界面（js制作游戏）
[./drafts/04_js.md](./drafts/04_js.md)

- [个人心得](./drafts/04_js.md#个人心得)
- [课程概述](./drafts/04_js.md#课程概述)

### 部署nginx与对接acapp（y总干货不多，我补充了一些）
[./drafts/05_nginx.md](./drafts/05_nginx.md)

- [部署流程](./drafts/05_nginx.md#部署流程)
- [注意事项](./drafts/05_nginx.md#注意事项)
- [一些关于 nginx 和 uWSGI 的知识](./drafts/05_nginx.md#一些关于-nginx-和-uwsgi-的知识)

关于 `nginx` 与 `uWSGI` 记录在 [get_to_know_nginx](./drafts/get_to_know_nginx.md) ：
- [前置知识](./drafts/get_to_know_nginx.md#前置知识)
  - [代理](./drafts/get_to_know_nginx.md#代理)
  - [正向代理](./drafts/get_to_know_nginx.md#正向代理)
  - [反向代理](./drafts/get_to_know_nginx.md#反向代理)
  - [负载均衡](./drafts/get_to_know_nginx.md#负载均衡)
  - [负载均衡和反向代理的区别](./drafts/get_to_know_nginx.md#负载均衡和反向代理的区别)
    - [SLB 产生背景](./drafts/get_to_know_nginx.md#slb-产生背景)
    - [SLB 的三种传输模式](./drafts/get_to_know_nginx.md#slb-的三种传输模式)
    - [反向代理模式](./drafts/get_to_know_nginx.md#反向代理模式)
    - [反向代理总结](./drafts/get_to_know_nginx.md#反向代理总结)
- [Nginx](./drafts/get_to_know_nginx.md#nginx)
  - [Nginx 常用命令](./drafts/get_to_know_nginx.md#nginx-常用命令)
  - [Nginx作用与conf相应配置](./drafts/get_to_know_nginx.md#nginx作用与conf相应配置)
    - [Nginx配置简单示例](./drafts/get_to_know_nginx.md#nginx配置简单示例)
    - [动静分离与相应配置](./drafts/get_to_know_nginx.md#动静分离与相应配置)
    - [反向代理与负载均衡](./drafts/get_to_know_nginx.md#反向代理与负载均衡)
  - [一些关于Nginx的讨论](./drafts/get_to_know_nginx.md#一些关于nginx的讨论)
    - [Nginx的Master-Worker模式](./drafts/get_to_know_nginx.md#nginx的master-worker模式)
    - [Nginx如何做到热部署？](./drafts/get_to_know_nginx.md#nginx如何做到热部署)
    - [Nginx如何做到高并发下的高效处理？](./drafts/get_to_know_nginx.md#nginx如何做到高并发下的高效处理)
    - [Nginx挂了怎么办？](./drafts/get_to_know_nginx.md#nginx挂了怎么办)
- [如何理解wsgi](./drafts/get_to_know_nginx.md#如何理解wsgi)
  - [作为应用程序](./drafts/get_to_know_nginx.md#作为应用程序)
  - [作为服务器](./drafts/get_to_know_nginx.md#作为服务器)

  ### 创建账号系统（上）
  [./drafts/06_login1.md](./drafts/06_login1.md)

- [用户数据](./drafts/06_login1.md#用户数据)
- [开始设计登录逻辑](./drafts/06_login1.md#开始设计登录逻辑)
- [实现用户登录注册后端](./drafts/06_login1.md#实现用户登录注册后端)
- [实现用户登录注册前端](./drafts/06_login1.md#实现用户登录注册前端)
