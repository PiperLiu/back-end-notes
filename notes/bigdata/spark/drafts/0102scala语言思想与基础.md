# scala语言思想与基础

其实把[菜鸟教程](https://www.runoob.com/scala/scala-tutorial.html)的读明白就够牛逼了，但是这里我还想总结强调一下，方便以后快速捡起来。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Scala 基本思想与注意事项](#scala-基本思想与注意事项)
  - [注意事项速查！](#注意事项速查)
- [最基本的语法示例](#最基本的语法示例)
  - [类型的声明、控制结构（for、模式匹配、case）](#类型的声明-控制结构for-模式匹配-case)
  - [Scala 语法独特的地方](#scala-语法独特的地方)
  - [Scala 的面向对象与一等公民“函数”](#scala-的面向对象与一等公民函数)
- [基本数据结构](#基本数据结构)
  - [注意事项与 Scala 奇技淫巧](#注意事项与-scala-奇技淫巧)
  - [关于数据结构的讨论（List or Array?）](#关于数据结构的讨论list-or-array)

<!-- /code_chunk_output -->

参考资料主要有：
- 曹洁 . Spark大数据分析技术（Scala版）[M]. 北京航空航天大学出版社, 2021. ISBN：9787512433854
- 陈欢 , 林世飞 . Spark最佳实践[M]. 人民邮电出版社, 2016. ISBN：9787115422286

### Scala 基本思想与注意事项

Sacla 即 Scalable Language ，正如其名，是一门可伸缩的编程语言：
- 基于 java 的虚拟机（ Scala 会被编译成 JVM 字节码）
- 但是既可以当脚本使用，又可以构造大型系统
- 是静态语言，但是可以像动态语言那样支持交互式编程
- 面型对象：每一个值都是对象，每一次运算都是一次方法调用
- 函数式编程：所有函数都是对象，函数是“一等公民”
- Scala 中几乎一切都是表达式

`scala` 是解释器， `scalac` 是编译器；可以直接 `scala test.scala` ，也可以 `scalac test.scala & scala test` （先把源码编译为字节码，再把字节码放到虚拟机中解释运行）。还可用输入 `scala` 进入交换编程界面。

所以要注意的是，需要先安装 JDK ，并且设置好环境变量 `JAVA_HOME` 。此外，更加重要的是， Scala 小版本兼容：`2.12.x` 与 `2.13.x` 这两者不兼容，`2.12.10` 与 `2.12.11` 才兼容。

#### 注意事项速查！

目前一共有 **3** 点：
- 泛型用 `[]` 而非 `<>`
- 对于 `Array[String]` **这种可迭代对象，取元素用 圆括号** `res(0)` 而非方括号 `res[0]`
- 对于 `RDD` 对象，不要轻易 `collect` ！多多利用其弹性内存的特性！

### 最基本的语法示例

#### 类型的声明、控制结构（for、模式匹配、case）

```scala
// 变量
val two: Int = 1 + 1

var one: Int = 1
var one: String = 'one'

// 函数
def addOne(x: Int): Int = x + 1

def add(x: Int, y: Int): Int = {
    x + y
}

// 部分控制结构
var filename = 
    if (!args.isEmpty) args(0)
    else "default.txt"

for (i <- 1 to 4)
    println("iteration " + i)
```

`1 to 4` 是 `[1,2,3,4]` ，而 `i until 4` 是 `[1,2,3]` 。

关于 `for` 还有一些奇技淫巧。
```scala
// 多个区间
for (a <- 1 to 2; b <- 1 to 2) {
    println("a: " + a + ", b: " + b)
}
// 结果
a: 1, b: 1
a: 1, b: 2
a: 2, b: 1
a: 2, b: 2

// 过滤器
val list1 = List(3, 5, 2, 1, 7)
for (x <- list1 if x % 2 == 1) print(" " + x)
// 3 5 1 7
```

关于模式匹配，则有更多奇技淫巧。这里我直接参考：[scala中case的用法](https://blog.csdn.net/qq_41669665/article/details/86158993)

```scala
// 一.简单匹配，值匹配：

val bools = List(true, false)
for (bool <- bools) {
    bool match {
        case true => println("heads")
        case false => println("tails")
        case _ => println("something other than heads or tails (yikes!)")
    }
}

import scala.util.Random
val randomInt = new Random().nextInt(10)
randomInt match {
    case 7 => println("lucky seven!")
    case otherNumber => println("boo, got boring ol' " + otherNumber)
}

// 二. 类型匹配

val sundries = List(23, "Hello", 8.5, 'q')
for (sundry <- sundries) {
    sundry match {
        case i: Int => println("got an Integer: " + i)
        case s: String => println("got a String: " + s)
        case f: Double => println("got a Double: " + f)
        case other => println("got something else: " + other)
    }
}

// 三 根据顺序匹配

val willWork = List(1, 3, 23, 90)
val willNotWork = List(4, 18, 52)
val empty = List()
for (l <- List(willWork, willNotWork, empty)) {
    l match {
        case List(_, 3, _, _) => println("Four elements, with the 2nd being '3'.")
        case List(_*) => println("Any other list with 0 or more elements.")
    }
}

// 四 case里面用 guard 的数组匹配

val tupA = ("Good", "Morning!")
val tupB = ("Guten", "Tag!")
    for (tup <- List(tupA, tupB)) {
        tup match {
            case (thingOne, thingTwo) if thingOne == "Good" =>
            println("A two-tuple starting with 'Good'.")
            case (thingOne, thingTwo) =>println("This has two things: " + thingOne + " and " + thingTwo)
        }
}

// 五 对象深度匹配

case class Person(name: String, age: Int)
val alice = new Person("Alice", 25)
val bob = new Person("Bob", 32)
val charlie = new Person("Charlie", 32)
for (person <- List(alice, bob, charlie)) {
    person match {
        case Person("Alice", 25) => println("Hi Alice!")
        case Person("Bob", 32) => println("Hi Bob!")
        case Person(name, age) =>
            println("Who are you, " + age + " year-old person named " + name + "?")
    }
}

// 六 正则表达式匹配

val BookExtractorRE = """Book: title=([^,]+),\s+authors=(.+)""".r
val MagazineExtractorRE = """Magazine: title=([^,]+),\s+issue=(.+)""".r

val catalog = List(
    "Book: title=Programming Scala, authors=Dean Wampler, Alex Payne",
    "Magazine: title=The New Yorker, issue=January 2009",
    "Book: title=War and Peace, authors=Leo Tolstoy",
    "Magazine: title=The Atlantic, issue=February 2009",
    "BadData: text=Who put this here??"
)

for (item <- catalog) {
    item match {
        case BookExtractorRE(title, authors) =>
            println("Book \"" + title + "\", written by " + authors)
        case MagazineExtractorRE(title, issue) =>
            println("Magazine \"" + title + "\", issue " + issue)
        case entry => println("Unrecognized entry: " + entry)
    }
}
```

关于 case ，我想强调其在“解包”中的应用：
```scala
dict = Map("Piper" -> 95, "Bob" -> 90)
dict.foreach {
    case (k, v) => printf(
        "grade of %s is %s/n", k, v
    )
}

grade of Piper is 95
grade of Bob is 90
```

上述：使用了 `foreach { case () => {} }` ，注意 `foreach` 的大括号。与下面等效。

```scala
dict = Map("Piper" -> 95, "Bob" -> 90)
dict.foreach (
    x => println(
        s"grade of ${x._1} is ${x._2}"
    )
)

grade of Piper is 95
grade of Bob is 90
```

#### Scala 语法独特的地方

1. 无参数方法，调用时不用加括号：`args.isEmpty`。
```scala
def width: Int = if (height == 0) 0 else contents(0).length

width  // 调用
```

2. `for` 中使用 `<-` ，相当于 Python 的 `in` 。

3. 继承用关键字 `extends` ：`class A(a: Int) extends B` 。

4. 单实例对象 / 静态成员变量与方法定义在 `object` 中：
```scala
object Timer {
    var count = 0
    def currentCount() : Long = {
        count += 1
        count
    }
}

Timer.currentCount()  // 直接调用

class Timer {
    ...
}
```

5. 函数返回不必非要加 `return` ，默认最后一个表达式。

6. 函数式：匿名函数作为参数，并且还可以更简洁
```scala
val numbers = List(1, -3, -5, 9, 0)

numbers.filter((x) => x > 0)
numbers.filter(x => x > 0)
numbers.filter(_ > 0)  // 一个参数且函数中仅被使用一次时
```

7. `_` 具有特殊的意义与工作（占位）
```scala
// 部分应用函数
def adder(m: Int, n: Int) = m + n

val add2 = adder(2, _: Int)  // add2: (Int) => Int = <function1>
add2(3)  // res1: Int = 5

// 柯里化 currying
def curriedSum(x: Int)(y: Int) = x + y
curriedSum (1)(2)

val onePlus = curriedSum(1)_  // 注意这里使用了 _
onePlus(2)

// 模式匹配
var times = 1
times match {
    case 1 => "one"
    case 2 => "two"
    case _ => "other"
}
```

#### Scala 的面向对象与一等公民“函数”
```scala
(1).+(2)  // 3
```

如上，`(1)`是对象，`.+(2)`是方法调用。 Scala 中万物皆对象。

```scala
var increase = (x: Int) => x + 1
```

如上，函数是一等公民，可以赋值给变量。

### 基本数据结构

有以下概念：
- 不可变列表 `List` 与可变列表 `ListBuffer`
- 定长数组 `Array` 与变长数组 `ArrayBuffer`
- 不可变集合 `Set` 与可变集合 `scala.collection.mutable.Set`
- 映射 `Map` 与 可变映射 `scala.collection.mutable.Map`
- 元组 `Tuple`

#### 注意事项与 Scala 奇技淫巧

1. 使用 `until` 是遍历数组的好办法，`by` 和 `_*` 特殊意义：
```scala
for (i <- 0 until.length) { }

Array (1,3,5,7,9,11)  // 等价于
Array[Int](1 to 11 by 2:_*)  // _* 有种解包的意味
```

2. 使用 `yield` 生成数组
```scala
val a = Array(1, 2, 3, 4)
val res1 = for (ele <- a) yield 2 * ele
// 2, 4, 6, 8
```

3. 元组的下标从 `1` 开始
```scala
val person = (1, 2, "ABC")
person._1  // 1
```

4. 拉链操作 `zip`
```scala
val symbols = Array("<", "-", ">")
val counts = Array(2, 10, 2)
val pairs = symbols.zip(counts)
// Array[(String, Int)] = Array((<, 2), (-, 10), (>, 2))
for ((s, n) <- pairs) print(s * n)
<<---------->>
```

5. `Map` 神奇操作

```scala
// 创建
val dict = Map("Piper" -> 95, "Bob" -> 90)
val kv   = Map(("Piper", 95), ("Bob", 90))

// 取值
dict("Piper")

// 合并 ++
dict ++ kv
dict.++(kv)

// 添加 + ，删除 -
val n = dict + ("Tom" -> 91)
val l = dict - "Tom"
```

对于可变 `Map` ：
```scala
// += -=
dict += (("Tom", 91), ("Jerry", 87))
dict -= "Tom"
dict -= ("Jerry", "Bob")

// ++= --= 与其他集合相联系
dict ++= List(("Tom", 91), ("Jerry", 87))
dict --= List("Jerry", "Bob")
```

6. `::` 与 `:::` 创建列表
```scala
1::3::5::Nil  // List[Int] = List(1, 3, 5)
```

注意 `::` 是右结合的：`(1::(3::(5::Nil)))` 。

```scala
// ::: 用来连接列表
val L4 = L3 ::: List("Hadoop", "Hbase")
```

#### 关于数据结构的讨论（List or Array?）

- 多用 List 而非 Array
- 列表的结构是递归的（即链表，`linkedList`），而数组是平等的

参考：
- [scala中List、Array、ListBuffer、ArrayList、Set、元组区别](https://blog.csdn.net/mar_ljh/article/details/81910286)
- [Scala学习笔记5 （集合 Collections）](https://blog.csdn.net/lyrebing/article/details/20362227)
