# 一些基础语法收集

https://learn.microsoft.com/zh-cn/training/paths/get-started-c-sharp-part-1/

文档：
- https://learn.microsoft.com/zh-cn/dotnet/csharp/

<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [demical 数据结构](#demical-数据结构)
- [逐字字符串字面量@](#逐字字符串字面量)
- [字符串内插 format $](#字符串内插-format-)
  - [String.Format 与由地区决定的货币格式](#stringformat-与由地区决定的货币格式)
- [string 实例自带填充和对齐如 PadRight](#string-实例自带填充和对齐如-padright)
- [foreach 语句](#foreach-语句)
- [?问号表示可以为 null 的变量](#问号表示可以为-null-的变量)
- [数据结构自带的 MinValue 与 MaxValue](#数据结构自带的-minvalue-与-maxvalue)
- [int.Parse(a) 和 .ToString() ，这很 java](#intparsea-和-tostring-这很-java)
- [TryParse 和 out ，这很 C# （out参数：返回值多个，不限类型；类似传入引用）](#tryparse-和-out-这很-c-out参数返回值多个不限类型类似传入引用)
- [ref 传入 Array.Resize 用于传引用](#ref-传入-arrayresize-用于传引用)
- [“反常规”语法](#反常规语法)
- [多维数组使用逗号](#多维数组使用逗号)
- [指定函数参数使用冒号](#指定函数参数使用冒号)
- [一个异常处理的例子](#一个异常处理的例子)

<!-- /code_chunk_output -->

### demical 数据结构

```cs
var d1 = 1.0m;
var d2 = 1.00m;
Console.WriteLine(d1);       // 1.0
Console.WriteLine(d2);       // 1.00
Console.WriteLine(d1 == d2); // True
Console.WriteLine(d1 + d2);  // 2.00
Console.WriteLine(d1 + 1.00000m); // 2.00000
```

如上 demical 数据结构确实让我觉得有点奇怪。

大概这就是微软眼中的“高级语言”吧。

### 逐字字符串字面量@

```cs
Console.WriteLine(@"    c:\source\repos    
        (this is where your code goes)");
```

类似 Python 中的 `""" """` 吧。

### 字符串内插 format $

```cs
string message = $"{greeting} {firstName}!";

int saleAmount = 1001;
Console.WriteLine($"Discount: {(saleAmount > 1000 ? 100 : 50)}");
```

可以【合并逐字文本和字符串内插】：
```cs
string projectName = "First-Project";
Console.WriteLine($@"C:\Output\{projectName}\Data");
```

#### String.Format 与由地区决定的货币格式

```cs
decimal measurement = 123456.78912m;
Console.WriteLine($"Measurement: {measurement:N4} units");

Measurement: 123,456.7891 units

decimal tax = .36785m;
Console.WriteLine($"Tax rate: {tax:P2}");

Tax rate: 36.79 %

decimal price = 123.45m;
int discount = 50;
Console.WriteLine($"Price: {price:C} (Save {discount:C})");

Price: $123.45 (Save $50.00)
```

以上逗号在哪里、货币什么符号，由系统地区/语言决定。

```cs
decimal price = 67.55m;
decimal salePrice = 59.99m;

string yourDiscount = String.Format("You saved {0:C2} off the regular {1:C2} price. ", (price - salePrice), price);

Console.WriteLine(yourDiscount);

You saved $7.56 off the regular $67.55 price.
```

### string 实例自带填充和对齐如 PadRight

### foreach 语句

```cs
int[] inventory = { 200, 450, 700, 175, 250 };
int sum = 0;
int bin = 0;
foreach (int items in inventory)
{
    sum += items;
    bin++;
    Console.WriteLine($"Bin {bin} = {items} items (Running total: {sum})");
}
Console.WriteLine($"We have {sum} items in inventory.");
```

### ?问号表示可以为 null 的变量

```cs
string? readResult;
readResult = Console.ReadLine();
```

### 数据结构自带的 MinValue 与 MaxValue

```cs
Console.WriteLine("Signed integral types:");

Console.WriteLine($"sbyte  : {sbyte.MinValue} to {sbyte.MaxValue}");
Console.WriteLine($"short  : {short.MinValue} to {short.MaxValue}");
Console.WriteLine($"int    : {int.MinValue} to {int.MaxValue}");
Console.WriteLine($"long   : {long.MinValue} to {long.MaxValue}");

Console.WriteLine("");
Console.WriteLine("Unsigned integral types:");

Console.WriteLine($"byte   : {byte.MinValue} to {byte.MaxValue}");
Console.WriteLine($"ushort : {ushort.MinValue} to {ushort.MaxValue}");
Console.WriteLine($"uint   : {uint.MinValue} to {uint.MaxValue}");
Console.WriteLine($"ulong  : {ulong.MinValue} to {ulong.MaxValue}");

Signed integral types:
sbyte  : -128 to 127
short  : -32768 to 32767
int    : -2147483648 to 2147483647
long   : -9223372036854775808 to 9223372036854775807

Unsigned integral types:
byte   : 0 to 255
ushort : 0 to 65535
uint   : 0 to 4294967295
ulong  : 0 to 18446744073709551615

Console.WriteLine("");
Console.WriteLine("Floating point types:");
Console.WriteLine($"float  : {float.MinValue} to {float.MaxValue} (with ~6-9 digits of precision)");
Console.WriteLine($"double : {double.MinValue} to {double.MaxValue} (with ~15-17 digits of precision)");
Console.WriteLine($"decimal: {decimal.MinValue} to {decimal.MaxValue} (with 28-29 digits of precision)");

Floating point types:
float  : -3.402823E+38 to 3.402823E+38 (with ~6-9 digits of precision)
double : -1.79769313486232E+308 to 1.79769313486232E+308 (with ~15-17 digits of precision)
decimal: -79228162514264337593543950335 to 79228162514264337593543950335 (with 28-29 digits of precision)
```

### int.Parse(a) 和 .ToString() ，这很 java

还有 `Convert.ToInt32(value1)` 更加灵活；这种转换会 round ，并不截断。

### TryParse 和 out ，这很 C# （out参数：返回值多个，不限类型；类似传入引用）

```cs
string value = "bad";
int result = 0;
if (int.TryParse(value, out result))
{
   Console.WriteLine($"Measurement: {result}");
}
else
{
   Console.WriteLine("Unable to report the measurement.");
}

if (result > 0)
   Console.WriteLine($"Measurement (w/ offset): {50 + result}");
```

ms learn 重的这个例子不是很好。我觉得[这篇](https://www.cnblogs.com/huangxuQaQ/p/10726513.html)写得还行：

```cs
static void Main(string[] args)
{
    int[] nums = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int max;
    int min;
    int sum;
    int avg;
    string s;
    Test(nums, out max, out min, out sum, out avg, out s);
    Console.WriteLine(max);
    Console.WriteLine(min);
    Console.WriteLine(sum);
    Console.WriteLine(avg);
    Console.WriteLine(s);
    Console.ReadKey();
}

public static void Test(int[] nums, out int max, out int min, out int sum, out int avg, out string s)//5个out参数修饰的是多余的返回值
{
    //out参数必须在方法内部为其赋值，否则返回去没有意义
    max = nums[0];
    min = nums[0];
    sum = 0;
    for (int i = 0; i < nums.Length; i++)
    {
        if (nums[i] > max)
        {
            max = nums[i];
        }
        if (nums[i] < min)
        {
            min = nums[i];
        }
        sum += nums[i];
    }
    avg = sum / nums.Length;
    //此方法void无返回值，无需写return
    s = "Test_Result";
}
```

调用方法之前，对out参数传递的变量只需声明，可以赋值也可以不赋值，赋值也会在方法中被覆盖掉。

使用out参数传递变量时，必须在方法内为其赋值，否则return的返回值没有意义。

方法的参数使用out修饰时，调用该方法时也要加上out关键字。

### ref 传入 Array.Resize 用于传引用

其实 C# 默认也是传引用（对于非值类型），这点应该与 java 大体上保持一致，不能是反直觉的。但是 C# 提供了 `in` `out` `ref` 关键字让事实更加清晰。

```cs
string[] pallets = { "B14", "A11", "B12", "A13" };
Console.WriteLine("");

Array.Clear(pallets, 0, 2);
Console.WriteLine($"Clearing 2 ... count: {pallets.Length}");
foreach (var pallet in pallets)
{
    Console.WriteLine($"-- {pallet}");
}

Console.WriteLine("");
Array.Resize(ref pallets, 6);
Console.WriteLine($"Resizing 6 ... count: {pallets.Length}");

pallets[4] = "C01";
pallets[5] = "C02";

foreach (var pallet in pallets)
{
    Console.WriteLine($"-- {pallet}");
}

Console.WriteLine("");
Array.Resize(ref pallets, 3);
Console.WriteLine($"Resizing 3 ... count: {pallets.Length}");

foreach (var pallet in pallets)
{
    Console.WriteLine($"-- {pallet}");
}

Clearing 2 ... count: 4
--
--
-- B12
-- A13

Resizing 6 ... count: 6
--
--
-- B12
-- A13
-- C01
-- C02

Resizing 3 ... count: 3
--
--
-- B12
```

与 go 不同，默认“数组”容器在函数参数中，是传递引用的。

```cs
int[] array = {1, 2, 3, 4, 5};

PrintArray(array);
Clear(array);
PrintArray(array);

void PrintArray(int[] array) 
{
    foreach (int a in array) 
    {
        Console.Write($"{a} ");
    }
    Console.WriteLine();
}

void Clear(int[] array) 
{
    for (int i = 0; i < array.Length; i++) 
    {
        array[i] = 0;
    }
}

1 2 3 4 5 
0 0 0 0 0
```

如果是 go 想要传递切片引用，则需要 `foo(&arr)` 。

### “反常规”语法

### 多维数组使用逗号

```cs
string[,] corporate = // 二维数组
{
    {"Robert", "Bavin"}, {"Simon", "Bright"},
    {"Kim", "Sinclair"}, {"Aashrita", "Kamath"},
    {"Sarah", "Delucchi"}, {"Sinan", "Ali"}};

string[,] external = 
{
    {"Vinnie", "Ashton"}, {"Cody", "Dysart"},
    {"Shay", "Lawrence"}, {"Daren", "Valdes"}
};
```

### 指定函数参数使用冒号

```cs
string externalDomain = "hayworth.com";

for (int i = 0; i < corporate.GetLength(0); i++) 
{
    DisplayEmail(first: corporate[i,0], last: corporate[i,1]);  // 注意这里
}

for (int i = 0; i < external.GetLength(0); i++) 
{
    DisplayEmail(first: external[i,0], last: external[i,1], domain: externalDomain);
}

void DisplayEmail(string first, string last, string domain = "contoso.com") 
{
    string email = first.Substring(0, 2) + last;
    email = email.ToLower();
    Console.WriteLine($"{email}@{domain}");
}
```

### 一个异常处理的例子

```cs
try
{
    OperatingProcedure1();
}
catch (Exception ex)
{
    Console.WriteLine(ex.Message);
    Console.WriteLine("Exiting application.");
}

static void OperatingProcedure1()
{
    string[][] userEnteredValues = new string[][]
    {
        new string[] { "1", "two", "3"},
        new string[] { "0", "1", "2"}
    };

    foreach(string[] userEntries in userEnteredValues)
    {
        try
        {
            BusinessProcess1(userEntries);
        }
        catch (Exception ex)
        {
            if (ex.StackTrace.Contains("BusinessProcess1"))
            {
                if (ex is FormatException)
                {
                    Console.WriteLine(ex.Message);
                    Console.WriteLine("Corrective action taken in OperatingProcedure1");
                }
                else if (ex is DivideByZeroException)
                {
                    Console.WriteLine(ex.Message);
                    Console.WriteLine("Partial correction in OperatingProcedure1 - further action required");

                    // re-throw the original exception
                    throw;
                }
                else
                {
                    // create a new exception object that wraps the original exception
                    throw new ApplicationException("An error occurred - ", ex);
                }
            }
        }

    }
}

static void BusinessProcess1(String[] userEntries)
{
    int valueEntered;

    foreach (string userValue in userEntries)
    {
        try
        {
            valueEntered = int.Parse(userValue);

            checked
            {
                int calculatedValue = 4 / valueEntered;
            }
        }
        catch (FormatException)
        {
            FormatException invalidFormatException = new FormatException("FormatException: User input values in 'BusinessProcess1' must be valid integers");
            throw invalidFormatException;
        }
        catch (DivideByZeroException)
        {
            DivideByZeroException unexpectedDivideByZeroException = new DivideByZeroException("DivideByZeroException: Calculation in 'BusinessProcess1' encountered an unexpected divide by zero");
            throw unexpectedDivideByZeroException;

        }
    }
}

FormatException: User input values in 'BusinessProcess1' must be valid integers
Corrective action taken in OperatingProcedure1
DivideByZeroException: Calculation in 'BusinessProcess1' encountered an unexpected divide by zero
Partial correction in OperatingProcedure1 - further action required
DivideByZeroException: Calculation in 'BusinessProcess1' encountered an unexpected divide by zero
Exiting application.
```
