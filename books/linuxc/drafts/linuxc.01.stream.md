# 第 6 章 Linux 的流

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [流可以理解为由系统分配的内存缓冲区](#流可以理解为由系统分配的内存缓冲区)
- [流的基础操作（读写、按行读写、按二进制读写）](#流的基础操作读写-按行读写-按二进制读写)
- [流的定位](#流的定位)
- [流的缓冲管理](#流的缓冲管理)
- [流的格式化输出](#流的格式化输出)

<!-- /code_chunk_output -->

### 流可以理解为由系统分配的内存缓冲区

Linux 下的文件编程所涉及的操作方式都是不带缓冲的 I/O ，因为每次调用相应的函数比如说 `read` 、 `write` 等对文件进行操作的时候都会调用内核的系统调用，由于每次都要通过内核对文件进行操作，所以操作效率比较低，对于流编程来说，首先对文件所映射的流进行操作，然后分阶段将相应的数据写入文件，极大地提高了相应的操作效率。

### 流的基础操作（读写、按行读写、按二进制读写）

Linux 提供 `fopen` 系列函数完成缓冲区和对应的文件相关联的过程。

```c
#include <stdio.h>
FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);  // 常用于将流定向于文件
```

调用成功返回一个 `FILE` 类型的文件指针，否则返回一个 `NULL` 指针。

可以使用 `fileno` 函数获取文件指针的文件描述符，例子见 [exam602fileno.c](../sample/chapter6/exam602fileno.c) 。

读写流函数如下。

```c
#include <stdio.h>
int fgetc(FILE *stream);
int getc(FILE *stream);
int getchar(void);

int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
```

此外按照行读写流如下。

```c
#include <stdio.h>
char *fgets(char *s, int size, FILE *stream);
char *gets(char *s);

int fputs(const char *s, FILE *stream);
int puts(const char *s);
```

使用 `fgets` 和 `fputs` 函数可以实现行读写文件，例子见 [exam606fgetsfile.c](../sample/chapter6/exam606fgetsfile.c) 。

按照块/结构（二进制）读写流如下。

```c
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
```

流的出错处理如下。

```c
#include <stdio.h>
int feof(FILE *stream);
int ferror(FILE *stream);
```

### 流的定位

```c
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence);
// whence: SEEK_SET, SEEK_CUR, SEEK_END
long ftell(FILE *stream);

// 将偏移量设定到流的起始部分
void rewind(FILE *stream);

int fseeko(FILE *stream, off_t offset, int whence);
off_t ftello(FILE *stream);

int fgetpos(FILE *stream, fpos_t *pos);
int fsetpos(FILE *stream, const fpos_t *pos);
```

### 流的缓冲管理

管理流的缓冲方式：
- 全缓冲：缓冲区满了再进行实际的 I/O 操作，磁盘文件通常是全缓冲的
- 行缓冲：每次读取一行，遇到 `\n` 进行 I/O ，标准输入和标准输出默认是行缓冲
- 无缓冲：标准错误 `stderr` 认是无缓冲的

缓冲方式的信息保存在 `FILE *fp; fp->_IO_file_flags` 中。

可以使用如下系统调用设置缓冲区大小。

```c
#include <stdio.h>
void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);
```

对于 `setvbuf` 来讲，如果 `mode` 为 `_IOFBF` 全缓冲或 `_IOLBF` 行缓冲，如果传入 `buf == NULL` ，则缓冲区由系统分配。

此外，还可以对流进行冲洗。

```c
#include <stdio.h>
int fflush(FILE *stream);  // 将缓冲区中尚未写入文件的数据写入文件

#include <stdio.h>
#include <stdio_ext.h>
void __fpurge(FILE *stream);  // 清空缓冲区
```

### 流的格式化输出

```c
#include <stdio.h>
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
```
