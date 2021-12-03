# 实现联机对战（下）

- 作者：yxc
- 链接：https://www.acwing.com/file_system/file/content/whole/index/content/3357332/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [联机对战实现](#联机对战实现)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [联机对战实现](#联机对战实现)

<!-- /code_chunk_output -->

打卡：
- 我的项目地址：[https://app160.acapp.acwing.com.cn/](https://app160.acapp.acwing.com.cn/)
- 我的 AcApp 地址：[https://www.acwing.com/file_system/file/content/whole/index/content/3262287/](https://www.acwing.com/file_system/file/content/whole/index/content/3262287/)
- 我的项目地址：[https://git.acwing.com/PiperLiu/acapp](https://git.acwing.com/PiperLiu/acapp)
- 我的笔记：[https://github.com/PiperLiu/back-end-notes](https://github.com/PiperLiu/back-end-notes)

### 联机对战实现

- 前端进行同步的发送和接收，后端主要起传输和广播作用
- 为了将子弹同步，需要将子弹存下来
  - 我的子弹同步给别人了，别人的子弹需要创建一个子弹实例，并且 `fireball.uuid` = 我传过去的子弹的 `uuid`
  - 在 `destroy` 前我们调用 `fireball` 的 `on_destroy` ，把其从玩家的 `fireball` 队列里取出来销毁
- 子弹碰撞的决策权完全归属于发出者本地的计算，在其他窗口为渲染

用状态机来决定玩家当前是否可移动。

这节课新内容不多，主要是上一节课的机械实践。

注意 AcWingOS 新增 `api` 关闭窗口：
```js
logout_on_remote() {  // 在远程服务器上登出
        if (this.platform === "ACAPP") {
            this.root.AcWingOS.api.window.close();
        } else {
            $.ajax({
                url: "https://app165.acapp.acwing.com.cn/settings/logout/",
                type: "GET",
                success: function (resp) {
                    if (resp.result === "success") {
                        location.reload();
                    }
                }
            });
        }
    }
```
