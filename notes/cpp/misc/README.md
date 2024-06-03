
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [C++ 万年历](#c-万年历)

<!-- /code_chunk_output -->


### C++ 万年历

如何用c++打出万年历？ - 孙孟越的回答 - 知乎
https://www.zhihu.com/question/435416023/answer/2027973395

代码见 [./code/stdout_calendar.cpp](./code/stdout_calendar.cpp)

```shell
g++ -std=gnu++11 notes/cpp/misc/code/stdout_calendar.cpp && ./a.out 
       January              February                March        
 Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa
                 1  2      1  2  3  4  5  6      1  2  3  4  5  6
  3  4  5  6  7  8  9   7  8  9 10 11 12 13   7  8  9 10 11 12 13
 10 11 12 13 14 15 16  14 15 16 17 18 19 20  14 15 16 17 18 19 20
 17 18 19 20 21 22 23  21 22 23 24 25 26 27  21 22 23 24 25 26 27
 24 25 26 27 28 29 30  28                    28 29 30 31         
 31                                                              
        April                  May                  June         
 Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa
              1  2  3                     1         1  2  3  4  5
  4  5  6  7  8  9 10   2  3  4  5  6  7  8   6  7  8  9 10 11 12
 11 12 13 14 15 16 17   9 10 11 12 13 14 15  13 14 15 16 17 18 19
 18 19 20 21 22 23 24  16 17 18 19 20 21 22  20 21 22 23 24 25 26
 25 26 27 28 29 30     23 24 25 26 27 28 29  27 28 29 30         
                       30 31                                     
        July                 August               September      
 Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa
              1  2  3   1  2  3  4  5  6  7            1  2  3  4
  4  5  6  7  8  9 10   8  9 10 11 12 13 14   5  6  7  8  9 10 11
 11 12 13 14 15 16 17  15 16 17 18 19 20 21  12 13 14 15 16 17 18
 18 19 20 21 22 23 24  22 23 24 25 26 27 28  19 20 21 22 23 24 25
 25 26 27 28 29 30 31  29 30 31              26 27 28 29 30      
                                                                 
       October              November              December       
 Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa
                 1  2      1  2  3  4  5  6            1  2  3  4
  3  4  5  6  7  8  9   7  8  9 10 11 12 13   5  6  7  8  9 10 11
 10 11 12 13 14 15 16  14 15 16 17 18 19 20  12 13 14 15 16 17 18
 17 18 19 20 21 22 23  21 22 23 24 25 26 27  19 20 21 22 23 24 25
 24 25 26 27 28 29 30  28 29 30              26 27 28 29 30 31   
 31                                                             
```

最妙的地方在于：
- 月份数据/常量数据很少，写死就行
- C++ 二维数组实际上是连续的内存空间，这里之所以做成“二维”，只是为了方便 for loop 自动换行；在内存拷贝时不考虑所谓的换行逻辑更加轻松
