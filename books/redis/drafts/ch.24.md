# 第 24 章 监视器

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [24.1 成为监视器](#241-成为监视器)
- [24.2 向监视器发送命令信息](#242-向监视器发送命令信息)

<!-- /code_chunk_output -->

通过执行 MONITOR 命令客户端可以将自己变成一个监视器，实时地接收并打印出服务器执行的命令。

### 24.1 成为监视器

### 24.2 向监视器发送命令信息

服务器在每次处理命令请求之前，都会调用 `replicationFeedMonitors` 函数，该函数会将命令信息发送给所有监视器。

```
# 伪代码如下
def replicationFeedMonitors(client, monitors, dbid, argv, argc):
    msg = create_message(client, dbid, argv, argc)
    for monitor in monitors:
        send_message(monitor, msg)
```
