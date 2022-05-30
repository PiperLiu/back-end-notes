# 8 | 微调

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [41 | 针对可复制的形参，在移动成本低并且一定会被复制的前提下，考虑将其按值传递](#41-针对可复制的形参在移动成本低并且一定会被复制的前提下考虑将其按值传递)
  - [与 C++ 98 不同， C++ 11 中传入右值，则移动构造](#与-c-98-不同-c-11-中传入右值则移动构造)
- [42 | 考虑置入而非插入](#42-考虑置入而非插入)
  - [考虑使用完美转发的置入往往效率更高](#考虑使用完美转发的置入往往效率更高)
  - [资源管理时使用置入需谨慎考虑](#资源管理时使用置入需谨慎考虑)
  - [与 explicit 构造函数联系需谨慎，以正则 std::regex 为例](#与-explicit-构造函数联系需谨慎以正则-stdregex-为例)

<!-- /code_chunk_output -->

### 41 | 针对可复制的形参，在移动成本低并且一定会被复制的前提下，考虑将其按值传递

#### 与 C++ 98 不同， C++ 11 中传入右值，则移动构造

```cpp
class Widget {
public:
    void addName(std::string newName) {
        names.push_back(std::move(newName));
    }
}
```

这时，不免让人疑惑，为什么不重载 `const std::string& /&& newName` 来书写左值右值对应的方法；或者使用 `T&& 配合 forward<T>` 做完美转发呢？

首先，重载的代码过于冗余，而之前也讨论过完美转发有很多不可用的场景。 **最重要的是，在这里使用值传递，成本不高。**

在 C++ 98 中，我们无论传入什么，形参 `newName` 都会经由复制构造函数创建。但是， **在 C++ 11 中， `newName` 仅在传入左值时才会被复制构造。而如果传入的是个右值，它会被移动构造。**

```cpp
Widget w;
...
std::string name("Bart");

w.addName(name);  // 传入左值
w.addName(name + " Jenne");  // 传入右值
```

所以，如果使用重载或者万能引用，传入左值时代价是一次复制，传入右值时代价是一次移动。而如果使用传值，传入左值时代价是一次复制加一次移动，传入右值时代价是两次移动。

注意，正如本条款标题，并不是所有情况都适于传递值。

### 42 | 考虑置入而非插入

可以把插入理解为 `push_back` ，而置入是 `emplace_back` 相关函数。这里有个对照关系，比如 `push_front` 与 `emplace_front` 、 `emplace_after` 与 `insert_after` 。

#### 考虑使用完美转发的置入往往效率更高

观察 `vector` 的 `push_back` 源码如下。

```cpp
template <class T,
          class Allocator = allocator<T>>
class vector {
public:
    ...
    void push_back(const T& x);  // 插入左值
    void push_back(T&& x);       // 插入右值
    ...
};
```

因此，我们必须给 `push_back` 传入 `T` 型别的对象。如果不是，那就隐式构造一个。观察下例。

```cpp
std::vector<std::string> vs;

vs.push_back("Bart");  // 注意 "Bart" 是个字面量，并非 string
// 上面的代码相当于
vs.push_back(std::string("Bart"));
```

也就是说， `vs.push_back("Bart");` 会经历如下步骤：
- 创建 `std::string("Bart")` 临时对象 `temp`
- 将 `temp` 传递给 `push_back` 右值重载版本，在 `vector` 内存中，还会根据 `temp` 复制构造一个 `std::string` 对象放在容器里
- `push_back` 结束， `temp` 就会被析构

有两次构造函数，一次析构函数的调用成本。

而如果使用 `emplace_back` 则没有这个问题。

`vs.emplace_back("Bart");` 会把 `"Bart"` 完美转发到 `vector` 内存中准备创建新对象的位置，在那里直接构造一个 `std::string` 对象。

**因此，在这个情形中，置入的成本是一次构造函数调用而已。**

此外，由于是完美转发，所以写作如下方式也没问题。

```cpp
vs.emplace_back(50, 'x');  // 置入一个 50 个 'x' 组成的 string 对象
```

另外，如果不是传入字面量（或其他特殊情形），二者性能其实没有差别。如下。

```cpp
std::string s("Bart");

// 下面二者静效果相同
vs.push_back(s);
vs.emplace_back(s);
```

因此总结一下，如果以下条件都成立，那么置入将几乎比插入高效：
- 欲添加的值是以构造而非赋值方式加入容器。因此如 `vs.emplace(vs.begin(), "Peter")` 往往采用移动赋值的手法，所以这里并不能体现出置入的高效性
- 传递的实参型别与容器持有之物型别不同。
- 容器不太可能由于出现重复情况而拒绝待添加的新值

#### 资源管理时使用置入需谨慎考虑

如下两个代码没有区别。

```cpp
// 因为自定义删除器
// 所以不使用 make_shared
ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
ptrs.push_back({new Widget, killWidget});
```

`push_back` 被调用之前，将有一个临时的 `temp` 对象（ `std::shared_ptr<Widget>(new Widget, killWidget)` ）被创建，其持有的是 `new Widget` 指针。

`push_back` 会按引用的方式接受 `temp` ，但在为链表分配内存以持有 `temp` 副本过程中，抛出了内存不足的异常，该异常传播到了 `push_back` 之外， `temp` 被析构，而 `killWidget` 被调用，没有空悬对象。

但是如果使用 `ptrs.emplace_back(new Widget, killWidget)` 呢？

由于 `new Widget` 被完美转发，而在节点上建立 `shared_ptr` 对象前发生了内存不足的异常，因此 `new Widget` 发生泄漏（因为没有作为唯一可以获取堆上对象的指针没有被获取）。

因此，最谨慎的方法应该如下，而这也会导致插入和置入性能没有差别。

```cpp
std::shared_ptr<Widget> spw(new Widget, killWidget);

ptrs.push_back(spw);
ptrs.emplace_back(spw);
```

#### 与 explicit 构造函数联系需谨慎，以正则 std::regex 为例

有时候 `emplace_back` 不会帮我们发现错误。如下。

```cpp
std::vector<std::regex> regexes;

// 如下不会报错
regexes.emplace_back(nullptr);

// 如下无法通过编译
regexes.push_back(nullptr);
```

为什么？因为 `std::regex` 对象构造函数带有 `explicit` 声明，阻止了隐式转换。

```cpp
regexes.push_back(nullptr);
// 上面相当于调用了如下的构造函数
std::regex r = nullptr;  // 错误！
```

但是， `std::regex` 构造函数却允许传入 `string` 以及 `const char *` 。所以下面的句子是能编译的。

```cpp
std::regex r(nullptr);
// 因此 emplace_back(nullptr) 可以正常工作
```

**因此注意，置入函数可能会执行在插入函数中被拒绝的型别转换。**
