# 1. Tutorial

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [1.1 Hello, World](#11-hello-world)
- [1.2 Command-Line Arguments](#12-command-line-arguments)
- [1.3 Finding Duplicate Lines](#13-finding-duplicate-lines)
- [1.4 Animated GIFs](#14-animated-gifs)
- [1.5 Fetching a URL](#15-fetching-a-url)
- [1.6 Fetching URLs Concurrently](#16-fetching-urls-concurrently)
- [1.7 A Web Server](#17-a-web-server)
- [1.8 Loose Ends](#18-loose-ends)

<!-- /code_chunk_output -->

### 1.1 Hello, World

### 1.2 Command-Line Arguments

### 1.3 Finding Duplicate Lines

### 1.4 Animated GIFs

主要是对 `image` 和 `math` 的运用，用 `for` 生成一张张图片 `append` 到 `gif.GIF struct` 的结构体中。

代码在 [../gopl.io/ch1/lissajous/main.go](../gopl.io/ch1/lissajous/main.go) 。这里的函数传入了一个 `io.Writer` ，而 `main` 中则是把 `os.Stdout` 传入。因此使用时如下。

```bash
go build gopl.io/ch1/lissajous
./lissajous > out.gif
```

### 1.5 Fetching a URL

### 1.6 Fetching URLs Concurrently

### 1.7 A Web Server

与 Gif 相结合，很有趣的一点是 `w http.ResponseWriter` 也是“继承”自 `io.Writer` ，因此我们也可以直接 `lissajous(w)` ，从而方便地把 Gif 作为 `http response` 的一部分。

### 1.8 Loose Ends
