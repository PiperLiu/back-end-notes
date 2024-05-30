# select with priority

## background

从 `<-chan` 中做两件事时，那种方法效率高。具体的场景比如：
- 你控制着一个射击机器人，有两个函数可以调用
  - `load(bullet Bullet)` 函数负责装填子弹
  - `shoot(target Target)` 函数负责射击
- 我们希望当两个指令同时发送到机器人时，它优先执行 `shoot` 指令

我们这里从外界接收 bullet 和 target 指令，让我们来用 go 写一个控制器。

## equal.go

控制器从两个 channel 接收指令，没有优先级。

## type_assert.go 类型断言

控制器从一个 channel 接收指令，通过类型断言来判断指令类型，顺序执行。完全由 channel 的 writer 发送的顺序决定了执行顺序（优先级）。

这样的效率竟然比 equal.go 还要高。看了眼 pprof ，没太看懂。大概猜测一下：
- 类型断言可以在编译器做，相当于语法糖，所以并不会影响性能
- select channel 时它少了一个 case ，这样会不会少了一些对 lock 的访问？
  - 简单测了一下，如果给 equal.go 少一个 channel ，确实性能就提升了，而且和 type_assert.go 差不多

## priority.go

控制器从两个 channel 接收指令。两个 channel 优先级有区别：
- 如果 channel B 中有指令，那么一定会优先执行

为什么写得这么复杂？
- https://www.liwenzhou.com/posts/Go/priority-in-select/
- https://www.reddit.com/r/golang/comments/wsymef/selecting_higher_priority_events_over_lower/
- 简单化可能造成 CPU 空转

参考 k8s ：
- https://github.com/kubernetes/kubernetes/blob/7509c4eb478a3ab94ff26be2b4068da53212d538/pkg/controller/nodelifecycle/scheduler/taint_manager.go#L244

缺点：
- 没想到效率如此低
- 不知道怎么封装成函数
- 不知道怎么应对 3 个及以上的优先级

## benchmark

```bash
go version go1.22.3 darwin/arm64

(base) ➜  chan-select-priority git:(master) ✗ go run equal.go 
Bullet count: 38628692, Target count: 194223
Total count: 38822915
(base) ➜  chan-select-priority git:(master) ✗ go run type_assert.go 
Bullet count: 47769070, Target count: 239572
Total count: 48008642
(base) ➜  chan-select-priority git:(master) ✗ go run priority.go   
Bullet count: 81410, Target count: 149283
Total count: 230693
```
