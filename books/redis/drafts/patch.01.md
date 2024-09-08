# Redis 5.0 新增 Stream

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [特性：保存历史消息的 sub/pub](#特性保存历史消息的-subpub)
- [操作：可以从历史开始订阅，可以实时订阅](#操作可以从历史开始订阅可以实时订阅)
- [源代码](#源代码)

<!-- /code_chunk_output -->

https://redis.io/docs/latest/develop/data-types/streams/

### 特性：保存历史消息的 sub/pub

> A Redis stream is a data structure that acts like an append-only log but also implements several operations to overcome some of the limits of a typical append-only log. These include random access in O(1) time and complex consumption strategies, such as consumer groups. You can use streams to record and simultaneously syndicate events in real time.

关键词：
- append-only
- O(1) 的随机索引
- 保存历史消息

所以可以理解为保存了历史消息的 sub/pub ？

### 操作：可以从历史开始订阅，可以实时订阅

https://redis.io/docs/latest/commands/?group=stream

这里用 Python 客户端演示下（我能想到的三种使用场景，代码用 LLM 生成的，不一定对，表示下哪个意思）。

如下是发送：

```python
import redis

# 创建 Redis 连接
r = redis.Redis()

# 向名为'my_stream'的 Stream 发送消息
message_id = r.xadd('my_stream', {'message': 'Hello, Redis Stream!'})
print(f"Sent message with ID: {message_id}")
```

如下是接收：

场景一：订阅时，从 stream 的第一条信息 fast forward 开始接收

```python
import redis

# 创建 Redis 连接
r = redis.Redis()

# 从名为'my_stream'的 Stream 的第一条消息开始读取
streams = r.xread({'my_stream': '0-0'}, count=1)
if streams:
    stream_name, messages = streams[0]
    for message_id, message_data in messages:
        print(f"Received message ID: {message_id}, Data: {message_data}")
else:
    print("No messages available.")
```

场景二：订阅时，忽略历史的消息，重新监听实时的消息

```python
import redis

# 创建 Redis 连接
r = redis.Redis()

# 使用 XPENDING 命令获取 Stream 的最后一个消息 ID
last_message_id = r.xinfo_stream('my_stream')['last-entry'][0]

# 使用 XREAD 命令监听实时消息，从最后一个消息 ID 的下一个开始
streams = r.xread({'my_stream': f'{last_message_id.decode()}-1'}, count=1)
if streams:
    stream_name, messages = streams[0]
    for message_id, message_data in messages:
        print(f"Received message ID: {message_id}, Data: {message_data}")
else:
    print("No messages available.")
```

场景三：订阅时，已知历史的 key，找到这个 key，从这里 back fill 消息，back fill 到最新的后也开始监听最新的消息

```python
import redis

# 创建 Redis 连接
r = redis.Redis()

known_key = '1693970218700-0'  # 假设已知的历史消息 ID

# 从已知的历史消息 ID 开始读取并回溯到最新消息
streams = r.xread({'my_stream': f'{known_key}'}, count=None, block=0)
while True:
    new_streams = r.xread({'my_stream': f'{known_key}'}, count=None, block=0)
    if not new_streams:
        break
    streams.extend(new_streams)

if streams:
    stream_name, messages = streams[0]
    for message_id, message_data in messages:
        print(f"Received message ID: {message_id}, Data: {message_data}")
else:
    print("No messages available.")
```

### 源代码

头文件如下，具体实现不贴了。

```c
#ifndef STREAM_H
#define STREAM_H

#include "rax.h"
#include "listpack.h"

/* Stream item ID: a 128 bit number composed of a milliseconds time and
 * a sequence counter. IDs generated in the same millisecond (or in a past
 * millisecond if the clock jumped backward) will use the millisecond time
 * of the latest generated ID and an incremented sequence. */
typedef struct streamID {
    uint64_t ms;        /* Unix time in milliseconds. */
    uint64_t seq;       /* Sequence number. */
} streamID;

typedef struct stream {
    rax *rax;               /* The radix tree holding the stream. */
    uint64_t length;        /* Number of elements inside this stream. */
    streamID last_id;       /* Zero if there are yet no items. */
    rax *cgroups;           /* Consumer groups dictionary: name -> streamCG */
} stream;

/* We define an iterator to iterate stream items in an abstract way, without
 * caring about the radix tree + listpack representation. Technically speaking
 * the iterator is only used inside streamReplyWithRange(), so could just
 * be implemented inside the function, but practically there is the AOF
 * rewriting code that also needs to iterate the stream to emit the XADD
 * commands. */
typedef struct streamIterator {
    stream *stream;         /* The stream we are iterating. */
    streamID master_id;     /* ID of the master entry at listpack head. */
    uint64_t master_fields_count;       /* Master entries # of fields. */
    unsigned char *master_fields_start; /* Master entries start in listpack. */
    unsigned char *master_fields_ptr;   /* Master field to emit next. */
    int entry_flags;                    /* Flags of entry we are emitting. */
    int rev;                /* True if iterating end to start (reverse). */
    uint64_t start_key[2];  /* Start key as 128 bit big endian. */
    uint64_t end_key[2];    /* End key as 128 bit big endian. */
    raxIterator ri;         /* Rax iterator. */
    unsigned char *lp;      /* Current listpack. */
    unsigned char *lp_ele;  /* Current listpack cursor. */
    unsigned char *lp_flags; /* Current entry flags pointer. */
    /* Buffers used to hold the string of lpGet() when the element is
     * integer encoded, so that there is no string representation of the
     * element inside the listpack itself. */
    unsigned char field_buf[LP_INTBUF_SIZE];
    unsigned char value_buf[LP_INTBUF_SIZE];
} streamIterator;

/* Consumer group. */
typedef struct streamCG {
    streamID last_id;       /* Last delivered (not acknowledged) ID for this
                               group. Consumers that will just ask for more
                               messages will served with IDs > than this. */
    rax *pel;               /* Pending entries list. This is a radix tree that
                               has every message delivered to consumers (without
                               the NOACK option) that was yet not acknowledged
                               as processed. The key of the radix tree is the
                               ID as a 64 bit big endian number, while the
                               associated value is a streamNACK structure.*/
    rax *consumers;         /* A radix tree representing the consumers by name
                               and their associated representation in the form
                               of streamConsumer structures. */
} streamCG;

/* A specific consumer in a consumer group.  */
typedef struct streamConsumer {
    mstime_t seen_time;         /* Last time this consumer was active. */
    sds name;                   /* Consumer name. This is how the consumer
                                   will be identified in the consumer group
                                   protocol. Case sensitive. */
    rax *pel;                   /* Consumer specific pending entries list: all
                                   the pending messages delivered to this
                                   consumer not yet acknowledged. Keys are
                                   big endian message IDs, while values are
                                   the same streamNACK structure referenced
                                   in the "pel" of the conumser group structure
                                   itself, so the value is shared. */
} streamConsumer;

/* Pending (yet not acknowledged) message in a consumer group. */
typedef struct streamNACK {
    mstime_t delivery_time;     /* Last time this message was delivered. */
    uint64_t delivery_count;    /* Number of times this message was delivered.*/
    streamConsumer *consumer;   /* The consumer this message was delivered to
                                   in the last delivery. */
} streamNACK;

/* Stream propagation informations, passed to functions in order to propagate
 * XCLAIM commands to AOF and slaves. */
typedef struct sreamPropInfo {
    robj *keyname;
    robj *groupname;
} streamPropInfo;

/* Prototypes of exported APIs. */
struct client;

/* Flags for streamLookupConsumer */
#define SLC_NONE      0
#define SLC_NOCREAT   (1<<0) /* Do not create the consumer if it doesn't exist */
#define SLC_NOREFRESH (1<<1) /* Do not update consumer's seen-time */

stream *streamNew(void);
void freeStream(stream *s);
unsigned long streamLength(const robj *subject);
size_t streamReplyWithRange(client *c, stream *s, streamID *start, streamID *end, size_t count, int rev, streamCG *group, streamConsumer *consumer, int flags, streamPropInfo *spi);
void streamIteratorStart(streamIterator *si, stream *s, streamID *start, streamID *end, int rev);
int streamIteratorGetID(streamIterator *si, streamID *id, int64_t *numfields);
void streamIteratorGetField(streamIterator *si, unsigned char **fieldptr, unsigned char **valueptr, int64_t *fieldlen, int64_t *valuelen);
void streamIteratorStop(streamIterator *si);
streamCG *streamLookupCG(stream *s, sds groupname);
streamConsumer *streamLookupConsumer(streamCG *cg, sds name, int flags);
streamCG *streamCreateCG(stream *s, char *name, size_t namelen, streamID *id);
streamNACK *streamCreateNACK(streamConsumer *consumer);
void streamDecodeID(void *buf, streamID *id);
int streamCompareID(streamID *a, streamID *b);
void streamIncrID(streamID *id);

#endif
```
