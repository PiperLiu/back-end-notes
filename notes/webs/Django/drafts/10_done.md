# 实现匹配系统与项目收尾

- 作者：yxc
- 链接：https://www.acwing.com/file_system/file/content/whole/index/content/3435229/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [加密、压缩js代码（terser）](#加密-压缩js代码terser)
- [清理监听函数](#清理监听函数)
- [编写每局游戏的结束界面](#编写每局游戏的结束界面)
- [总结](#总结)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [加密、压缩js代码（terser）](#加密-压缩js代码terser)
- [清理监听函数](#清理监听函数)
- [编写每局游戏的结束界面](#编写每局游戏的结束界面)
- [总结](#总结)
  - [🔨 How to Deploy](#how-to-deploy)
  - [📕 我的笔记](#我的笔记)

<!-- /code_chunk_output -->

打卡：
- 我的项目地址：[https://app160.acapp.acwing.com.cn/](https://app160.acapp.acwing.com.cn/)
- 我的 AcApp 地址：[https://www.acwing.com/file_system/file/content/whole/index/content/3262287/](https://www.acwing.com/file_system/file/content/whole/index/content/3262287/)
- 我的项目地址：[https://git.acwing.com/PiperLiu/acapp](https://git.acwing.com/PiperLiu/acapp)
- 我的笔记：[https://github.com/PiperLiu/back-end-notes](https://github.com/PiperLiu/back-end-notes)

### 加密、压缩js代码（terser）

```bash
sudo apt-get update
sudo apt-get install npm
sudo npm install terser -g
```

`terser` 不仅支持文件输入，也支持标准输入。结果会输出到标准输出中。

使用方式：

```bash
terser xxx.js -c -m
```

我们将 `terser` 加到打包脚本里。

`scripts/compress_game_js.sh`

```bash
#! /bin/bash

JS_PATH=/home/acs/acapp/game/static/js/
JS_PATH_DIST=${JS_PATH}dist/
JS_PATH_SRC=${JS_PATH}src/

find $JS_PATH_SRC -type f -name '*.js' | sort | xargs cat | terser -c -m > ${JS_PATH_DIST}game.js

echo yes | python3 manage.py collectstatic
```

### 清理监听函数

在 AcApp 关闭时移除 `resize` 监听事件。

`game/static/js/src/playground/zbase.js`

```js
    start() {
        let outer = this;
        let uuid = this.create_uuid();
        $(window).on(`resize`, function() {
            outer.resize();
        });

        if (this.root.AcWingOS) {  // 在 AcWingOS 提供的 API 里
            this.root.AcWingOS.api.window.on_close(function() {
                $(window).off(`resize`);
            });
        }
    }
```

但是，如果我们同时打开两个窗口，关闭一个，将导致两个窗口的 `resize` 都被移除。

因此给每个窗口的 `resize` 加一个唯一标志。

```js
    create_uuid() {
        let res = "";
        for (let i = 0; i < 8; i ++ ) {
            let x = parseInt(Math.floor(Math.random() * 10));  // 返回[0, 1)之间的数
            res += x;
        }
        return res;
    }

    start() {
        let outer = this;
        $(window).resize(function() {
        let uuid = this.create_uuid();
        $(window).on(`resize.${uuid}`, function() {
            outer.resize();
        });

        if (this.root.AcWingOS) {
            this.root.AcWingOS.api.window.on_close(function() {
                $(window).off(`resize.${uuid}`);
            });
        }
    }
```

其他事件不需要移除，因为其他事件绑定到了 AcApp 的 `html` 元素上，元素被删除了，事件就没了。

注意：
- 同一个页面中，多个 AcApp 引入的 `js` 代码只会加载一次，因此 `AC_GAME_OBJECTS` 等全局变量是同一个页面、同一个 AcApp 的所有窗口共用的。
- 各自创建的局部变量是独立的，比如 `new AcGame()` 创建出的对象各个窗口是独立的。

### 编写每局游戏的结束界面

这里就是增加新对象 `game/static/js/src/playground/score_board/zbase.js` 。

此外，模型 unity ，在基类加入 `upadte_late` ，表示在每一帧结束时的渲染动作。

```js
let AC_GAME_OBJECTS = [];

class AcGameObject {
    ...

    late_update() {  // 在每一帧的最后执行一次
    }

    ...
}

let last_timestamp;
let AC_GAME_ANIMATION = function (timestamp) {

    ...

    for (let i = 0; i < AC_GAME_OBJECTS.length; i++) {
        let obj = AC_GAME_OBJECTS[i];
        obj.late_update();
    }

    last_timestamp = timestamp;

    requestAnimationFrame(AC_GAME_ANIMATION);
}

requestAnimationFrame(AC_GAME_ANIMATION);
```

一些其他逻辑不再记录。

### 总结

这个 Django 启动了 3 个服务，并且在前后端分离。

但这三个服务都强烈依托于 Django 服务。

最后负本项目 `README.md` ：

#### 🔨 How to Deploy

首先修改 `acapp/settings.py`  中的配置。

```python
ALLOWED_HOSTS = [
        '49.232.99.41',
        '你的地址'  # for test
        ]
```

```bash
git clone git@git.acwing.com:PiperLiu/acapp.git
cd acapp

# 打包 js
./scripts/compress_game_js.sh

# 开启 Django 服务
uwsgi --ini scripts/uwsgi.ini

# 开启 wss 服务
daphne -b 0.0.0.0 -p 5015 acapp.asgi:application

# 开启 thrift 服务
cd match_system/src/
./main.py
```

#### 📕 我的笔记

GitHub 备份在[这里](https://github.com/PiperLiu/back-end-notes/blob/master/notes/webs/Django/README.md)， Gitee 备份在[这里](https://gitee.com/piperliu/back-end-notes/blob/master/notes/webs/Django/README.md)。
