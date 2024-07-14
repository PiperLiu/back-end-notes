# 第 10 章 RDB 持久化

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [10.1 RDB 文件的创建与载入](#101-rdb-文件的创建与载入)
  - [10.1.1 SAVE 命令执行时的服务器状态](#1011-save-命令执行时的服务器状态)
  - [10.1.2 BGSAVE 命令执行时的服务器状态](#1012-bgsave-命令执行时的服务器状态)
  - [10.1.3 RDB 文件载入时的服务器状态](#1013-rdb-文件载入时的服务器状态)
- [10.2 自动间隔性保存](#102-自动间隔性保存)
  - [10.2.1 设置保存条件](#1021-设置保存条件)
  - [10.2.2 dirty 计数器和 lastsave 属性](#1022-dirty-计数器和-lastsave-属性)
  - [10.2.3 检查保存条件是否满足](#1023-检查保存条件是否满足)
- [10.3 RDB 文件结构](#103-rdb-文件结构)
  - [10.3.1 databases 部分](#1031-databases-部分)
  - [10.3.2 key_value_pairs 部分](#1032-key_value_pairs-部分)
  - [10.3.3 value 的编码](#1033-value-的编码)
- [10.4 分析 RDB 文件](#104-分析-rdb-文件)
  - [10.4.1 不包含任何键值对的 RDB 文件](#1041-不包含任何键值对的-rdb-文件)
  - [10.4.2 包含字符串键的 RDB 文件](#1042-包含字符串键的-rdb-文件)
  - [10.4.3 包含带有过期时间的字符串键的 RDB 文件](#1043-包含带有过期时间的字符串键的-rdb-文件)
  - [10.4.4 包含一个集合键的 RDB 文件](#1044-包含一个集合键的-rdb-文件)
  - [10.4.5 关于分析 RDB 文件的说明](#1045-关于分析-rdb-文件的说明)

<!-- /code_chunk_output -->

### 10.1 RDB 文件的创建与载入

`SAVE` 命令阻塞式地 dump rdb 文件。

`BGSAVE` 命令在后台异步地 dump rdb 文件，代码实现层面涉及 `fork()` 。

#### 10.1.1 SAVE 命令执行时的服务器状态

进程阻塞了。

#### 10.1.2 BGSAVE 命令执行时的服务器状态

由于是 `fork` 出来的子进程实现的，因此父进程仍然可以执行命令。但是 `SAVE` 不行。

#### 10.1.3 RDB 文件载入时的服务器状态

载入时什么都做不了。

### 10.2 自动间隔性保存

#### 10.2.1 设置保存条件

`server.saveparams` 是一个数组。

#### 10.2.2 dirty 计数器和 lastsave 属性

`dirty` 记录上次修改后修改操作次数。

`lastsave` 记录上次保存的时间戳。

#### 10.2.3 检查保存条件是否满足

是在 `serverCron` 函数中一个很简单的遍历。

src/server.c

```c
        /* If there is not a background saving/rewrite in progress check if
         * we have to save/rewrite now */
         for (j = 0; j < server.saveparamslen; j++) {
            struct saveparam *sp = server.saveparams+j;

            /* Save if we reached the given amount of changes,
             * the given amount of seconds, and if the latest bgsave was
             * successful or if, in case of an error, at least
             * REDIS_BGSAVE_RETRY_DELAY seconds already elapsed. */
            if (server.dirty >= sp->changes &&
                server.unixtime-server.lastsave > sp->seconds &&
                (server.unixtime-server.lastbgsave_try >
                 REDIS_BGSAVE_RETRY_DELAY ||
                 server.lastbgsave_status == REDIS_OK))
            {
                redisLog(REDIS_NOTICE,"%d changes in %d seconds. Saving...",
                    sp->changes, (int)sp->seconds);
                rdbSaveBackground(server.rdb_filename);
                break;
            }
         }
```

### 10.3 RDB 文件结构

src/rio.h

```c
truct _rio {
    /* Backend functions.
     * Since this functions do not tolerate short writes or reads the return
     * value is simplified to: zero on error, non zero on complete success. */
    size_t (*read)(struct _rio *, void *buf, size_t len);
    size_t (*write)(struct _rio *, const void *buf, size_t len);
    off_t (*tell)(struct _rio *);
    int (*flush)(struct _rio *);
    /* The update_cksum method if not NULL is used to compute the checksum of
     * all the data that was read or written so far. The method should be
     * designed so that can be called with the current checksum, and the buf
     * and len fields pointing to the new block of data to add to the checksum
     * computation. */
    void (*update_cksum)(struct _rio *, const void *buf, size_t len);

    /* The current checksum */
    uint64_t cksum;

    /* number of bytes read or written */
    size_t processed_bytes;

    /* maximum single read or write chunk size */
    size_t max_processing_chunk;

    /* Backend-specific vars. */
    union {
        /* In-memory buffer target. */
        struct {
            sds ptr;
            off_t pos;
        } buffer;
        /* Stdio file pointer target. */
        struct {
            FILE *fp;
            off_t buffered; /* Bytes written since last fsync. */
            off_t autosync; /* fsync after 'autosync' bytes written. */
        } file;
        /* Multiple FDs target (used to write to N sockets). */
        struct {
            int *fds;       /* File descriptors. */
            int *state;     /* Error state of each fd. 0 (if ok) or errno. */
            int numfds;
            off_t pos;
            sds buf;
        } fdset;
    } io;
};
```

src/rdb.c

```c
/* Produces a dump of the database in RDB format sending it to the specified
 * Redis I/O channel. On success REDIS_OK is returned, otherwise REDIS_ERR
 * is returned and part of the output, or all the output, can be
 * missing because of I/O errors.
 *
 * When the function returns REDIS_ERR and if 'error' is not NULL, the
 * integer pointed by 'error' is set to the value of errno just after the I/O
 * error. */
int rdbSaveRio(rio *rdb, int *error) {
    dictIterator *di = NULL;
    dictEntry *de;
    char magic[10];
    int j;
    long long now = mstime();
    uint64_t cksum;

    if (server.rdb_checksum)
        rdb->update_cksum = rioGenericUpdateChecksum;
    snprintf(magic,sizeof(magic),"REDIS%04d",REDIS_RDB_VERSION);
    if (rdbWriteRaw(rdb,magic,9) == -1) goto werr;

    for (j = 0; j < server.dbnum; j++) {
        redisDb *db = server.db+j;
        dict *d = db->dict;
        if (dictSize(d) == 0) continue;
        di = dictGetSafeIterator(d);
        if (!di) return REDIS_ERR;

        /* Write the SELECT DB opcode */
        if (rdbSaveType(rdb,REDIS_RDB_OPCODE_SELECTDB) == -1) goto werr;
        if (rdbSaveLen(rdb,j) == -1) goto werr;

        /* Iterate this DB writing every entry */
        while((de = dictNext(di)) != NULL) {
            sds keystr = dictGetKey(de);
            robj key, *o = dictGetVal(de);
            long long expire;

            initStaticStringObject(key,keystr);
            expire = getExpire(db,&key);
            if (rdbSaveKeyValuePair(rdb,&key,o,expire,now) == -1) goto werr;
        }
        dictReleaseIterator(di);
    }
    di = NULL; /* So that we don't release it again on error. */

    /* EOF opcode */
    if (rdbSaveType(rdb,REDIS_RDB_OPCODE_EOF) == -1) goto werr;

    /* CRC64 checksum. It will be zero if checksum computation is disabled, the
     * loading code skips the check in this case. */
    cksum = rdb->cksum;
    memrev64ifbe(&cksum);
    if (rioWrite(rdb,&cksum,8) == 0) goto werr;
    return REDIS_OK;

werr:
    if (error) *error = errno;
    if (di) dictReleaseIterator(di);
    return REDIS_ERR;
}
```

看起来 `rio` 是 io 时用到的状态结构变量。

按照书上：
- `REDIS` 可以理解为文件协议标识？
- `db_version`
- `databases` 是数据
- `EOF` 结束符
- `checksum` 校验和，防止 rdb 文件损坏带来问题

#### 10.3.1 databases 部分

- `SELECTDB` 标识即将读到一个新数据库
- `db_number` 数据库编号
- `key_value_pairs`

#### 10.3.2 key_value_pairs 部分

- `TYPE`
- `key`
- `value`

#### 10.3.3 value 的编码

介绍了各个数据结构的：
- 编码排布
- 压缩规则

### 10.4 分析 RDB 文件

#### 10.4.1 不包含任何键值对的 RDB 文件

用 `od -c dump.rdb` 命令查看文件内容。

> od指令会读取所给予的文件的内容，并将其内容以八进制字码呈现出来。

#### 10.4.2 包含字符串键的 RDB 文件

#### 10.4.3 包含带有过期时间的字符串键的 RDB 文件

#### 10.4.4 包含一个集合键的 RDB 文件

#### 10.4.5 关于分析 RDB 文件的说明

可以使用专用工具 `redis-check-dump` 来分析 RDB 文件。

