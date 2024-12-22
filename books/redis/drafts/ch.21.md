# 第 21 章 排序

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [21.1 SORT 命令的实现](#211-sort-命令的实现)
- [21.2 ALPHA 选项的实现](#212-alpha-选项的实现)
- [21.3 ASC 选项和 DESC 选项的实现](#213-asc-选项和-desc-选项的实现)
- [21.4 BY 选项的实现](#214-by-选项的实现)
- [21.5 带有 ALPHA 选项的 BY 选项的实现](#215-带有-alpha-选项的-by-选项的实现)
- [21.6 LIMIT 选项的实现](#216-limit-选项的实现)
- [21.7 GET 选项的实现](#217-get-选项的实现)
- [21.8 STORE 选项的实现](#218-store-选项的实现)
- [21.9 多个选项的执行顺序](#219-多个选项的执行顺序)
  - [21.9.1 选项的执行顺序](#2191-选项的执行顺序)
  - [21.9.2 选项的摆放顺序](#2192-选项的摆放顺序)

<!-- /code_chunk_output -->

### 21.1 SORT 命令的实现

### 21.2 ALPHA 选项的实现

按照字母序排序。

### 21.3 ASC 选项和 DESC 选项的实现

### 21.4 BY 选项的实现

### 21.5 带有 ALPHA 选项的 BY 选项的实现

### 21.6 LIMIT 选项的实现

### 21.7 GET 选项的实现

GET 可以理解为一种管道 pipe ？

比如，我们 SORT 排序返回的是元素本身，但是我们可以使用 GET 选项来获取元素相关的内容，如下。

```bash
redis> SADD students "peter" "jack" "tom"
(integer) 3

redis> SORT students ALPHA
1) "jack"
2) "peter"
3) "tom"

redis> SET peter-name "Peter White"
OK

redis> SET jack-name "Jack Brown"
OK

redis> SET tom-name "Tom Green"
OK

redis> SORT students ALPHA GET *-name
1) "Jack Brown"
2) "Peter White"
3) "Tom Green"
```

也可以两个一起用，比如：

```bash
redis> SORT students ALPHA GET *-name GET *-birth
1) "Jack Brown"
2) "1995-05-24"
...
```

### 21.8 STORE 选项的实现

可以把 SORT 结果保存。

```bash
redis> SADD students "peter" "jack" "tom"
(integer) 3

redis> SORT students ALPHA STORE sorted-students
(integer) 3

redis> LRANGE sorted-students 0 -1
1) "jack"
2) "peter"
3) "tom"
```

### 21.9 多个选项的执行顺序

#### 21.9.1 选项的执行顺序

```
SORT <key> ALPHA DESC BY <by-pattern> LIMIT <offset> <count> GET <get-pattern> STORE <store-key>
```

执行顺序：
- `SORT <key> ALPHA DESC BY <by-pattern>`
- `LIMIT <offset> <count>`
- `GET <get-pattern>`
- `STORE <store-key>`

#### 21.9.2 选项的摆放顺序

除了 `GET` 选项，其他选项都可以放在任意位置，不影响。
