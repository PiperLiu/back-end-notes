# 第 23 章 慢查询日志

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [23.1 慢查询记录的保存](#231-慢查询记录的保存)
- [23.2 慢查询日志的阅览和删除](#232-慢查询日志的阅览和删除)
- [23.3 添加新日志](#233-添加新日志)

<!-- /code_chunk_output -->

```
showlog-log-slower-than <微秒>
slowlog-max-len
```

慢查询日志用于记录那些执行时间超过指定微秒数的命令。

### 23.1 慢查询记录的保存

```c
struct redisServer {
    // ...
    long long slowlog_entry_id;
    list *slowlog; // 保存慢查询日志的链表
}
```

### 23.2 慢查询日志的阅览和删除

### 23.3 添加新日志
