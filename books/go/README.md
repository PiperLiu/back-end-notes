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
- [6. Methods](#6-methods)
- [7. Interface](#7-interface)
- [8. Goroutines and Channels](#8-goroutines-and-channels)
- [9. Concurrency with Shared Variables](#9-concurrency-with-shared-variables)

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

### 6. Methods

[./drafts/go.06.methods.md](./drafts/go.06.methods.md)

- [6.1 Method Declarations](./drafts/go.06.methods.md#61-method-declarations)
- [6.2 Methods with a Pointer Receiver](./drafts/go.06.methods.md#62-methods-with-a-pointer-receiver)
  - [6.2.1 Nil is Valid Receiver Value](./drafts/go.06.methods.md#621-nil-is-valid-receiver-value)
- [6.3 Composing Types by Struct Embedding](./drafts/go.06.methods.md#63-composing-types-by-struct-embedding)
- [6.4 Method Values and Expressions](./drafts/go.06.methods.md#64-method-values-and-expressions)
- [6.5 Example: Bit Vector Type](./drafts/go.06.methods.md#65-example-bit-vector-type)
- [6.6 Encapsulation](./drafts/go.06.methods.md#66-encapsulation)

### 7. Interface

[./drafts/go.07.interfaces.md](./drafts/go.07.interfaces.md)

- [7.1 Interfaces as Contracts](./drafts/go.07.interfaces.md#71-interfaces-as-contracts)
- [7.2 Interface Types](./drafts/go.07.interfaces.md#72-interface-types)
- [7.3 Interface Satisfaction](./drafts/go.07.interfaces.md#73-interface-satisfaction)
- [7.4 Parsing Flags with flag.Value](./drafts/go.07.interfaces.md#74-parsing-flags-with-flagvalue)
- [7.5 Interface Values](./drafts/go.07.interfaces.md#75-interface-values)
  - [7.5.1 Caveat: An Interface Containing a Nil Pointer Is Non-Nil](./drafts/go.07.interfaces.md#751-caveat-an-interface-containing-a-nil-pointer-is-non-nil)
- [7.6 Sorting with sort.Interface](./drafts/go.07.interfaces.md#76-sorting-with-sortinterface)
- [7.7 The http.Handler Interface](./drafts/go.07.interfaces.md#77-the-httphandler-interface)
- [7.8 The error Interface](./drafts/go.07.interfaces.md#78-the-error-interface)
- [7.9 Example: Expression Evaluator （很实用：手写 ast 以及 eval ）](./drafts/go.07.interfaces.md#79-example-expression-evaluator-很实用手写-ast-以及-eval)
- [7.10 Type Assertions](./drafts/go.07.interfaces.md#710-type-assertions)
- [7.11 Discriminating Errors with Type Assertions](./drafts/go.07.interfaces.md#711-discriminating-errors-with-type-assertions)
- [7.12 Querying Behaviors with Interface Type Assertions](./drafts/go.07.interfaces.md#712-querying-behaviors-with-interface-type-assertions)
- [7.13 Type Switches （使用 x.(type) ）](./drafts/go.07.interfaces.md#713-type-switches-使用-xtype)
- [7.14 Example: Token-Based XML Decoding](./drafts/go.07.interfaces.md#714-example-token-based-xml-decoding)
- [7.15 A Few Words of Advice](./drafts/go.07.interfaces.md#715-a-few-words-of-advice)

### 8. Goroutines and Channels

[./drafts/go.08.goroutines-and-channels.md](./drafts/go.08.goroutines-and-channels.md)

- [8.1 Goroutines](./drafts/go.08.goroutines-and-channels.md#81-goroutines)
- [8.2 Example: Concurrent Clock Server](./drafts/go.08.goroutines-and-channels.md#82-example-concurrent-clock-server)
- [8.3 Example: Concurrent Echo Server](./drafts/go.08.goroutines-and-channels.md#83-example-concurrent-echo-server)
- [8.4 Channels](./drafts/go.08.goroutines-and-channels.md#84-channels)
  - [8.4.1 Unbuffered Channels](./drafts/go.08.goroutines-and-channels.md#841-unbuffered-channels)
  - [8.4.2 Pipelines](./drafts/go.08.goroutines-and-channels.md#842-pipelines)
  - [8.4.3 Unidirectional Channel Types](./drafts/go.08.goroutines-and-channels.md#843-unidirectional-channel-types)
  - [8.4.4 Buffered Channels](./drafts/go.08.goroutines-and-channels.md#844-buffered-channels)
- [8.5 Looping in Parallel](./drafts/go.08.goroutines-and-channels.md#85-looping-in-parallel)
- [8.6 Example: Concurrent Web Crawler](./drafts/go.08.goroutines-and-channels.md#86-example-concurrent-web-crawler)
- [8.7 Multiplexing with select](./drafts/go.08.goroutines-and-channels.md#87-multiplexing-with-select)
- [8.8 Example: Concurrent Directory Traversal](./drafts/go.08.goroutines-and-channels.md#88-example-concurrent-directory-traversal)
- [8.9 Cancellation](./drafts/go.08.goroutines-and-channels.md#89-cancellation)
- [8.10 Example: Chat Server](./drafts/go.08.goroutines-and-channels.md#810-example-chat-server)

### 9. Concurrency with Shared Variables

[./drafts/go.09.concurrency-with-shared-variables.md](./drafts/go.09.concurrency-with-shared-variables.md)

- [9.1 Race Conditions](./drafts/go.09.concurrency-with-shared-variables.md#91-race-conditions)
- [9.2 Mutual Exclusion: sync.Mutex](./drafts/go.09.concurrency-with-shared-variables.md#92-mutual-exclusion-syncmutex)
- [9.3 Read/Write Mutexes: sync.RWMutex](./drafts/go.09.concurrency-with-shared-variables.md#93-readwrite-mutexes-syncrwmutex)
- [9.4 Memory Synchronization](./drafts/go.09.concurrency-with-shared-variables.md#94-memory-synchronization)
- [9.5 Lazy Initialization: sync.Once](./drafts/go.09.concurrency-with-shared-variables.md#95-lazy-initialization-synconce)
- [9.6 The Race Detector](./drafts/go.09.concurrency-with-shared-variables.md#96-the-race-detector)
- [9.7 Example: Concurrent Non-Blocking Cache （一个键值对一个锁？不，人均 entry 做 chan broadcast ）](./drafts/go.09.concurrency-with-shared-variables.md#97-example-concurrent-non-blocking-cache-一个键值对一个锁不人均-entry-做-chan-broadcast)
- [9.8 Goroutines and Threads （重要）](./drafts/go.09.concurrency-with-shared-variables.md#98-goroutines-and-threads-重要)
  - [9.8.1 Growable Stacks](./drafts/go.09.concurrency-with-shared-variables.md#981-growable-stacks)
  - [9.8.2 Goroutine Scheduling](./drafts/go.09.concurrency-with-shared-variables.md#982-goroutine-scheduling)
  - [9.8.3 GOMAXPROCS](./drafts/go.09.concurrency-with-shared-variables.md#983-gomaxprocs)
  - [9.8.4 Goroutines Have No Identity](./drafts/go.09.concurrency-with-shared-variables.md#984-goroutines-have-no-identity)
