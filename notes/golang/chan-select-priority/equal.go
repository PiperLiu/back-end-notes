package main

import (
	"fmt"
	"math/rand"
	"os"
	"runtime/pprof"
	"time"
)

// control parameters
var senderNum int = 10
var randomProbability float64 = 0.995
var bulletBufferSize int = 100
var targetBufferSize int = 1
var runDuration time.Duration = 10 * time.Second

// global variables
var bulletCount int = 0
var targetCount int = 0

type Bullet interface{}

type Target interface{}

type Robot interface {
	Load(bullet Bullet)
	Shoot(target Target)
}

type EasyBullet struct {
}

type EasyTarget struct {
}

type EasyRobot struct {
	bullets     chan Bullet
	targets     chan Target
	terminating chan struct{}
}

func (r *EasyRobot) Load(bullet Bullet) {
	bulletCount++
}

func (r *EasyRobot) Shoot(target Target) {
	targetCount++
}

func main() {
	robot := &EasyRobot{
		bullets:     make(chan Bullet, bulletBufferSize),
		targets:     make(chan Target, targetBufferSize),
		terminating: make(chan struct{}),
	}

	cpuFile, err := os.Create("cpu_equal.prof")
	if err != nil {
		fmt.Println(err)
	}
	defer cpuFile.Close()
	if err = pprof.StartCPUProfile(cpuFile); err != nil {
		fmt.Println(err)
	}
	defer pprof.StopCPUProfile()

	for i := 0; i < senderNum; i++ {
		go func() {
			for {
				if rand.Float64() < randomProbability {
					robot.bullets <- &EasyBullet{}
				} else {
					robot.targets <- &EasyTarget{}
				}
			}
		}()
	}

	go func() {
		for {
			select {
			case <-robot.terminating:
				return
			case bullet := <-robot.bullets:
				robot.Load(bullet)
			case target := <-robot.targets:
				robot.Shoot(target)
			default:
				// do nothing
			}
		}
	}()

	time.Sleep(runDuration)
	robot.terminating <- struct{}{}
	fmt.Printf("Bullet count: %d, Target count: %d\n", bulletCount, targetCount)
	fmt.Printf("Total count: %d\n", bulletCount+targetCount)
}
