# A Tour of Go

学 MIT6.824 了！用 go 1.13 ，终于正式学 go 了！

之前确实有些接触，但是没有项目；这里咱们先从基础学起，边学边做 824 。有很多好视频在 [幼麟实验室](https://space.bilibili.com/567195437) ，这个咱们以后慢慢看。

https://go.dev/tour/list


<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Basics](#basics)
  - [Packages, variables, and functions](#packages-variables-and-functions)
  - [Flow control statements: for, if, else, switch and defer](#flow-control-statements-for-if-else-switch-and-defer)
  - [More types: structs, slices, and maps](#more-types-structs-slices-and-maps)
- [Methods and interfaces](#methods-and-interfaces)
- [Generics](#generics)
- [Concurrency](#concurrency)

<!-- /code_chunk_output -->

## Basics

### Packages, variables, and functions

[./drafts/gotour.1.1.md](./drafts/gotour.1.1.md)
- [package main](./drafts/gotour.1.1.md#package-main)
- [Imports](./drafts/gotour.1.1.md#imports)
- [函数 Functions](./drafts/gotour.1.1.md#函数-functions)
  - [share a type, multiple results](./drafts/gotour.1.1.md#share-a-type-multiple-results)
  - [Named return values](./drafts/gotour.1.1.md#named-return-values)
- [Variables](./drafts/gotour.1.1.md#variables)
  - [var](./drafts/gotour.1.1.md#var)
  - [Short variable declarations](./drafts/gotour.1.1.md#short-variable-declarations)
- [Basic types](./drafts/gotour.1.1.md#basic-types)
  - [%T %v](./drafts/gotour.1.1.md#t-v)
  - [Zero values](./drafts/gotour.1.1.md#zero-values)
  - [Type conversions](./drafts/gotour.1.1.md#type-conversions)
  - [Type inference](./drafts/gotour.1.1.md#type-inference)
  - [Constants, Numeric Constants](./drafts/gotour.1.1.md#constants-numeric-constants)

### Flow control statements: for, if, else, switch and defer

[./drafts/gotour.1.2.md](./drafts/gotour.1.2.md)

- [For (For is Go's "while")](./drafts/gotour.1.2.md#for-for-is-gos-while)
- [If](./drafts/gotour.1.2.md#if)
  - [with a short statement](./drafts/gotour.1.2.md#with-a-short-statement)
- [If and else](./drafts/gotour.1.2.md#if-and-else)
  - [Switch （无需 break ）](./drafts/gotour.1.2.md#switch-无需-break)
  - [Switch with no condition](./drafts/gotour.1.2.md#switch-with-no-condition)
- [Defer](./drafts/gotour.1.2.md#defer)
  - [Stacking defers](./drafts/gotour.1.2.md#stacking-defers)
  - [Defer, Panic, and Recover](./drafts/gotour.1.2.md#defer-panic-and-recover)
  - [Panic and Recover](./drafts/gotour.1.2.md#panic-and-recover)

### More types: structs, slices, and maps

[./drafts/gotour.1.3.md](./drafts/gotour.1.3.md)
- [Pointers](./drafts/gotour.1.3.md#pointers)
- [Structs](./drafts/gotour.1.3.md#structs)
- [Pointers to structs （取址不像 C 无需写箭头）](./drafts/gotour.1.3.md#pointers-to-structs-取址不像-c-无需写箭头)
- [Struct Literals](./drafts/gotour.1.3.md#struct-literals)
- [Arrays](./drafts/gotour.1.3.md#arrays)
- [Slices](./drafts/gotour.1.3.md#slices)
  - [Slices are like references to arrays](./drafts/gotour.1.3.md#slices-are-like-references-to-arrays)
  - [Slice literals](./drafts/gotour.1.3.md#slice-literals)
  - [Slice length and capacity](./drafts/gotour.1.3.md#slice-length-and-capacity)
  - [nil 判断是否为无数据（无数据不代表空切片）](./drafts/gotour.1.3.md#nil-判断是否为无数据无数据不代表空切片)
  - [Creating a slice with make](./drafts/gotour.1.3.md#creating-a-slice-with-make)
  - [Slices of slices](./drafts/gotour.1.3.md#slices-of-slices)
  - [Appending to a slice](./drafts/gotour.1.3.md#appending-to-a-slice)
  - [深入 Slices: usage and internals](./drafts/gotour.1.3.md#深入-slices-usage-and-internals)
- [Range](./drafts/gotour.1.3.md#range)
- [Maps](./drafts/gotour.1.3.md#maps)
  - [Map literals](./drafts/gotour.1.3.md#map-literals)
  - [Mutating Maps](./drafts/gotour.1.3.md#mutating-maps)
- [Function values](./drafts/gotour.1.3.md#function-values)
- [Function closures](./drafts/gotour.1.3.md#function-closures)

## Methods and interfaces

[./drafts/gotour.2.1.md](./drafts/gotour.2.1.md)
- [Methods](./drafts/gotour.2.1.md#methods)
  - [关于 type](./drafts/gotour.2.1.md#关于-type)
  - [Pointer receivers （拷贝一个还是传引用）](./drafts/gotour.2.1.md#pointer-receivers-拷贝一个还是传引用)
  - [Pointers and functions](./drafts/gotour.2.1.md#pointers-and-functions)
  - [Methods and pointer indirection](./drafts/gotour.2.1.md#methods-and-pointer-indirection)
- [Interfaces](./drafts/gotour.2.1.md#interfaces)
  - [Interfaces are implemented implicitly](./drafts/gotour.2.1.md#interfaces-are-implemented-implicitly)
  - [Interface values with nil underlying values](./drafts/gotour.2.1.md#interface-values-with-nil-underlying-values)
  - [The empty interface](./drafts/gotour.2.1.md#the-empty-interface)
- [Type assertions](./drafts/gotour.2.1.md#type-assertions)
  - [使用括号两种返回值](./drafts/gotour.2.1.md#使用括号两种返回值)
  - [Type switches](./drafts/gotour.2.1.md#type-switches)
- [Stringers](./drafts/gotour.2.1.md#stringers)
- [Errors](./drafts/gotour.2.1.md#errors)
- [Readers](./drafts/gotour.2.1.md#readers)

## Generics

[./drafts/gotour.3.1.md](./drafts/gotour.3.1.md)
- [Type parameters](./drafts/gotour.3.1.md#type-parameters)
- [Generic types](./drafts/gotour.3.1.md#generic-types)

## Concurrency

[./drafts/gotour.4.1.md](./drafts/gotour.4.1.md)
- [Goroutines](./drafts/gotour.4.1.md#goroutines)
- [Channelss](./drafts/gotour.4.1.md#channelss)
  - [Buffered Channels](./drafts/gotour.4.1.md#buffered-channels)
  - [Range and Close](./drafts/gotour.4.1.md#range-and-close)
  - [Select](./drafts/gotour.4.1.md#select)
  - [Default Selection](./drafts/gotour.4.1.md#default-selection)
- [sync.Mutex](./drafts/gotour.4.1.md#syncmutex)
