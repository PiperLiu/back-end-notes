参考：
- 【WSL】WSL折腾之旅（3）SSH远程连接 - 爱折腾的小码农的文章 - 知乎 https://zhuanlan.zhihu.com/p/355748937
- 如何用笔记本ssh连接局域网内其他电脑上的wsl2 ubuntu - 巴布的文章 - 知乎 https://zhuanlan.zhihu.com/p/357038111

对于 wsl2 ：

```bash
sudo apt-get remove openssh-server
sudo apt-get install openssh-server

sudo vi /etc/ssh/sshd_config
# 设置 Port 2222
# 设置 PermitRootLogin yes
# 设置PasswordAuthentication yes

sudo vim /etc/hosts.allow
# 添加 sshd: ALL

sudo service ssh --full-restart

ifconfig
```

对于 Powershell ，需要管理员权限：

```powershell
# 将端口转发到 wsl
netsh interface portproxy add v4tov4 listenaddress=0.0.0.0 listenport=2222 connectaddress=[WSL2的IP] connectport=[WSL2的PORT 2222]

# 开启防火墙入站规则
netsh advfirewall firewall add rule name=WSL2 dir=in action=allow protocol=TCP localport=2222
```

可以了， `ssh name@ip -p 2222` 就行。

注意最后要用外地机器 `ssh` 到 windows 的 ip 上；另外需要在同一个 WLAN 下。
