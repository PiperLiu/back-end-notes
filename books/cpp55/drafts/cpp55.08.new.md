# 8 | 定制 new 和 delete

Customizing new and delete

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [49 | 了解 new-handler 的行为](#49-了解-new-handler-的行为)
  - [new 的错误处理函数 new-handler](#new-的错误处理函数-new-handler)
  - [设计专属的 set_new_handler](#设计专属的-set_new_handler)
  - [设计模板化的 new_handler](#设计模板化的-new_handler)
  - [nothrow 形式：分配失败返回 null](#nothrow-形式分配失败返回-null)
- [50 | 了解 new 和 delete 的合理替换时机](#50-了解-new-和-delete-的合理替换时机)
- [51 | 编写 new 和 delete 时需固守常规](#51-编写-new-和-delete-时需固守常规)
  - [即便客户要求 0 bytes ， new 也得返回一个合法指针](#即便客户要求-0-bytes-new-也得返回一个合法指针)
- [52 | 写了 placement new 也要写 placement delete](#52-写了-placement-new-也要写-placement-delete)
  - [考虑 placement new 异常的情况](#考虑-placement-new-异常的情况)
  - [placement new 会覆盖正常的 new](#placement-new-会覆盖正常的-new)

<!-- /code_chunk_output -->

### 49 | 了解 new-handler 的行为

Understand the behavior of the new-handler.

#### new 的错误处理函数 new-handler

我们想指定 new 抛出的异常，就是指定 new-handler 。如何指定呢？使用 set_new_handler 。

```cpp
namespace std {
    typedef void (*new_handler) ( );
    new_handler set_new_handler(new_handler p) throw();
}
```

new_handler 是一个函数指针，传参为空，返回值为空。

使用举例：

```cpp
void outOfMem()
{
    std::cerr << "Unable to satisfy request for memory\n";
    std::abort();
}

int main()
{
    std::set_new_handler(outOfMem);
    int* pBigDataArray = new int[1000000000000L];
}
```

一个设计良好的 new-handler 应该做以下事情：
- 让更多内容可被使用
- 安装另一个 new-handler
- 卸载 new-handler （使用完新 new-handler 后恢复旧的 new-handler）
- 抛出 bad_alloc 的异常
- 不返回，调用 abort 或 exit

#### 设计专属的 set_new_handler

```cpp
class Widget {
public:
    static std::new_handler set_new_handler(std::new_handler p) throw();
    static void* operator new(std::size_t size) throw(std::bad_alloc);
private:
    static std::new_handler currentHandler;
};

// static 必须再 class 外定义
// 在 class 实现文件内初始化为 null
std::new_handler Widget::currentHandler = 0;

// 注意这里返回旧的 handler 其实很妙
// 这是特化 Widget 的 new_handler
std::new_handler Widget::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler;
}

// 基于资源管理类实现一个 holder
class NewHandlerHolder {
public:
    explicit NewHandlerHolder(std::new_handler nh): handler(nh) {}
    // 这里在析构时，将 new_handler 恢复成初构造时的 handler
    ~NewHandlerHolder() {
        std::set_new_handler(handler);
    }
private:
    std::new_handler handler;
    NewHandlerHolder(const NewHandlerHolder&);  // 阻止 copying
    NewHandlerHolder& operator=(const NewHandlerHolder&);
}

// 基于以上实现一个 new 就很简单
void* Widget::operator new(std::size_t size) throw(std::bad_alloc)
{
    // 这里 h 的 handler 记录下来旧的 new_handler
    NewHandlerHolder h(std::set_new_handler(currentHandler));
    return ::operator new(size);
    // 这里 h 析构， new_handler 恢复为 new handler
}
```

如上就实现了一个可以在使用 Widget 的 new 时自动将 new_handler 设置为 Widget ，而使用结束后立即恢复的 global new_handler 的机制。

#### 设计模板化的 new_handler

```cpp
template<typename T>  // mixin 风格的 base class
class NewHandlerSupport {
public:
    static std::new_handler set_new_handler(std::new_handler p) throw();
    static void* operator new(std::size_t size) throw(std::bad_alloc);
    ...  // 其他的 operator new 版本 见条款 52
private:
    static std::new_handler currentHandler;
};

template<typename T>
std::new_handler
NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler;
}
template<typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size)
throw(std::bad_alloc)
{
    NewHandlerHolder h(std::set_new_handler(currentHandler));
    return ::operator new(size);
}
// 以下将每一个 currentHandler 初始化为 null
template<typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;
```

如何使用呢？使用 public 继承。

```cpp
class Widget: public NewHandlerSupport<Widget> {
    ... // 不用再声明 set_new_handler 或 operator new 了
}
```

#### nothrow 形式：分配失败返回 null

```cpp
class Widget { ... };
Widget* pw1 = new Widget;  // 分配失败抛出 bad_alloc
if (pw1 == 0) ...  // 一定不会发生 ==0
Widget* pw2 = new(std::nothrow) Widget;  // 分配失败返回 0
if (pw2 == 0) ... // 可能会发生
```

### 50 | 了解 new 和 delete 的合理替换时机

Understand when it makes sense to replace new and delete.

一般而言，齐位 alignment 意义重大，因为 C++ 要求所有 operator new 返回的指针都有适当的对齐（取决于数据类型）， malloc 就是在这种要求下工作的。

有许多理由需要写多个自定的 new 和 delete ，包括改善效能、对 heap 运用错误进行调试、收集 heap 使用信息。

### 51 | 编写 new 和 delete 时需固守常规

Adhere to convention when writing new and delete.

#### 即便客户要求 0 bytes ， new 也得返回一个合法指针

关于 non-member operator new 的伪码如下：

```cpp
void* operator new(std::size_t size) throw(std::bad_alloc)
{
    using namespace std;
    if (size == 0) {
        size = 1;
    }
    while (true) {
        尝试分配 size bytes;
        if (分配成功)
        return 一个指针，指向分配得来的内存;

        // 分配失败
        new_handler globalHandler = set_new_handler(0);
        set_new_handler(globalHandler);

        if (globalHandler) (*globalHandler)();
        else throw std::bad_alloc();
    }
}
```

此外，注意 sizeof(Base) 无论何时都不会是 0 。

### 52 | 写了 placement new 也要写 placement delete

Write placement delete if you write placement new.

如果 operator new 接受的参数除了一定会有的那个 size_t 之外还有其他，这便是个所谓的 palcement new 。如下。

```cpp
class Widget {
public:
    ...
    static void* operator new(std::size_t size, std::ostream& logStream)
        throw(std::bad_alloc);  // placement new
    static void operator delete(void* pMemory, std::size_t size)
        throw();  // 正常的 class 专属 delete
};
```

#### 考虑 placement new 异常的情况

```cpp
Widget* pw = new (std::cerr) Widget;
```

如山给，如果内存分配成功，而 Widget 构造函数抛出异常，运行期有责任取消 new 分配的内存。

但是这里 new 接受了额外实参，因此需要一个接受额外实参版本的 delete。

因此需要些 placement delete （在 Widget 内）：

```cpp
static void operator delete(void* pMemory. std::ostream& logStream)
    throw();
```

palcement delete 只有在“伴随 placement new 调用而触发的构造函数”出现异常时才会被调用。

#### placement new 会覆盖正常的 new

如果你只声明了 placement new ，请注意正常的 new 将无法使用。因此一般而言会做声明以及 using 正常的操作符：

```cpp
class Widget {
public:
    using StandardNewDeleteForms::operator new;
    using StandardNewDeleteForms::operator delete;
    static void* operator new(std::size_t size, std::ostream& logStream)
        throw(std::bad_alloc);  // placement new
    static void operator delete(void* pMemory. std::ostream& logStream)
        throw();
};
```
