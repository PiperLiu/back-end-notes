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

��Ҫ�Ƕ� `image` �� `math` �����ã��� `for` ����һ����ͼƬ `append` �� `gif.GIF struct` �Ľṹ���С�

������ [../gopl.io/ch1/lissajous/main.go](../gopl.io/ch1/lissajous/main.go) ������ĺ���������һ�� `io.Writer` ���� `main` �����ǰ� `os.Stdout` ���롣���ʹ��ʱ���¡�

```bash
go build gopl.io/ch1/lissajous
./lissajous > out.gif
```

### 1.5 Fetching a URL

### 1.6 Fetching URLs Concurrently

### 1.7 A Web Server

�� Gif ���ϣ�����Ȥ��һ���� `w http.ResponseWriter` Ҳ�ǡ��̳С��� `io.Writer` ���������Ҳ����ֱ�� `lissajous(w)` ���Ӷ�����ذ� Gif ��Ϊ `http response` ��һ���֡�

### 1.8 Loose Ends
