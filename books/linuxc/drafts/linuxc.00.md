# 杂记

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [关于 make 的一个案例：使用变量、自动变量、隐式规则、模式规则](#关于-make-的一个案例使用变量-自动变量-隐式规则-模式规则)

<!-- /code_chunk_output -->

### 关于 make 的一个案例：使用变量、自动变量、隐式规则、模式规则

最开始的 `makefile` 如下。

```makefile
exam1: exam2.o exam3.o
	gcc exam2.o exam3.o -o exam1
exam2.o: exam2.c exam2.h
	gcc -Wall -O -g -c exam2.c -o exam2.o
exam3.o: exam3.c exam3.h
	gcc -Wall -O -g -c exam3.c -o exam3.o
```

使用变量修改如下。

```makefile
OBJS = exam2.o exam3.o
CC = gcc
CFLAGS = -Wall -O -g
exam1: $(OBJS)
	$(CC) $(OBJS) -o exam1
exam2.o: exam2.c exam2.h
	$(CC) $(CFLAGS) -c exam2.c -o exam2.o
exam3.o: exam3.c exam3.h
	$(CC) $(CFLAGS) -c exam3.c -o exam3.o
```

使用自动变量如下。

```makefile
OBJS = exam2.o exam3.o
CC = gcc
CFLAGS = -Wall -O -g
exam1: $(OBJS)
	$(CC) $^ -o $@
exam2.o: exam2.c exam2.h
	$(CC) $(CFLAGS) -c $< -o $@
exam3.o: exam3.c exam3.h
	$(CC) $(CFLAGS) -c $< -o $@
```

其中， `$^` 表示所有不重复依赖的文件，以空格分开，如果目标是静态库文件，它所代表的只能是所有的库成员（ `.o` 文件）名； `$<` 是第一个依赖文件的名称； `$@` 表示目标文件。 **可检索 makefile 自动变量相关关键字。**

还可以利用一些规则。

比如利用 make 的隐式规则，可以修改如下。

```makefile
OBJS = exam2.o exam3.o
CC = gcc
CFLAGS = -Wall -O -g
exam1: $(OBJS)
	$(CC) $^ -o $@
```

在 makefile 中的隐式规则中指出，所有的 `.o` 文件都可以自动由 `.c` 文件使用命令 `$(CC) $(CPPFLAGS) $(CFLAGS) -c file.c -o file.o` 生成。

此外，还可以应用模式规则。

```makefile
OBJS = exam2.o exam3.o
CC = gcc
CFLAGS = -Wall -O -g
exam1: $(OBJS)
	$(CC) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```
