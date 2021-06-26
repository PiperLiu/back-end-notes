# 大数据技术生态略写与Spark历史

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [从『脚本 + MySQL』到『Hadoop + Hive』](#从脚本-mysql到hadoop-hive)
- [比 Hadoop 更快的 Spark](#比-hadoop-更快的-spark)
- [为何 Spark 会比 Hadoop 快？](#为何-spark-会比-hadoop-快)
- [为什么大数据领域技术大多基于 Java 而非 C++ ？](#为什么大数据领域技术大多基于-java-而非-c)
  - [解释一下：宁可移动计算，不要移动数据](#解释一下宁可移动计算不要移动数据)
  - [简单聊一下 Java 序列化](#简单聊一下-java-序列化)
- [Spark 简单历史](#spark-简单历史)
- [Spark 与大数据生态](#spark-与大数据生态)

<!-- /code_chunk_output -->

### 从『脚本 + MySQL』到『Hadoop + Hive』

参考《Spark最佳实践》，作者在2008年进入腾讯，要管理上百台服务器，面临问题有二：
- 服务器产生日志文件导致容量不够，经常半夜被电话吵醒删数据
- 大量的数据统计（当时的解决方案：选择一个字段对数据进行散列化，然后按散列值的不同分别存储在单独的 MySQL 数据表中，这些 MySQL 节点是分布在上百台机器上的；需要分析时，再写脚本去各个节点取数据）

上述过程导致了：
- 节点承载计算量不均匀，有些节点成为瓶颈
- 开发成本高

于是引入了 Hadoop + Hive 解决方案。 Hadoop 让各个节点数据分布均匀；此外开发效率提升。但是，作者指出：**Hive 的计算效率不是很高，运行时间可能是『脚本 + MySQL』的数倍。** 不再使用该技术的更主要的原因为：人们的计算需求的复杂度越来越高，难以用 Hive 中的 SQL 语句解决。

### 比 Hadoop 更快的 Spark

Spark 更快、开发更舒适。

比如， Hadoop MR （MapReduce，大数据分布式计算/并行计算的重要概念）编程中的示例程序 WordCount ，在 Hadoop 下用 Java 实现需要上百行，且需要编译、上传执行；而 Spark 下可以直接用 Scala 的交互特性直接操作，并且，代码很短：

```java
text_file = spark.textFile("hdfs://...")
text_file.flatMap(lambda line: line.split())
    .map(lambda word: (word, 1))
    .reduceByKey(lambda a, b: a + b)
```

### 为何 Spark 会比 Hadoop 快？

《Spark最佳实践》中提到： Spark 更多地使用内存（在一开始被称为内存计算）。此外，作者陈欢、林世飞还认为， Spark 定义了核心数据结构： **RDD 弹性分布式数据集** 。相比之下， Hadoop 没有核心数据结构，只是实现了 MapReduce 计算模型。

这样， Spark 任务粒度更小，“原先在 Hadoop 下一个 Map 或 Reduce 实现的功能，在 Spark 下可能被拆分为多个 Job 。”

### 为什么大数据领域技术大多基于 Java 而非 C++ ？

《Spark最佳实践》作者陈欢、林世飞猜测有三点原因：
- Java 跨平台、生态好、在大型系统构建方面优势大、基于 Java 的 Scala 语言精简
- 大数据场景下系统的瓶颈大多不在计算上，而是传输和读写
- Java 序列化特性可以在“宁可移动计算，不要移动数据”中发挥优势

#### 解释一下：宁可移动计算，不要移动数据

参考资料：
- [移动计算比移动数据更划算](https://blog.csdn.net/huzai9527/article/details/88379463)
- ["移动数据"不如"移动计算"?](https://blog.csdn.net/qq_31024823/article/details/91972215)

数据是庞大的，程序比数据小的多，显然将数据输入给程序是不划算的，那么反其而行，将程序分发到数据所在的地方进行计算，岂不划算？

- 移动计算就是把计算任务下发到数据所在的节点进行处理。
- 移动数据就是将数据移动到计算任务的节点，这样将损耗大量网络开销，导致流量激增，处理效率慢。

#### 简单聊一下 Java 序列化

参考资料：
- [Java 序列化](https://www.runoob.com/java/java-serialization.html)

Java 提供了一种对象序列化的机制，该机制中，一个对象可以被表示为一个字节序列，该字节序列包括该对象的数据、有关对象的类型的信息和存储在对象中数据的类型。

将序列化对象写入文件之后，可以从文件中读取出来，并且对它进行反序列化，也就是说，对象的类型信息、对象的数据，还有对象中的数据类型可以用来在内存中新建对象。

整个过程都是 Java 虚拟机（JVM）独立的，也就是说，在一个平台上序列化的对象可以在另一个完全不同的平台上反序列化该对象。

类 `ObjectInputStream` 和 `ObjectOutputStream` 是高层次的数据流，它们包含反序列化和序列化对象的方法。

`ObjectOutputStream` 类包含很多写方法来写各种数据类型，但是一个特别的方法例外：

```java
public final void writeObject(Object x) throws IOException
```

上面的方法序列化一个对象，并将它发送到输出流。相似的 `ObjectInputStream` 类包含如下反序列化一个对象的方法：

```java
public final Object readObject() throws IOException,
                                 ClassNotFoundException
```

该方法从流中取出下一个对象，并将对象反序列化。它的返回值为 `Object` ，因此，你需要将它转换成合适的数据类型。

为了演示序列化在 Java 中是怎样工作的，我将使用之前教程中提到的 `Employee` 类，假设我们定义了如下的 `Employee` 类，该类实现了 `Serializable` 接口。

```java
public class Employee implements java.io.Serializable
{
   public String name;
   public String address;
   // 该属性是短暂的，SSN 值不会被发送到输出流
   public transient int SSN;
   public int number;
   public void mailCheck()
   {
      System.out.println("Mailing a check to " + name
                           + " " + address);
   }
}
```

请注意，一个类的对象要想序列化成功，必须满足两个条件：

- 该类必须实现 `java.io.Serializable` 接口。
- 该类的所有属性必须是可序列化的。如果有一个属性不是可序列化的，则该属性必须注明是短暂的。

如果你想知道一个 Java 标准类是否是可序列化的，请查看该类的文档。检验一个类的实例是否能序列化十分简单， 只需要查看该类有没有实现 `java.io.Serializable` 接口。

当序列化一个对象到文件时， 按照 Java 的标准约定是给文件一个 .ser 扩展名。

```java
// 序列化，并且保存到磁盘
import java.io.*;
 
public class SerializeDemo
{
   public static void main(String [] args)
   {
      Employee e = new Employee();
      e.name = "Reyan Ali";
      e.address = "Phokka Kuan, Ambehta Peer";
      e.SSN = 11122333;
      e.number = 101;
      try
      {
         FileOutputStream fileOut =
         new FileOutputStream("/tmp/employee.ser");
         ObjectOutputStream out = new ObjectOutputStream(fileOut);
         out.writeObject(e);
         out.close();
         fileOut.close();
         System.out.printf("Serialized data is saved in /tmp/employee.ser");
      }catch(IOException i)
      {
          i.printStackTrace();
      }
   }
}

// 反序列化，先从磁盘读取
import java.io.*;
 
public class DeserializeDemo
{
   public static void main(String [] args)
   {
      Employee e = null;
      try
      {
         FileInputStream fileIn = new FileInputStream("/tmp/employee.ser");
         ObjectInputStream in = new ObjectInputStream(fileIn);
         e = (Employee) in.readObject();
         in.close();
         fileIn.close();
      }catch(IOException i)
      {
         i.printStackTrace();
         return;
      }catch(ClassNotFoundException c)
      {
         System.out.println("Employee class not found");
         c.printStackTrace();
         return;
      }
      System.out.println("Deserialized Employee...");
      System.out.println("Name: " + e.name);
      System.out.println("Address: " + e.address);
      System.out.println("SSN: " + e.SSN);
      System.out.println("Number: " + e.number);
    }
}

// 反序列化结果
Deserialized Employee...
Name: Reyan Ali
Address:Phokka Kuan, Ambehta Peer
SSN: 0
Number:101
```

`readObject()` 方法中的 `try/catch` 代码块尝试捕获 `ClassNotFoundException` 异常。对于 JVM 可以反序列化对象，它必须是能够找到字节码的类。如果JVM在反序列化对象的过程中找不到该类，则抛出一个 `ClassNotFoundException` 异常。

注意，`readObject()` 方法的返回值被转化成 `Employee` 引用。

当对象被序列化时，属性 `SSN` 的值为 `111222333`，但是因为该属性是短暂的，该值没有被发送到输出流。所以反序列化后 `Employee` 对象的 `SSN` 属性为 `0`。

### Spark 简单历史

参考资料：
- [Spark的历史与发展（目录）](https://www.cnblogs.com/xia520pi/p/8609938.html)

2009年， Spark 诞生于伯克利大学 AMPLab ，最开初属于伯克利大学的研究性项目。它于2010年正式开源，并于2013年成为了 Apache 基金项目，并于2014年成为 Apache 基金的顶级项目，整个过程不到五年时间。

### Spark 与大数据生态

参考《Spark最佳实践》：
- Spark 在参考 MapReduce 的机厂商重新实现了一套通用计算框架
- Spark 支持类似 Storm 的实时流式计算和图计算
- Spark 可以代替 Hadoop 的 MapReduce 计算
- Spark SQL 可以替代 Hive
- Spark 存储依然使用 Hadoop HDFS
