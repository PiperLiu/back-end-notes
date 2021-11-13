# 配置docker、git环境与项目创建

- 作者：yxc
- 链接：https://www.acwing.com/activity/content/72/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [配置 docker 环境](#配置-docker-环境)
- [知识点拾遗](#知识点拾遗)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [配置 docker 环境](#配置-docker-环境)
  - [准备工作–配置环境](#准备工作配置环境)
  - [开始施工](#开始施工)
  - [登录`django`管理员界面](#登录django管理员界面)
- [知识点拾遗](#知识点拾遗)
  - [Docker临时端口映射方法](#docker临时端口映射方法)
  - [Linux中ag全文搜索工具](#linux中ag全文搜索工具)
  - [Django中一些概念](#django中一些概念)

<!-- /code_chunk_output -->

### 配置 docker 环境

参考修改自[栎](https://www.acwing.com/solution/content/72577/)：

#### 准备工作–配置环境

`scp /var/lib/acwing/docker/images/ django_lesson_1_0.tar server_name:` 将镜像上传到自己租的云端服务器

`ssh server_name` 登录自己的云端服务器

`docker load -i django_lesson_1_0.tar` 将镜像加载到本地

`docker run -p 20000:22 -p 8000:8000 --name django_server -itd django_lesson:1.0` 创建并运行`django_lesson:1.0`镜像** (端口要自己去云平台放行)**

`docker attach django_server` 进入创建的`docker`容器

`passwd` 设置`root`密码

`adduser acs` 创建普通用户`acs`

`usermod -aG sudo acs` 给用户`acs`分配`sudo`权限

`sudo su acs` 可切换到用户`acs`中

`ctrl p + ctrl q` 挂起容器

返回`AC terminal`，为`acs`用户配置别名和免密登录

`vim ~/.ssh/config`

`ssh-copy-id 别名` 为`acs`用户一键添加公钥，免密登录
`scp .bashrc .vimrc .tmux.conf 别名:` 配置一下环境

#### 开始施工

打开`tumx`

`django-admin startproject acapp` 创建`django`项目`acapp`

配置`git`：
- `ssh-keygen` 生成密钥用于连接到`ac git`上面
- 在`git`偏好设置中，打开`ssh`密钥，添加一下刚才生成的公钥
- `git init` 进到`acapp`中将其配置成`git`仓库
打开`git`，在`git`上创建一个仓库（项目）按照下面的提示在`acs`里面配置一下`git`

```bash
git config --global user.name xxx
git config --global user.email xxx@xxx.com
git add .
git commit -m "xxx"
git remote add origin git@git.acwing.com:xxx/XXX.git #建立连接
git push --set-upstream origin master
```

再打开一个`tmux`，(一个`tmux`用于维护控制台，另一个`tmux`用于开发)跑一下我们的项目 `python3 manage.py runserver 0.0.0.0:8000`

`ag ALLOWED_HOSTS` 全文搜索
- 找到这个字段所在位置，并将自己的IP地址添加到里面
- 便可用 `IP地址:8000` 打开`django`页面

`git` 一下代码
- 注意：在`git`仓库的根目录下（`acapp`）,`vim .gitignore`
- 在文件里面写上 `*/__pycache__`(两个杠)
- 再去添加文件的时候就不会再添加这类不必要的文件

`python3 manage.py startapp game` 创建`game` `app`

#### 登录`django`管理员界面

`ctrl c` 先关掉控制台

`python3 manage.py migrate` 同步一下数据库的修改

`python3 manage.py createsuperuser` 创建管理员账号

`pyhton3 manage.py runserver 0.0.0.0:8000` 启动控制台

`IP地址:8000/admin` 进到管理员登录界面，输入一下刚才创建的账号即可进到管理员界面

```bash
cd game
touch urls.py
mkdir templates
vim views # 定义一个index函数 视频位置 1:00:00
vim urls.py # 写一下路由
cd acapp vim urls.py # 将刚才的路由写到总路由里面
models：存储各种数据结构（class等）
views：存储函数
urls：路由
templates：存储网页模板
```

参考[屿祚](https://www.acwing.com/solution/content/72330/)的脚本：

vim game/views.py
```python
from django.http import HttpResponse

def index(resquest):
    line1 = '<h1 style="text-align: center">你是故意找茬是吧</h1>'
    line3 = '<a href="/play/">进入劈瓜</a>'
    line4 = '<hr>'
    line2 = '<img src = "xxx">'
    return HttpResponse(line1+line3+line4+line2)

def play(resquest):
    line1 = '<h1 style="text-align: center">给你机会你不中用啊</h1>'
    line3 = '<a href="/">返回找茬</a>'
    line4= '<hr>'
    line2 = '<img src = "xxx">'
    return HttpResponse(line1+line3+line4+line2)
```

vim game/urls.py
```python
from django.urls import path
from game.views import index, play

urlpatterns = [ 
     path("", index, name="index"),
     path("play/", play, name="play")
     ]
```

vim acapp/acapp/urls.py
```python 
from django.urls import path, include # 修改第十一行
path('', include('game.urls')), # 在第20行之后插入
```

`git`一下代码，完工。

### 知识点拾遗

#### Docker临时端口映射方法

参考[Accepted.](https://www.acwing.com/solution/content/72223/)：

在群里看到有些小伙伴run容器的时候没有映射端口号，所以给出这个方法来临时映射端口号。

方法：
- 利用`docker ps`命令查看`<container_id>`（容器id）
- 查看容器IP:
  - `docker inspect <container_id> | grep IPAddress`
- 宿主机配置`iptables`:
  - `iptables -t nat -A DOCKER -p tcp --dport <host_port> -j DNAT --to-destination <container_ip>:<docker_port>`
  - `iptables -t nat -A POSTROUTING -j MASQUERADE -p tcp --source <container_ip> --destination <container_ip> --dport <docker_port>`
  - `iptables -A DOCKER -j ACCEPT -p tcp --destination <container_ip> --dport <docker_port>`
- 查看是否映射成功:
  - `iptables -t nat -nvL`

例如：将容器内`22`端口映射到宿主机`20000`端口。

```bash
172.17.0.2   --->    container_ip
20000        --->    host_port
22           --->    docker_port

iptables -t nat -A DOCKER -p tcp --dport 20000 -j DNAT --to-destination 172.17.0.2:22
iptables -t nat -A POSTROUTING -j MASQUERADE -p tcp --source 172.17.0.2 --destination 172.17.0.2 --dport 22
iptables -A DOCKER -j ACCEPT -p tcp --destination 172.17.0.2 --dport 22

iptables -t nat -nvL #（查看是否映射成功）
```

注：如果报错在命令前面加上`sudo`提升一下权限

#### Linux中ag全文搜索工具

[还在用 ack ？试试更快的 rg/ag 吧](https://zhuanlan.zhihu.com/p/129262877)：

The Silver Searcher
```
A code-searching tool similar to ack, but faster.
```

这次介绍一个命令行文本搜索工具 `The Silver Searcher`，命令行简写为 `ag`。 按照官方的说法 `ag` 要比 `ack` 快几十倍，并且使用更加友好。笔者日常电脑上也基本上是使用 `ag` 替代了 `grep/ack` 搜索文本，体验还是挺不错的，安装完之后还可以和 `vim` 进行集成，比如笔者之前介绍过 `fzf.vim` 插件， 可以直接在 `vim` 里使用 `:Ag` 命令来进行文本搜索。

评论区有人提到 `rg` 似乎更快，感兴趣的也可以试试 `ripgrep`。同样可以在 `fzf.vim` 中使用 `:Rg` 调用 `rg` 来搜索。

优势：
- 搜索大项目比 `ack` 更快
- 自动忽略 `.gitignore/.ignore` 文件
- 友好的命令行参数

#### Django中一些概念

Django 中以 `app` 为单位管理项目。

官方文档：https://www.djangoproject.com/

Django 暴露给开发者的组件是很明晰的。
