# 实现联机对战（上）

- 作者：yxc
- 链接：https://www.acwing.com/file_system/file/content/whole/index/content/3357332/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [总流程](#总流程)
- [联机对战思路（websocket协议）](#联机对战思路websocket协议)
- [配置 channels_redis 等](#配置-channels_redis-等)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [总流程](#总流程)
- [联机对战思路（websocket协议）](#联机对战思路websocket协议)
- [配置 channels_redis 等](#配置-channels_redis-等)
  - [安装 channels_redis](#安装-channels_redis)
  - [配置 acapp/asgi.py](#配置-acappasgipy)
  - [配置acapp/settings.py](#配置acappsettingspy)
  - [配置game/routing.py](#配置gameroutingpy)
  - [编写game/consumers](#编写gameconsumers)
  - [前端新增 new WebSocket()](#前端新增-new-websocket)
  - [启动django_channels](#启动django_channels)

<!-- /code_chunk_output -->

### 总流程

首先修改前端：统一长度单位，防止玩家改变窗口大小出现不适配问题。
- 增加 `resize` 方法，并且在 `game/static/js/src/playground/zbase.js` 的 `start() {}` 中增加 `$(window).resize(function() { outer.resize(); });` 监听
- 将长度单位改为相对大小
- 修改 css 让画面保持居中

之后进行：
- 增加“联机对战”模式
- 配置`django_channels`
- 编写同步函数

### 联机对战思路（websocket协议）

首先为了减轻服务器负载、提升玩家体验，我们在本地判定玩家是否被击中，然后再将结果传给服务器，服务器再将结果更新给其他玩家。

由于服务器需要主动将数据更新给其他玩家，因此 http 协议（单线）不再适用，使用 websocket 协议（双向）。

http 的加密版是 https ， ws 的加密版是 wss 。

我们使用 Django 对 wss 的支持工具： `Django Channels` 。

### 配置 channels_redis 等

#### 安装 channels_redis

```bash
pip install channels_redis
```

#### 配置 acapp/asgi.py

内容如下：
```python
import os

from channels.auth import AuthMiddlewareStack
from channels.routing import ProtocolTypeRouter, URLRouter
from django.core.asgi import get_asgi_application
from game.routing import websocket_urlpatterns

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'acapp.settings')

application = ProtocolTypeRouter({
    "http": get_asgi_application(),
    "websocket": AuthMiddlewareStack(URLRouter(websocket_urlpatterns))
})
```

#### 配置acapp/settings.py

在`INSTALLED_APPS`中添加`channels`，添加后如下所示：

```python
INSTALLED_APPS = [ 
    'channels',
    'game.apps.GameConfig',
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
]
```

然后在文件末尾添加：
```python
ASGI_APPLICATION = 'acapp.asgi.application'
CHANNEL_LAYERS = {
    "default": {
        "BACKEND": "channels_redis.core.RedisChannelLayer",
        "CONFIG": {
            "hosts": [("127.0.0.1", 6379)],
        },
    },
}
```

#### 配置game/routing.py

这一部分的作用相当于`http`的`urls`。

内容如下：
```python
from django.urls import path
from game.consumers.multiplayer.index import MultiPlayer

websocket_urlpatterns = [
    path("wss/multiplayer/", MultiPlayer.as_asgi(), name="wss_multiplayer"),
]
```

#### 编写game/consumers

这一部分的作用相当于`http`的`views`。

参考示例：
```python
from channels.generic.websocket import AsyncWebsocketConsumer
import json

class MultiPlayer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.accept()
        print('accept')

        self.room_name = "room"
        await self.channel_layer.group_add(self.room_name, self.channel_name)

    async def disconnect(self, close_code):
        print('disconnect')
        await self.channel_layer.group_discard(self.room_name, self.channel_name);


    async def receive(self, text_data):
        data = json.loads(text_data)
        print(data)
```

在我们的例子中：

`game/consumers/multiplayer/index.py`：
```python
from channels.generic.websocket import AsyncWebsocketConsumer
import json
from django.conf import settings
from django.core.cache import cache

class MultiPlayer(AsyncWebsocketConsumer):
    async def connect(self):
        self.room_name = None

        for i in range(1000):
            name = "room-%d" % (i)
            if not cache.has_key(name) or len(cache.get(name)) < settings.ROOM_CAPACITY:
                self.room_name = name
                break

        if not self.room_name:
            return

        await self.accept()

        if not cache.has_key(self.room_name):
            cache.set(self.room_name, [], 3600)  # 有效期1小时

        for player in cache.get(self.room_name):
            await self.send(text_data=json.dumps({
                'event': "create_player",
                'uuid': player['uuid'],
                'username': player['username'],
                'photo': player['photo'],
            }))

        await self.channel_layer.group_add(self.room_name, self.channel_name)

    async def disconnect(self, close_code):
        print('disconnect')
        await self.channel_layer.group_discard(self.room_name, self.channel_name);


    async def create_player(self, data):
        players = cache.get(self.room_name)
        players.append({
            'uuid': data['uuid'],
            'username': data['username'],
            'photo': data['photo']
        })
        cache.set(self.room_name, players, 3600)  # 有效期1小时
        # 这里用 group_send 在组内广播
        # type 这个参数是调用广播接收方的哪个函数
        await self.channel_layer.group_send(
            self.room_name,
            {
                'type': "group_create_player",
                'event': "create_player",
                'uuid': data['uuid'],
                'username': data['username'],
                'photo': data['photo'],
            }
        )

    # 在其他用户的广播中被调用
    # 给前端发送 json 创建玩家
    async def group_create_player(self, data):
        await self.send(text_data=json.dumps(data))

    async def receive(self, text_data):
        data = json.loads(text_data)
        event = data['event']
        if event == "create_player":
            await self.create_player(data)
```

#### 前端新增 new WebSocket()

```js
class MultiPlayerSocket {
    constructor(playground) {
        this.playground = playground;

        this.ws = new WebSocket("wss://app165.acapp.acwing.com.cn/wss/multiplayer/");

        this.start();
    }

    start() {
        this.receive();
    }

    receive () {
        let outer = this;

        // ws 用 onmessage 接受组内信息
        this.ws.onmessage = function(e) {
            let data = JSON.parse(e.data);
            let uuid = data.uuid;
            if (uuid === outer.uuid) return false;

            let event = data.event;
            if (event === "create_player") {
                outer.receive_create_player(uuid, data.username, data.photo);
            }
        };
    }

    send_create_player(username, photo) {
        let outer = this;
        this.ws.send(JSON.stringify({
            'event': "create_player",
            'uuid': outer.uuid,
            'username': username,
            'photo': photo,
        }));
    }

    receive_create_player(uuid, username, photo) {
        let player = new Player(
            this.playground,
            this.playground.width / 2 / this.playground.scale,
            0.5,
            0.05,
            "white",
            0.15,
            "enemy",
            username,
            photo,
        );

        player.uuid = uuid;
        this.playground.players.push(player);
    }
}
```

为了同步所有内容，因此所有的东西都需要一个唯一编号，就是 `uuid` 。

然后再修改一些前端：
- `game/static/js/src/menu/zbase.js` 开始界面切换
- `game/static/js/src/playground/ac_game_object/zbase.js` 增加唯一编号 `uuid`
- `game/static/js/src/playground/player/zbase.js` 判断是不是本机玩家
- `game/static/js/src/playground/zbase.js` 区分单人模式和多人模式，多人模式与 `MultiPlayerSocket` 耦合

总结：
- 前端的 `WebSocket` 实例使用 `onmessage` 接收传过来的信息，后端使用 `send` 发给前端
- 用户对应的后端使用 `channel_layer.group_send` 向组内玩家广播，调用各个玩家后端的 `'type'` 方法

#### 启动django_channels

在`~/acapp`目录下执行：
```bash
daphne -b 0.0.0.0 -p 5015 acapp.asgi:application
```

`ws` 和 `wsgi` （http）并存。
