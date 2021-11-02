# 配置docker、git环境与项目创建

- 作者：yxc
- 链接：https://www.acwing.com/activity/content/72/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [](#)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [](#)

<!-- /code_chunk_output -->

### 

参考修改自[栎](https://www.acwing.com/solution/content/72577/)：

准备工作–配置环境
scp /var/lib/acwing/docker/images/ django_lesson_1_0.tar server_name: # 将镜像上传到自己租的云端服务器
ssh server_name # 登录自己的云端服务器
docker load -i django_lesson_1_0.tar # 将镜像加载到本地
docker run -p 20000:22 -p 8000:8000 --name django_server -itd django_lesson:1.0 # 创建并运行django_lesson:1.0镜像 (端口要自己去云平台放行)
docker attach django_server # 进入创建的docker容器
passwd # 设置root密码
adduser acs # 创建普通用户acs
usermod -aG sudo acs # 给用户acs分配sudo权限
su -acs# 可切换到用户acs中
ctrl p + ctrl q #挂起容器
返回AC terminal，为acs用户配置别名和免密登录
vim config # 配置别名
ssh-copy-id 别名 # 为acs用户一键添加公钥，免密登录
scp .bashrc .vimrc .tmux.conf 别名: # 配置一下环境

开始施工
打开tumx
django-admin startproject acapp #创建django项目acapp
配置git：
ssh-keygen # 生成密钥用于连接到ac git上面
在git偏好设置中，打开ssh密钥，添加一下刚才生成的公钥
git init # 进到acapp中将其配置成git仓库
打开git，在git上创建一个仓库（项目）按照下面的提示在acs里面配置一下git
git config --global user.name xxx
git config --global user.email xxx@xxx.com
git add .
git commit -m "xxx"
git remote add origin git@git.acwing.com:xxx/XXX.git #建立连接
git push --set-upstream origin master
再打开一个tmux，(一个tmux用于维护控制台，另一个tmux用于开发)跑一下我们的项目 pyhton3 manage.py runserver 0.0.0.0:8000
ag ALLOWED-HOSTS #全文搜索
找到这个字段所在位置，并将自己的IP地址添加到里面
便可用 IP地址：8000 打开django页面
git 一下代码
注意：在git仓库的根目录下（acapp）,vim .gitignore
在文件里面写上 */__pycache__(两个杠)
再去添加文件的时候就不会再添加这类不必要的文件

python3 manage.py startapp game # 创建gameapp

登录django管理员界面
ctrl c 先关掉控制台
python3 manage.py migrate #同步一下数据库的修改
python3 manage.py createsuperuser # 创建管理员账号
pyhton3 manage.py runserver 0.0.0.0:8000 # 启动控制台
IP地址：8000/admin # 进到管理员登录界面，输入一下刚才创建的账号即可进到管理员界面
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
git一下代码，完工
写的有点乱，但大概思路是对的，大家就随便看看吧
