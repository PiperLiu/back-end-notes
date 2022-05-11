# 1 | 型别推导

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1 | 理解模板型别推导](#1-理解模板型别推导)
  - [ParamType 是个指针或引用，但不是个万能引用](#paramtype-是个指针或引用但不是个万能引用)
  - [ParamType 是个万能引用](#paramtype-是个万能引用)
  - [ParamType 既非指针也非引用](#paramtype-既非指针也非引用)
  - [数组实参](#数组实参)
- [函数实参](#函数实参)
- [2 | 理解 auto 型别推导](#2-理解-auto-型别推导)
  - [C++ 11 支持统一初始化 uniform initialization （大括号）](#c-11-支持统一初始化-uniform-initialization-大括号)
  - [auto 型别推导就是模板推导，只有一个例外](#auto-型别推导就是模板推导只有一个例外)
  - [C++ 14 运行 auto 作为返回值，但使用的是模板推导](#c-14-运行-auto-作为返回值但使用的是模板推导)

<!-- /code_chunk_output -->

### 1 | 理解模板型别推导

```cpp
template<typename T>
void f(ParamType param);

f(expr)
```

我们将讨论 ParamType 不同的情况下， expr 将导致 ParamType 的具体型别推导。

#### ParamType 是个指针或引用，但不是个万能引用

```cpp
template<typename T>
void f(T& param);

int x = 27;             // x int
const int cx = x;       // cx const int
const int& rx = x;      // rx 是 const int&

f(x);  // T 型别是 int ， param 型别是 int&
f(cx); // T 型别是 const int ， param 型别是 const int&
f(rx); // T 型别是 const int ， param 型别是 const int&
```

注意， const 常量性也会成为型别推到的部分。

```cpp
template<typename T>
void f(const T& param);

int x = 27;             // x int
const int cx = x;       // cx const int
const int& rx = x;      // rx 是 const int&

f(x);  // T 型别是 int ， param 型别是 const int&
f(cx); // T 型别是 int ， param 型别是 const int&
f(rx); // T 型别是 int ， param 型别是 const int&
```

指针也没有什么差别。

```cpp
template<typename T>
void f(T* param);

int x = 27;
const int *px = &x;  // px 是 const int

f(&x);  // T 型别是 int ， param 型别是 int*
f(px);  // T 型别是 const int ， param 型别是 const int*
```

#### ParamType 是个万能引用

expr 是左值时， T 和 ParamType 都被推导为左值引用。

```cpp
template<typename T>
void f(T&& param);  // param 现在是个万能引用

int x = 27;
const int cx = x;
const int& rx = x;

f(x);   // x 是左值，所以 T 的型别是 int& ， param 型别 int&
f(cx);  // cx 左值， T const int&
        // param const int&
f(rx);  // rx 左值， T const int&
        // param const int&
f(27);  // 27 右值， T int
        // 此时 param int&&
```

条款 24 会解释为何出现如上结果。

#### ParamType 既非指针也非引用

那就按值传递了。这意味着传递一个 **副本** 。

```cpp
template<typename T>
void f(T param);

int x = 27;
const int cx = x;
const int& rx = x;

f(x);  // T 和 param int
f(cx); // T 和 param int
f(rx); // T 和 param int
```

const 和 volatile 会在按值形参处被忽略，但是指针或引用不会。

```cpp
template<typename T>
void f(T param);

const char* const ptr = 
"Fun with pointers";
/**
 * 这里 * 左边的 const 表示字符串这块内存不能被修改，
 * * 右边表示 ptr 不可以指涉其他内存位置或 null
 */

f(ptr);  // 传递类型为 const char * const 实参
// 如上， const 没有被忽略
```

#### 数组实参

注意，在 C 中数组或许和指针等价， C++ 不是。

```cpp
const char name[] = "J. P. Briggs";  // name 的型别是 const char[13]
const char * ptrToName = name;       // 数组退化成指针

template<typename T>
void f(T param);  // 按值形参的模板
f(name);          // T 将被推导为 const char *

template<typename T>
void g(T& param);  // 按引用方式
g(name);          // T 将被推导为 const char [13]
```

有趣的来了，可以用这个特性来做一个模板函数，推导出数组中含有的元素个数：

```cpp
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) {
    return N;
}

// 应用如下
int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
int mappedVals[arraySize(keyVals)];
std::array<int, arraySize(keyVals)> mappedVals;
```

我有个问题：这样会导致编译后体积过于膨胀吗？可以在运行时应用吗？

```cpp
int main()
{
    int n = 3;
    int a[] = {1, 2, 3};   // 不会报错
    int a[n] = {1, 2, 3};  // no matching function for call to 'arraySize(int [n])
    int x = arraySize(a);
    return 0;
}
```

我目前能给出的答案是：不会，不可以在运行时应用。首先模板是在编译器使用的，用于生成非模板代码。另外， C++ 是静态类型语言，因此编译器各个变量类型都是要确定的，所以这里的 arraySize 相当于宏，换句话说，即便不用模板，我们也可以在写代码时知道要确定的数组大小。

### 函数实参

函数也可以退化为指针。

```cpp
void someFunc(int, double);

template<typename T>
void f1(T param);

template<typename T>
void f2(T& param);

f1(someFunc);  // param 型别是 void (*)(int, double)
f2(someFunc);  // param 型别是 void (&)(int, double)
```

### 2 | 理解 auto 型别推导

#### C++ 11 支持统一初始化 uniform initialization （大括号）

```cpp
int x1 = 27;
int x2(27);
// 如下是 C++ 11 新增
int x3{27};
int x4 = {27};
```

#### auto 型别推导就是模板推导，只有一个例外

所有推导皆与模板推导一致，只有 `std::initilizer_list<int>` 不同。

```cpp
auto x1 = 27;  // int
auto x2(27);   // int
auto x3 = {27};  // std::initializer_list<int>
auto x4{27};     // std::initializer_list<int>
```

下面是一些无法推导或者与模板行为不符的情况。

```cpp
auto x5 = { 1, 2, 3.0 };  // 错误
                          // 因为无法推导出 std::initializer_list<T> 中的 T
```

如下代码不能通过编译。

```cpp
template<typename T>
void f(T param);

f({ 11, 23, 9 });  // 错误，推导不出 T

auto x = { 11, 23, 9 };  // 这个可以推导出 std::initializer_list<int> 哦
```

如下代码则可以通过编译。

```cpp
template<typename T>
void f(std::initializer_list<T> initList);

f({ 11, 23, 9 });  // 没有问题， T 被推导为 int
```

#### C++ 14 运行 auto 作为返回值，但使用的是模板推导

C++ 14 中运行 auto 作为返回值，也允许在 lambda 式中形参为 auto 。但是注意这里 auto 的推导是模板推导，无法处理 `std::initializer_list<T>` 。

```cpp
auto createInitList()
{
    return { 1, 2, 3 };  // 错误，无法推导型别
}

std::vector<int> v;
auto resetV =  // C++ 14
    [&v](const auto& newValue) { v = newValue; };
resetV({ 1, 2, 3 });  // 错误，无法为 {1, 2, 3} 完成型别推导
```