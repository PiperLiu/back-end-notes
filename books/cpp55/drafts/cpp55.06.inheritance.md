# 6 | 继承与面型对象设计

Inheritance and Object-Oriented Design

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [32 | 确定你的 public 继承塑模出 is-a 关系](#32-确定你的-public-继承塑模出-is-a-关系)
- [33 | 避免遮掩继承而来的名称](#33-避免遮掩继承而来的名称)
  - [子类的名称会遮掩基类的名称](#子类的名称会遮掩基类的名称)
  - [使用 using 声明或者转交函数 forwarding functions 可以让名称取消遮掩](#使用-using-声明或者转交函数-forwarding-functions-可以让名称取消遮掩)
- [34 | 区分接口继承和实现继承](#34-区分接口继承和实现继承)
- [35 | 考虑 virtual 函数以外的其他选择](#35-考虑-virtual-函数以外的其他选择)

<!-- /code_chunk_output -->

### 32 | 确定你的 public 继承塑模出 is-a 关系

Make sure public inheritance models "is-a."

"public 继承"意味 is-a 。适用于 base classes 身上的每一件事情也适用于 derived classes 身上，因为每一个 derived class 对象也都是一个 base class 对象。

### 33 | 避免遮掩继承而来的名称

Avoid hiding inherited names.

#### 子类的名称会遮掩基类的名称

```cpp
class Base {
private:
    int x;
public:
    virtual void mf1() = 0;
    virtual void mf1(int);
    virtual void mf2();
    void mf3();
    void mf3(double);
    ...
};
class Derived: public Base {
public:
    virtual void mf1();
    void mf3();
    void mf4();
    ...
};

Derived d;
int x;
...
d.mf1();  // 没问题，调用 Derived::mf1
d.mf1(x); // 错误，因为 Derived::mf1 遮掩了 Base::mf1
d.mf2();  // 没问题 调用 Base::mf2
d.mf3();  // 没问题 调用 Derived::mf3
d.mf3(x); // 错误， Derived::mf3 遮掩了 Base::mf3
```

#### 使用 using 声明或者转交函数 forwarding functions 可以让名称取消遮掩

使用 using 如下。

```cpp
class Base {
private:
    int x;
public:
    virtual void mf1() = 0;
    virtual void mf1(int);
    virtual void mf2();
    void mf3();
    void mf3(double);
    ...
};
class Derived: public Base {
public:
    using Base::mf1;  // 让 Base 内的东西在 Derived 作用域可见
    using Base::mf3;
    virtual void mf1();
    void mf3();
    void mf4();
    ...
};
```

假设 Derived 以 private 形式继承 Base ，而 Derived 唯一想继承的 mf1 是那个无参数版本。 using 声明式在这里排不上用场，因为 using 声明式会令继承而来的某给定名称之所有同名函数在 derived class 中都可见。

此时使用转交函数 forwarding function ：

```cpp
class Base {
public:
    virtual void mf1() = 0;
    virtual void mf1(int);
    ...
};
class Derived: private Base {
public:
    virtual void mf1()
    { Base::mf1(); }  // 转交函数暗自成为 inline
}

// 可以调用 d.mf1() ，调用 d.mf1() 是错误的
```

### 34 | 区分接口继承和实现继承

Differentiate between inheritance of interface and inheritance of implementation.

可以用 protected 实现接口的提供和缺省的实现：

```cpp
class Airplane {
public:
  virtual void fly(const Airport& destination) = 0;
  ...
protected:
  void defaultFly(const Airport& destination);
};
void Airplane::defaultFly(const Airport& destination)
{
  // default code for flying an airplane to the given destination
}
class ModelA: public Airplane {
public:
  virtual void fly(const Airport& destination)
  { defaultFly(destination); }
  ...
};
class ModelB: public Airplane {
public:
  virtual void fly(const Airport& destination)
  { // ModelX's special implementation }
  ...
};
```

纯虚函数也是可以定义的，体现如下。

```cpp
class Airplane {
public:
  virtual void fly(const Airport& destination) = 0;
  ...
};
void Airplane::fly(const Airport& destination)
{
  // default code for flying an airplane to the given destination
}
class ModelA: public Airplane {
public:
  virtual void fly(const Airport& destination)
  { Airplane::fly(destination); }
  ...
};
class ModelB: public Airplane {
public:
  virtual void fly(const Airport& destination)
  { // ModelX's special implementation }
  ...
};
```

上述两个设计是差不多的，后者中声明表现的是接口，定义部分表现出缺省行为。但是合并了 fly 和 defaultFly 丧失了“让两个函数享有不同保护级别”的机会。

### 35 | 考虑 virtual 函数以外的其他选择

Consider alternatives to virtual functions.

