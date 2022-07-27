# 11. Testing

Go provides a mechanism to update variables and inspect their values at run time, to call their methods, and to apply the operations intrinsic to their representation, all without knowing their types at compile time. This mechanism is called reflection. Reflection also lets us treat types themselves as first-class values.

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [12.1 Why Reflection （检测是否已存在 Stringer 方法）](#121-why-reflection-检测是否已存在-stringer-方法)
- [12.2 reflect.Type and reflect.Value](#122-reflecttype-and-reflectvalue)
- [12.3 Display, a Recursive Value Printer](#123-display-a-recursive-value-printer)
- [12.4 Example: Encoding S-Expressions](#124-example-encoding-s-expressions)
- [12.5 Setting Variables with reflect.Value](#125-setting-variables-with-reflectvalue)
- [12.6 Example: Decoding S-Expressions](#126-example-decoding-s-expressions)
- [12.7 Accessing Struct Field Tags](#127-accessing-struct-field-tags)
- [12.8 Displaying the Methods of a Type](#128-displaying-the-methods-of-a-type)
- [12.9 A Word of Caution](#129-a-word-of-caution)

<!-- /code_chunk_output -->

### 12.1 Why Reflection （检测是否已存在 Stringer 方法）

书中首先引出一个例子，如何自己实现 Sprint 方法。

```go
func Sprint(x interface{}) string {
    // 这里的 stringer 十分巧妙
    // 用于检查 x 是否有 String 方法
    type stringer interface {
        String() string
    }
    switch x := x.(type) {
    case stringer:
        return x.String()
    case string:
        return x
    case int:
        return strconv.Itoa(x)
    case bool:
        if x {
            return "true"
        }
        return "false"
    default:
        return "???"
    }
}
```

如上面临的问题是如果针对每个 type 都设置一种输出情况，那不可能。

### 12.2 reflect.Type and reflect.Value

```go
t := reflect.TypeOf(3)  // a reflect.Type
fmt.Println(t.String()) // "int"
fmt.Println(t)          // "int"

// reflect.TypeOf 是在运行期获取变量类型，因此总是获取具体类型
var w io.Writer = os.Stdout
fmt.Println(reflect.TypeOf(w)) // "*os.File"

// 关于 reflect.Value
v := reflect.ValueOf(3)  // a reflect.Value
fmt.Println(v.String()) // "3"
fmt.Printf("%v\n", v)   // "3"
fmt.Println(v.String()) // NOTE: "<int Value>"
```

此外还有一些其他方法，如下。

```go
v := reflect.ValueOf(3)
t := v.Type()  // a reflect.Type
fmt.Println(t.String()) // "int"

x := v.Interface() // an interface{}
i := x.(int)       // a int
fmt.Printf("%d\n", i)     // "3"
```

而 reflect.Invalid 表示一个无效的 reflect.Value 。

代码可以参考 [format](../gopl.io/ch12/format/format.go) 。

### 12.3 Display, a Recursive Value Printer

在 [display](../gopl.io/ch12/display/display.go) 中，利用反射很棒地对任意对象进行解构，比如 `reflect.Slice` `reflect.Struct` `reflect.Map` 。

### 12.4 Example: Encoding S-Expressions

### 12.5 Setting Variables with reflect.Value

```go
x := 2                    // value    type    variable?
a := reflect.ValueOf(2)   // 2        int     no
b := reflect.ValueOf(x)   // 2        int     no
c := reflect.ValueOf(&x)  // &x       *int    no 
d := c.Elem()             // 2        int     yes (x)
```

我的理解是， `a b c` 都相当于立即数一样，放在寄存器的值，当让不属于变量（没有地址）。

```go
fmt.Println(a.CanAddr())  // "false"
fmt.Println(b.CanAddr())  // "false"
fmt.Println(c.CanAddr())  // "false"
fmt.Println(d.CanAddr())  // "true"
```

一个有地址的“变量”，一般而言是可以被修改的。

```go
x := 2
d := reflect.ValueOf(&x).Elem()   // d refers to the variable x
px := d.Addr().Interface().(*int) // px := &x
*px = 3                           // x = 3
fmt.Println(x)                    // "3"
```

也可以使用 `reflect.Value.Set` 来设置变量。

```go
d.Set(reflect.ValueOf(3)) // x = 3
```

一些案例记录如下。

```go
d.Set(reflect.ValueOf(int64(5)))  // panic: int64 is not assignable to int

x := 2
b := reflect.ValueOf(x)
b.Set(reflect.ValueOf(3))  // panic: Set using unaddressable value

x := 1
rx := reflect.ValueOf(&x).Elem()
rx.SetInt(2)                      // OK, x = 2
rx.Set(reflect.ValueOf(3))        // OK, x = 3
rx.SetString("hello")             // panic: string is not assignable to int
rx.Set(reflect.ValueOf("hello"))  // panic: string is not assignable to int

var y interface{}
ry := reflect.ValueOf(&y).Elem()
ry.SetInt(2)                      // panic: SetInt called on interface Value
ry.Set(reflect.ValueOf(3))        // OK, y = int(3)
ry.SetString("hello")             // panic: SetString called on interface Value
ry.Set(reflect.ValueOf("hello"))  // OK, y = "hello"
```

要注意的是，可以获取地址未必意味着该值可以改变。

```go
stdout := reflect.ValueOf(os.Stdout).Elem()  // *os.Stdout, an os.File var
fmt.Println(stdout.Type())                   // "os.File"¬¬
fd := stdout.FieldByName("fd")
fmt.Println(fd.Int())                        // "1"
fd.SetInt(2)                                 // panic: unexported field

fmt.Println(fd.CanAddr(), fd.CanSet())       // "true false"
```

### 12.6 Example: Decoding S-Expressions

### 12.7 Accessing Struct Field Tags

在例子 [search](../gopl.io/ch12/search/main.go) 中利用 [params](../gopl.io/ch12/params/params.go) 解析 field tag ，建立起一个服务，可以将 tag 中对应的字段放到对应的 field 元素里。

核心代码如下。

```go
// search
// search implements the /search URL endpoint.
func search(resp http.ResponseWriter, req *http.Request) {
	var data struct {
		Labels     []string `http:"l"`
		MaxResults int      `http:"max"`
		Exact      bool     `http:"x"`
	}
	data.MaxResults = 10 // set default
	if err := params.Unpack(req, &data); err != nil {
		http.Error(resp, err.Error(), http.StatusBadRequest) // 400
		return
	}

	// ...rest of handler...
	fmt.Fprintf(resp, "Search: %+v\n", data)
}

// params
// Unpack populates the fields of the struct pointed to by ptr
// from the HTTP request parameters in req.
func Unpack(req *http.Request, ptr interface{}) error {
	if err := req.ParseForm(); err != nil {
		return err
	}

	// Build map of fields keyed by effective name.
	fields := make(map[string]reflect.Value)
	v := reflect.ValueOf(ptr).Elem() // the struct variable
	for i := 0; i < v.NumField(); i++ {
		fieldInfo := v.Type().Field(i) // a reflect.StructField
		tag := fieldInfo.Tag           // a reflect.StructTag
		name := tag.Get("http")
		if name == "" {
			name = strings.ToLower(fieldInfo.Name)
		}
		fields[name] = v.Field(i)
	}

	// Update struct field for each parameter in the request.
	for name, values := range req.Form {
		f := fields[name]
		if !f.IsValid() {
			continue // ignore unrecognized HTTP parameters
		}
		for _, value := range values {
			if f.Kind() == reflect.Slice {
				elem := reflect.New(f.Type().Elem()).Elem()
				if err := populate(elem, value); err != nil {
					return fmt.Errorf("%s: %v", name, err)
				}
				f.Set(reflect.Append(f, elem))
			} else {
				if err := populate(f, value); err != nil {
					return fmt.Errorf("%s: %v", name, err)
				}
			}
		}
	}
	return nil
}
```

注意到，在 `http:"max"` 中，`max` 是一个字段名，将在 `tag := fieldInfo.Tag tag.Get("http")` 中被取出来。

用法如下：
```bash
$ ./search &
$ ./fetch 'http://localhost:12345/search'
Search: {Labels:[] MaxResults:10 Exact:false}
$ ./fetch 'http://localhost:12345/search?l=golang&l=programming'
Search: {Labels:[golang programming] MaxResults:10 Exact:false}
$ ./fetch 'http://localhost:12345/search?q=hello&x=123'
x: strconv.ParseBool: parsing "123": invalid syntax
```

### 12.8 Displaying the Methods of a Type

上面的例子展示了把 struct 的 Field 提取，在 [methods](../gopl.io/ch12/methods/methods.go) 中则展示了将 Method 提取。

### 12.9 A Word of Caution

不使用 reflection 的三点原因：
- 脆弱的程序
- 没有类型检查，可读性差
- 速度慢（小数据量的测试可能看不出来差别）
