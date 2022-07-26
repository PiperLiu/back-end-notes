# 10. Packages and the Go Tool

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [10.1 Introduction](#101-introduction)
- [10.2 Import Paths](#102-import-paths)
- [10.3 The Package Declaration](#103-the-package-declaration)
- [10.4 Import Declarations](#104-import-declarations)
- [10.5 Blank Imports](#105-blank-imports)
- [10.6 Packages and Naming](#106-packages-and-naming)
- [10.7 The Go Tool](#107-the-go-tool)
  - [10.7.1 Workspace Organization](#1071-workspace-organization)
  - [10.7.2 Downloading Packages](#1072-downloading-packages)
  - [10.7.3 Building Packages](#1073-building-packages)
  - [10.7.4 Documenting Packages](#1074-documenting-packages)
  - [10.7.5 Internal Packages](#1075-internal-packages)
  - [10.7.6 Querying packages](#1076-querying-packages)

<!-- /code_chunk_output -->

### 10.1 Introduction

### 10.2 Import Paths

### 10.3 The Package Declaration

有三个特例：
- main
- `_test.go` 后缀
- 版本号比如 `gopkg.in/yaml.v2`

### 10.4 Import Declarations

### 10.5 Blank Imports

```go
import _ "image/png"  // register PNG decoder
```

例子见 [jpeg](../gopl.io/ch10/jpeg/) 。如果没有 black import ，那么 `img, kind, err := image.Decoder(in)` 中就会出错，因为不认识 png 这个格式。

如下是 png 源码。

```go
package png // image/png

func Decode(r io.Reader) (image.Image, error)
func DecodeConfig(r io.Reader) (image.Config, error)

func init() {
    const pngHeader = "\x89PNG\r\n\x1a\n"
    image.RegisterFormat("png", pngHeader, Decode, DecodeConfig)
}
```

### 10.6 Packages and Naming

### 10.7 The Go Tool

#### 10.7.1 Workspace Organization

很遗憾的是本书支持的是 Go 1.5 的版本，而 `go mod` 直到 1.11 才被引入，因此本书描述的结构未能脱离开 GOPATH ，必须在 GOPATH 之下。

```sh
$ export GOPATH=$HOME/gobook
$ go get gopl.io/...
```

如下是书中的结构：

```
GOPATH/
    src/
        gopl.io/
            .git/
            ch1/
                helloworld/
                    main.go
                dup/
                    main.go
                ...
        golang.org/x/net/
            .git/
            html/
                parse.go
                node.go
                ...
    bin/
        helloworld
        dup
    pkg/
        darwin_amd64/
            ...
```

#### 10.7.2 Downloading Packages

#### 10.7.3 Building Packages

特殊的命名如 net_linux.go 或开头的注释 `// +build linux darwin` 都会影响编译。

#### 10.7.4 Documenting Packages

#### 10.7.5 Internal Packages

`go doc`

#### 10.7.6 Querying packages

`go list`
