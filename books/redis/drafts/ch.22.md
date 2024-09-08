# 第 22 章 二进制位数组

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [22.1 位数组的表示](#221-位数组的表示)
- [22.2 GETBIT 命令的实现](#222-getbit-命令的实现)
- [22.3 SETBIT 命令的实现](#223-setbit-命令的实现)
  - [22.3.1 SETBIT 命令的执行示例](#2231-setbit-命令的执行示例)
  - [22.3.2 带扩展操作的 SETBIT 命令示例](#2232-带扩展操作的-setbit-命令示例)
- [22.4 BITCOUNT 命令的实现](#224-bitcount-命令的实现)
  - [22.4.1 二进制位统计算法（1）：遍历算法](#2241-二进制位统计算法1遍历算法)
  - [22.4.2 二进制位统计算法（2）：查表算法](#2242-二进制位统计算法2查表算法)
  - [22.4.3 二进制位统计算法（3）：variable-precision SWAR 算法](#2243-二进制位统计算法3variable-precision-swar-算法)
  - [22.4.4 二进制位统计算法（4）：Redis的实现](#2244-二进制位统计算法4redis的实现)
- [22.5 BITOP 命令的实现](#225-bitop-命令的实现)

<!-- /code_chunk_output -->

```bash
redis> SETBIT bit 0 1  # 0000 0001
redis> SETBIT bit 3 1  # 0000 1001
```

此外还有其他命令。

### 22.1 位数组的表示

用的是 sds 字符串数据结构。

### 22.2 GETBIT 命令的实现

`GETBIT <bitarray> <offset>` 命令用于获取位数组 `<bitarray>` 中偏移量为 `<offset>` 的位的值。

常数时间复杂度。

### 22.3 SETBIT 命令的实现

#### 22.3.1 SETBIT 命令的执行示例

#### 22.3.2 带扩展操作的 SETBIT 命令示例

涉及到“动态扩容”。

### 22.4 BITCOUNT 命令的实现

#### 22.4.1 二进制位统计算法（1）：遍历算法

#### 22.4.2 二进制位统计算法（2）：查表算法

#### 22.4.3 二进制位统计算法（3）：variable-precision SWAR 算法

尽管有些硬件/指令集已经支持直接统计二进制位数，但还是有必要了解目前已知的最快通用算法。不需要使用额外内存。

原理不纪录。

#### 22.4.4 二进制位统计算法（4）：Redis的实现

查表法 + variable-precision SWAR 算法。

### 22.5 BITOP 命令的实现

比如 `BITOP AND destkey key1 key2` 这种操作命令。
