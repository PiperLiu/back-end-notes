# A Tour of Go

学 MIT6.824 了！用 go 1.13 ，终于正式学 go 了！

之前确实有些接触，但是没有项目；这里咱们先从基础学起，边学边做 824 。有很多好视频在 [幼麟实验室](https://space.bilibili.com/567195437) ，这个咱们以后慢慢看。

https://go.dev/tour/list


<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Basics](#basics)
  - [Packages, variables, and functions](#packages-variables-and-functions)
  - [Flow control statements: for, if, else, switch and defer](#flow-control-statements-for-if-else-switch-and-defer)

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

