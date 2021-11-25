# 创建账号系统（下）（AcApp端AcWing一键登录）

- 作者：yxc
- 链接：https://www.acwing.com/blog/content/12467/
- 来源：AcWing
- 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [AcApp流程](#acapp流程)

<!-- /code_chunk_output -->

细分目录：

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=4 orderedList=false} -->

<!-- code_chunk_output -->

- [AcApp流程](#acapp流程)
  - [后端相关流程](#后端相关流程)
  - [前后端实现要点](#前后端实现要点)

<!-- /code_chunk_output -->

### AcApp流程

#### 后端相关流程

![](./images/2021112502.png)

<h4>第一步 申请授权码<code>code</code></h4>

请求授权码的API：

```
AcWingOS.api.oauth2.authorize(appid, redirect_uri, scope, state, callback);
```

<h5>参数说明</h5>

<table>
<thead>
<tr>
<th>参数</th>
<th>是否必须</th>
<th>说明</th>
</tr>
</thead>
<tbody>
<tr>
<td>appid</td>
<td>是</td>
<td>应用的唯一id，可以在AcWing编辑AcApp的界面里看到</td>
</tr>
<tr>
<td>redirect_uri</td>
<td>是</td>
<td>接收授权码的地址。需要用urllib.parse.quote对链接进行处理。</td>
</tr>
<tr>
<td>scope</td>
<td>是</td>
<td>申请授权的范围。目前只需填userinfo</td>
</tr>
<tr>
<td>state</td>
<td>是</td>
<td>用于判断请求和回调的一致性，授权成功后后原样返回。该参数可用于防止csrf攻击（跨站请求伪造攻击），建议第三方带上该参数，可设置为简单的随机数</td>
</tr>
<tr>
<td>callback</td>
<td>是</td>
<td>redirect_uri返回后的回调函数</td>
</tr>
</tbody>
</table>

<h5>返回说明</h5>

用户同意授权后，会将`code`和`state`传递给`redirect_uri`。

如果用户拒绝授权，则将会收到如下错误码：

```
{
    errcode: "40010"
    errmsg: "user reject"
}
```

<hr />

<h4>第二步 申请授权令牌<code>access_token</code>和用户的<code>openid</code></h4>

请求地址：`https://www.acwing.com/third_party/api/oauth2/access_token/`

参考示例：

```
请求方法：GET
https://www.acwing.com/third_party/api/oauth2/access_token/?appid=APPID&amp;secret=APPSECRET&amp;code=CODE
```

<h5>参数说明</h5>

<table>
<thead>
<tr>
<th>参数</th>
<th>是否必须</th>
<th>说明</th>
</tr>
</thead>
<tbody>
<tr>
<td>appid</td>
<td>是</td>
<td>应用的唯一id，可以在AcWing编辑AcApp的界面里看到</td>
</tr>
<tr>
<td>secret</td>
<td>是</td>
<td>应用的秘钥，可以在AcWing编辑AcApp的界面里看到</td>
</tr>
<tr>
<td>code</td>
<td>是</td>
<td>第一步中获取的授权码</td>
</tr>
</tbody>
</table>

<h5>返回说明</h5>

申请成功示例：

```
{ 
    "access_token": "ACCESS_TOKEN", 
    "expires_in": 7200, 
    "refresh_token": "REFRESH_TOKEN",
    "openid": "OPENID", 
    "scope": "SCOPE",
}
```

申请失败示例：

```
{
    "errcode": 40001,
    "errmsg": "code expired",  # 授权码过期
}
```

<h5>返回参数说明</h5>

<table>
<thead>
<tr>
<th>参数</th>
<th>说明</th>
</tr>
</thead>
<tbody>
<tr>
<td>access_token</td>
<td>授权令牌，有效期2小时</td>
</tr>
<tr>
<td>expires_in</td>
<td>授权令牌还有多久过期，单位（秒）</td>
</tr>
<tr>
<td>refresh_token</td>
<td>用于刷新access_token的令牌，有效期30天</td>
</tr>
<tr>
<td>openid</td>
<td>用户的id。每个AcWing用户在每个acapp中授权的openid是唯一的,可用于识别用户。</td>
</tr>
<tr>
<td>scope</td>
<td>用户授权的范围。目前范围为userinfo，包括用户名、头像</td>
</tr>
</tbody>
</table>

<hr />

<h5>刷新access_token的有效期</h5>

`access_token`的有效期为2小时，时间较短。`refresh_token`的有效期为30天，可用于刷新`access_token`。刷新结果有两种：

- 如果<code>access_token</code>已过期，则生成一个新的<code>access_token</code>。
- 如果<code>access_token</code>未过期，则将当前的<code>access_token</code>的有效期延长为2小时。

参考示例：

```
请求方法：GET
https://www.acwing.com/third_party/api/oauth2/refresh_token/?appid=APPID&amp;refresh_token=REFRESH_TOKEN
```

返回结果的格式与申请`access_token`相同。

<hr />

<h4>第三步 申请用户信息</h4>

请求地址：`https://www.acwing.com/third_party/api/meta/identity/getinfo/`

参考示例：

```
请求方法：GET
https://www.acwing.com/third_party/api/meta/identity/getinfo/?access_token=ACCESS_TOKEN&amp;openid=OPENID
```

<h5>参数说明</h5>

<table>
<thead>
<tr>
<th>参数</th>
<th>是否必须</th>
<th>说明</th>
</tr>
</thead>
<tbody>
<tr>
<td>access_token</td>
<td>是</td>
<td>第二步中获取的授权令牌</td>
</tr>
<tr>
<td>openid</td>
<td>是</td>
<td>第二步中获取的用户openid</td>
</tr>
</tbody>
</table>
<h5>返回说明</h5>

申请成功示例：

```
{
    'username': "USERNAME",
    'photo': "https:cdn.acwing.com/xxxxx"
}
```

申请失败示例：

```
{
    'errcode': "40004",
    'errmsg': "access_token expired"  # 授权令牌过期
}
```

#### 前后端实现要点
