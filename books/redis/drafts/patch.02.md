# debug Redis 3.0

<!-- @import "[TOC]" {cmd="toc" depthFrom=3 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [机器与 VS Code Debugger 配置](#机器与-vs-code-debugger-配置)

<!-- /code_chunk_output -->

### 机器与 VS Code Debugger 配置

```bash
# 架构
uname -m
x86_64
# Ubuntu 版本
cat /etc/issue
Ubuntu 22.04.4 LTS \n \l
# gcc 版本
gcc --version
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
# gdb 版本
gdb --version
GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1
```

VS Code 配置：

tasks.json：

```json
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Clean",
            "type": "shell",
            "command": "make",
            "args": [
                "clean"
            ],
            "problemMatcher": []
        },
        {
            "label": "Compile",
            "type": "shell",
            "command": "make",
            "args": [
                "noopt"
            ],
            "problemMatcher": []
        },
        {
            "label": "Build",
            "dependsOn": [
                "Clean",
                "Compile"
            ]
        },
        {
            "label": "writeRedisPID",
            "type": "shell",
            "command": "ps -ef | grep 'redis-server' | grep -v grep | awk '{print $2}' > /tmp/redis.pid"
        }
    ]
}
```

launch.json：

```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Redis Server Run",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/src/redis-server",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            // "preLaunchTask": "Build"
        }
        // ,
        // {
        //     "name": "Redis Server Attach",
        //     "type": "cppdbg",
        //     "request": "attach",
        //     "processId": "${input:redisPID}",
        //     "program": "${workspaceFolder}/src/redis-server",
        //     "args": [],
        //     "stopAtEntry": false,
        //     "cwd": "${fileDirname}",
        //     "environment": [],
        //     "externalConsole": false,
        //     "MIMode": "gdb",
        //     "preLaunchTask": "writeRedisPID",
        //     "subProcess": true,
        //     "justMyCode": true,
        //     "redirectOutput": true,
        // }
    ],
    "inputs": [
        {
          "id": "redisPID",
          "type": "command",
          "command": "extension.commandvariable.file.content",
          "args": {
            "fileName": "/tmp/redis.pid"
          }
        }
      ]
}
```
