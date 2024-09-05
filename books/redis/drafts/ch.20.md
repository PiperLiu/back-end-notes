# 第 20 章 Lua 脚本

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [20.1 创建并修改 Lua 环境](#201-创建并修改-lua-环境)
  - [20.1.1 创建 Lua 环境](#2011-创建-lua-环境)
  - [20.1.2 载入函数库](#2012-载入函数库)
  - [20.1.3 创建 redis 全局表格](#2013-创建-redis-全局表格)
  - [20.1.4 使用 Redis 自制的随机函数来替换 Lua 原有的随机函数](#2014-使用-redis-自制的随机函数来替换-lua-原有的随机函数)
  - [20.1.5 创建排序辅助函数](#2015-创建排序辅助函数)
  - [20.1.6 创建 redis.pcall 函数的错误报告辅助函数](#2016-创建-redispcall-函数的错误报告辅助函数)
  - [20.1.7 保护 Lua 的全局环境](#2017-保护-lua-的全局环境)
  - [20.1.8 将 Lua 环境保存到服务器状态的 lua 属性中](#2018-将-lua-环境保存到服务器状态的-lua-属性中)
- [20.2 Lua 环境协作组件](#202-lua-环境协作组件)
  - [20.2.1 伪客户端](#2021-伪客户端)
  - [20.2.2 lua_scripts 字典](#2022-lua_scripts-字典)
  - [20.3 EVAL 命令的实现](#203-eval-命令的实现)
  - [20.3.1 定义脚本函数](#2031-定义脚本函数)
  - [20.3.2 将脚本保存到 lua_scripts 字典](#2032-将脚本保存到-lua_scripts-字典)
  - [20.3.3 执行脚本函数](#2033-执行脚本函数)
- [20.4 EVALSHA 命令的实现](#204-evalsha-命令的实现)
- [20.5 脚本管理命令的实现](#205-脚本管理命令的实现)
  - [20.5.1 SCRIPT FLUSH](#2051-script-flush)
  - [20.5.2 SCRIPT EXISTS](#2052-script-exists)
  - [20.5.3 SCRIPT LOAD](#2053-script-load)
  - [20.5.4 SCRIPT KILL](#2054-script-kill)
- [20.6 脚本复制](#206-脚本复制)
  - [20.6.1 复制 EVAL 命令、 SCRIPT FLUSH 命令和 SCRIPT LOAD 命令](#2061-复制-eval-命令--script-flush-命令和-script-load-命令)
  - [20.6.2 复制 EVALSHA 命令](#2062-复制-evalsha-命令)

<!-- /code_chunk_output -->

### 20.1 创建并修改 Lua 环境

Redis 服务器创建了一个 Lua 环境，并且载入了一些函数库。

#### 20.1.1 创建 Lua 环境

服务器调用 C API 函数 lua_open() 来创建 Lua 环境。

#### 20.1.2 载入函数库

#### 20.1.3 创建 redis 全局表格

在 Lua 环境中创建一个 table ，是一个全局变量。纪录 Redis 执行相关操作的相关信息。

#### 20.1.4 使用 Redis 自制的随机函数来替换 Lua 原有的随机函数

因为 Redis 要求相同脚本在不同机器上产生相同结果， Lua 函数必须无副作用 side effect ，是 pure function 。

#### 20.1.5 创建排序辅助函数

此理由与上节同。

#### 20.1.6 创建 redis.pcall 函数的错误报告辅助函数

当脚本调用 redis.pcall 函数执行 Redis 命令，但是发生错误时，函数 `__redis__err__handler` 会打印出错代码的位置和错误信息。

#### 20.1.7 保护 Lua 的全局环境

禁止将不加 local 修饰的全局变量直接赋值给 Lua 环境。

但是，并未禁止直接修改已有的全局变量。

#### 20.1.8 将 Lua 环境保存到服务器状态的 lua 属性中

### 20.2 Lua 环境协作组件

#### 20.2.1 伪客户端

Lua 脚本调用 redis.call 时，将命令通过伪客户端传送给命令执行器；接收命令结果同理。

#### 20.2.2 lua_scripts 字典

```c
struct redisServer {
    // ...
    dict *lua_scripts; // Lua 脚本字典
    // key: 脚本的校验和 SHA1 值 checksum
    // value: 脚本的源代码 source code
    // ...
};
```

因此可以载入已经执行过的 Lua 脚本，如下。

```bash
redis> SCRIPT LOAD "return 'hi'"
"2f1a7f0a4d5d5d1d9d5d5d5d"
```

#### 20.3 EVAL 命令的实现

#### 20.3.1 定义脚本函数

对于 `EVAL "return 'hi'" 0` 命令，相当于在服务器中定义了以下代码：

```lua
function f_2f1a7f0a4d5d5d1d9d5d5d5d()
    return 'hi'
end
```

#### 20.3.2 将脚本保存到 lua_scripts 字典

#### 20.3.3 执行脚本函数

这里要载入一些钩子处理超时、客户端传入 `SCRIPT KILL` 命令等情况。

### 20.4 EVALSHA 命令的实现

伪代码：

```
def EVALSHA(sha1):
    func_name = "f_" + sha1
    if function_exists_in_lua_env(func_name):
        execute_lua_function(func_name)
    else:
        load_script_from_redis_server(sha1)
```

### 20.5 脚本管理命令的实现

#### 20.5.1 SCRIPT FLUSH

#### 20.5.2 SCRIPT EXISTS

#### 20.5.3 SCRIPT LOAD

#### 20.5.4 SCRIPT KILL

### 20.6 脚本复制

主服务器复制到从服务器时。

#### 20.6.1 复制 EVAL 命令、 SCRIPT FLUSH 命令和 SCRIPT LOAD 命令

#### 20.6.2 复制 EVALSHA 命令
