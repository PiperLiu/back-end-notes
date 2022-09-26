# go 中 select 优先级

[Go语言在select语句中实现优先级](https://www.cnblogs.com/xiao-xue-di/p/14439516.html)

```go
func worker2(ch1, ch2 <-chan int, stopCh chan struct{}) {
	for {
		select {
		case <-stopCh:
			return
		case job1 := <-ch1:
			fmt.Println(job1)
		case job2 := <-ch2:
		priority:
			for {
				select {
				case job1 := <-ch1:  // 优先执行 job1
					fmt.Println(job1)
				default:
					break priority
				}
			}
			fmt.Println(job2)  // 没有 job1 了才执行 job2
		}
	}
}
```

k8s 中应用：

```go
// kubernetes/pkg/controller/nodelifecycle/scheduler/taint_manager.go 
func (tc *NoExecuteTaintManager) worker(worker int, done func(), stopCh <-chan struct{}) {
	defer done()

	// 当处理具体事件的时候，我们会希望 Node 的更新操作优先于 Pod 的更新
	// 因为 NodeUpdates 与 NoExecuteTaintManager无关应该尽快处理
	// -- 我们不希望用户(或系统)等到PodUpdate队列被耗尽后，才开始从受污染的Node中清除pod。
	for {
		select {
		case <-stopCh:
			return
		case nodeUpdate := <-tc.nodeUpdateChannels[worker]:
			tc.handleNodeUpdate(nodeUpdate)
			tc.nodeUpdateQueue.Done(nodeUpdate)
		case podUpdate := <-tc.podUpdateChannels[worker]:
			// 如果我们发现了一个 Pod 需要更新，我么你需要先清空 Node 队列.
		priority:
			for {
				select {
				case nodeUpdate := <-tc.nodeUpdateChannels[worker]:
					tc.handleNodeUpdate(nodeUpdate)
					tc.nodeUpdateQueue.Done(nodeUpdate)
				default:
					break priority
				}
			}
			// 在 Node 队列清空后我们再处理 podUpdate.
			tc.handlePodUpdate(podUpdate)
			tc.podUpdateQueue.Done(podUpdate)
		}
	}
}
```

我自己写的本地测试：

```go
package main

import (
	"flag"
	"fmt"
	"math/rand"
	"time"
)

func main() {
	n := flag.Int("b", 8, "buckets")

	flag.Parse()

	dataCh := make(chan int)
	chList := make([]chan struct{}, 0)
	for i := 0; i < *n; i++ {
		bucketFunc, ch := makeBucketEffect(i, dataCh)
		go bucketFunc()
		chList = append(chList, ch)
	}
	go core(chList)

	for i := 0; i < 100; i++ {
		time.Sleep(time.Millisecond * 8)
		dataCh <- i
	}
}

func makeBucketEffect(name int, dataCh chan int) (func(), chan struct{}) {
	ch := make(chan struct{}, 2)  // set 2 for not block in core
	bucketFunc := func() {
		for {
			// https://www.cnblogs.com/xiao-xue-di/p/14439516.html
			select {
			case <-ch: // higher priority
				fmt.Println("->", name, "core task", time.Now().Format("2006 0102 15:04:05.000"))
			case data := <-dataCh:
			priority:
				for {
					select {
					case <-ch:
						fmt.Println("->", name, "core task", time.Now().Format("2006 0102 15:04:05.000"))
					default:
						break priority
					}
				}
				// deal data
				r := rand.Intn(100) + 100
				fmt.Println(name, "start dealing with", data)
				time.Sleep(time.Duration(r * int(time.Millisecond)))
				fmt.Println(name, "done dealing with", data)
			}
		}
	}
	return bucketFunc, ch
}

func core(chList []chan struct{}) {
	for {
		time.Sleep(time.Millisecond * 500)
		for i, ch := range chList {
			fmt.Println("<- core send", i, time.Now().Format("2006 0102 15:04:05.000"))
			ch <- struct{}{}
		}
	}
}
```
