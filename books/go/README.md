# Go 语言程序设计（英文版）

Alan A.A.Donovan. Brian W.Kemighan. Go 语言程序设计（英文版）. 电子工业出版社. 第一版

![](./drafts/images/20220712.go1.jpg)

![](./drafts/images/20220712.go2.jpg)

夯实一下 go ，找这本书再合适不过了。已经是读的第三本 Kemighan 的书了。在公司的书架上找到的，纯英文版，值得挑战一下。

- 本书的代码 https://github.com/adonovan/gopl.io/
- 本书的网站 http://www.gopl.io/

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1. Tutorial](#1-tutorial)
- [2. Program Structure](#2-program-structure)
- [3. Basic Data Types](#3-basic-data-types)
- [4. Composite Types](#4-composite-types)
- [5. Functions](#5-functions)

<!-- /code_chunk_output -->

### 1. Tutorial

[./drafts/go.01.tutorial.md](./drafts/go.01.tutorial.md)

- [1.1 Hello, World](./drafts/go.01.tutorial.md#11-hello-world)
- [1.2 Command-Line Arguments](./drafts/go.01.tutorial.md#12-command-line-arguments)
- [1.3 Finding Duplicate Lines](./drafts/go.01.tutorial.md#13-finding-duplicate-lines)
- [1.4 Animated GIFs](./drafts/go.01.tutorial.md#14-animated-gifs)
- [1.5 Fetching a URL](./drafts/go.01.tutorial.md#15-fetching-a-url)
- [1.6 Fetching URLs Concurrently](./drafts/go.01.tutorial.md#16-fetching-urls-concurrently)
- [1.7 A Web Server](./drafts/go.01.tutorial.md#17-a-web-server)
- [1.8 Loose Ends](./drafts/go.01.tutorial.md#18-loose-ends)

### 2. Program Structure

[./drafts/go.02.program-structure.md](./drafts/go.02.program-structure.md)

- [2.1 Names](./drafts/go.02.program-structure.md#21-names)
- [2.2 Declarations](./drafts/go.02.program-structure.md#22-declarations)
- [2.3 Variables](./drafts/go.02.program-structure.md#23-variables)
  - [2.3.1 Short Variable Declarations](./drafts/go.02.program-structure.md#231-short-variable-declarations)
  - [2.3.2 Pointers](./drafts/go.02.program-structure.md#232-pointers)
  - [2.3.3 The new Function](./drafts/go.02.program-structure.md#233-the-new-function)
  - [2.3.4 Lifetime of Variables](./drafts/go.02.program-structure.md#234-lifetime-of-variables)
- [2.4 Assignments](./drafts/go.02.program-structure.md#24-assignments)
  - [2.4.1 Tuple Assignment](./drafts/go.02.program-structure.md#241-tuple-assignment)
  - [2.4.2 Assignability](./drafts/go.02.program-structure.md#242-assignability)
- [2.5 Type Declarations](./drafts/go.02.program-structure.md#25-type-declarations)
- [2.6 Packages and Files](./drafts/go.02.program-structure.md#26-packages-and-files)
  - [2.6.1 Imports](./drafts/go.02.program-structure.md#261-imports)
  - [2.6.2 Package Initialization](./drafts/go.02.program-structure.md#262-package-initialization)
- [2.7 Scope](./drafts/go.02.program-structure.md#27-scope)

### 3. Basic Data Types

[./drafs/go.03.basic-data-types.md](./drafts/go.03.basic-data-types.md)

- [3.1 Integers](./drafts/go.03.basic-data-types.md#31-integers)
- [3.2 Floating-Point Numbers](./drafts/go.03.basic-data-types.md#32-floating-point-numbers)
- [3.3 Complex Numbers](./drafts/go.03.basic-data-types.md#33-complex-numbers)
- [3.4 Booleans](./drafts/go.03.basic-data-types.md#34-booleans)
- [3.5 Strings （博弈：可变性与内存共享）](./drafts/go.03.basic-data-types.md#35-strings-博弈可变性与内存共享)
  - [3.5.1 String Literals](./drafts/go.03.basic-data-types.md#351-string-literals)
  - [3.5.2 Unicode （引出 rune ）](./drafts/go.03.basic-data-types.md#352-unicode-引出-rune)
  - [3.5.3 UTF-8](./drafts/go.03.basic-data-types.md#353-utf-8)
  - [3.5.4 Strings and Byte Slices](./drafts/go.03.basic-data-types.md#354-strings-and-byte-slices)
  - [3.5.5 Conversions between Strings and Numbers](./drafts/go.03.basic-data-types.md#355-conversions-between-strings-and-numbers)
- [3.6 Constants](./drafts/go.03.basic-data-types.md#36-constants)
  - [3.6.1 The Constant Generator iota](./drafts/go.03.basic-data-types.md#361-the-constant-generator-iota)
  - [3.6.2 Untyped Constants](./drafts/go.03.basic-data-types.md#362-untyped-constants)

### 4. Composite Types

[./drafts/go.04.composite-types.md](./drafts/go.04.composite-types.md)

- [4.1 Arrays](./drafts/go.04.composite-types.md#41-arrays)
- [4.2 Slices](./drafts/go.04.composite-types.md#42-slices)
  - [4.2.1 The append Function](./drafts/go.04.composite-types.md#421-the-append-function)
  - [4.2.2 In-Place Slice Techniques](./drafts/go.04.composite-types.md#422-in-place-slice-techniques)
- [4.3 Maps](./drafts/go.04.composite-types.md#43-maps)
- [4.4 Structs](./drafts/go.04.composite-types.md#44-structs)
  - [4.4.1 Struct Literals](./drafts/go.04.composite-types.md#441-struct-literals)
  - [4.4.2 Comparing Structs](./drafts/go.04.composite-types.md#442-comparing-structs)
  - [4.4.3 Struct Embedding and Anonymous Fields](./drafts/go.04.composite-types.md#443-struct-embedding-and-anonymous-fields)
- [4.5 JSON](./drafts/go.04.composite-types.md#45-json)
- [4.6 Text and HTML Templates](./drafts/go.04.composite-types.md#46-text-and-html-templates)

### 5. Functions

[./drafts/go.05.functions.md](./drafts/go.05.functions.md)

- [5.1 Function Declarations](./drafts/go.05.functions.md#51-function-declarations)
- [5.2 Recursion](./drafts/go.05.functions.md#52-recursion)
- [5.3 Multiple Return Values](./drafts/go.05.functions.md#53-multiple-return-values)
- [5.4 Errors](./drafts/go.05.functions.md#54-errors)
  - [5.4.1 Error-Handling Strategies](./drafts/go.05.functions.md#541-error-handling-strategies)
  - [5.4.2 End of File (EOF)](./drafts/go.05.functions.md#542-end-of-file-eof)
- [5.5 Function Values](./drafts/go.05.functions.md#55-function-values)
- [5.6 Anonymous Functions](./drafts/go.05.functions.md#56-anonymous-functions)
  - [5.6.1 Caveat: Capturing Iteration Variables （重要：闭包捕获循环变量的坑）](./drafts/go.05.functions.md#561-caveat-capturing-iteration-variables-重要闭包捕获循环变量的坑)
- [5.7 Variadic Functions](./drafts/go.05.functions.md#57-variadic-functions)
- [5.8 Deferred Function Calls](./drafts/go.05.functions.md#58-deferred-function-calls)
- [5.9 Panic](./drafts/go.05.functions.md#59-panic)
- [5.10 Recover](./drafts/go.05.functions.md#510-recover)
