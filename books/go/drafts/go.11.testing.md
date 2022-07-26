# 11. Testing

除了最常见的 `TestXXX` 还有 `BenchmarkXXX` 、 `ExampleXXX` 。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [11.1 The go test Tool](#111-the-go-test-tool)
- [11.2 Test Functions](#112-test-functions)
  - [11.2.1 Randomized Testing](#1121-randomized-testing)
  - [11.2.2 Testing a Command](#1122-testing-a-command)
  - [11.2.3 White-Box Testing](#1123-white-box-testing)
  - [11.2.4 External Test Packages](#1124-external-test-packages)
  - [11.2.5 Writing Effective Tests](#1125-writing-effective-tests)
  - [11.2.6 Avoiding Brittle Tests](#1126-avoiding-brittle-tests)
- [11.3 Coverage](#113-coverage)
- [11.4 Benchmark Functions](#114-benchmark-functions)
- [11.5 Profiling](#115-profiling)
- [11.6 Example Functions](#116-example-functions)

<!-- /code_chunk_output -->

### 11.1 The go test Tool

### 11.2 Test Functions

#### 11.2.1 Randomized Testing

#### 11.2.2 Testing a Command

[echo](../gopl.io/ch11/echo/) 中用 Stdout 来 mock 命令行命令。

#### 11.2.3 White-Box Testing

参考 [storage](../gopl.io/ch11/storage2/) ，在 mock 了一个函数后应该注意 `defer` 将其恢复。

#### 11.2.4 External Test Packages

有时候我们需要 `xxx_test` 作为 package 避免循环调用。

#### 11.2.5 Writing Effective Tests

#### 11.2.6 Avoiding Brittle Tests

### 11.3 Coverage

```go
go tool cover -html=coverage.out
```

### 11.4 Benchmark Functions

```go
import "testing"

func BenchmarkIsPalindrome(b *testing.B) {
  for i := 0; i < b.N; i++ {
    IsPalindrome("OK")
  }
}
```

```bash
go test -bench=. -benchmem
```

### 11.5 Profiling

```bash
go test -cpuprofile=cpu.out
go test -memprofile=mem.out
go test -blockprofile=block.out

go test -run=NONE -bench=ClientServer -cpuprofile=cpu.log net/http
go tool pprof -text -nodecount=10 ./http.test cpu.log
```

### 11.6 Example Functions

```go
func ExampleIsPalindrome() {
  fmt.Println(IsPalindrome("OK"))
  fmt.Println(IsPalindrome("Able was I ere I saw Elba"))
  // Output:
  // false
  // true
}
```

三个用处：
- 文档
- 测试（如果有 `// Output:` 注释会参与测试）
- golang.org 的 godoc 服务器会在 Go playground 生成相应代码
