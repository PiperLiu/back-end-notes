# 实现聊天系统

- 作者：yxc
- 链接：https://www.acwing.com/activity/content/72/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [消除一个bug（让聚焦的html元素获取事件，tabindex=0）](#消除一个bug让聚焦的html元素获取事件tabindex0)
- [聊天框开发注意事项](#聊天框开发注意事项)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [消除一个bug（让聚焦的html元素获取事件，tabindex=0）](#消除一个bug让聚焦的html元素获取事件tabindex0)
- [聊天框开发注意事项](#聊天框开发注意事项)

<!-- /code_chunk_output -->

打卡：
- 我的项目地址：[https://app160.acapp.acwing.com.cn/](https://app160.acapp.acwing.com.cn/)
- 我的 AcApp 地址：[https://www.acwing.com/file_system/file/content/whole/index/content/3262287/](https://www.acwing.com/file_system/file/content/whole/index/content/3262287/)
- 我的项目地址：[https://git.acwing.com/PiperLiu/acapp](https://git.acwing.com/PiperLiu/acapp)
- 我的笔记：[https://github.com/PiperLiu/back-end-notes](https://github.com/PiperLiu/back-end-notes)

### 消除一个bug（让聚焦的html元素获取事件，tabindex=0）

以前多开窗口时，按一下 `q` 在一个窗口触发攻击，此时再 focus 到另一个窗口，也会触发攻击，显然这是 bug 。

解决方案是将 `q` 监听绑定到 `canvas` 元素上，而非 `window` 上。

将 `game/static/js/src/playground/player/zbase.js` ：

```js
// 删掉 $(window).keydown(function(e) {
// 加入
this.playground.game_map.$canvas.keydown(function(e) {...}
```

然而， `canvas` 并不能天生接受事件监听，还需要对其属性进行修改：

```js
// this.$canvas = $(`<canvas></canvas>`);
this.$canvas = $(`<canvas tabindex=0></canvas>`);

// 并且在 canvas 初始化时
this.$canvas.focus();
```

[MDN](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Global_attributes/tabindex)：
- `tabindex=负值` (通常是`tabindex=“-1”`)，表示元素是可聚焦的，但是不能通过键盘导航来访问到该元素，用JS做页面小组件内部键盘导航的时候非常有用。
- `tabindex="0"` ，表示元素是可聚焦的，并且可以通过键盘导航来聚焦到该元素，它的相对顺序是当前处于的DOM结构来决定的。
- `tabindex=正值`，表示元素是可聚焦的，并且可以通过键盘导航来访问到该元素；它的相对顺序按照`tabindex` 的数值递增而滞后获焦。如果多个元素拥有相同的 `tabindex`，它们的相对顺序按照他们在当前DOM中的先后顺序决定。

### 聊天框开发注意事项

聊天框的 `scroll` 逻辑：

```js
render_message(message) {
    return $(`<div>${message}</div>`);
}

add_message(username, text) {
    this.show_history();
    let message = `[${username}]${text}`;
    this.$history.append(this.render_message(message));
    this.$history.scrollTop(this.$history[0].scrollHeight);
}
```

