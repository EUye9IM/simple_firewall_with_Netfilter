## 内核开发环境搭建

以 Rocky8 为例

```bash
dnf install kernel-headers-`uname -r` kernel-devel-`uname -r` gcc make elfutils-libelf-devel
```

## 编译内核模块

- 进入 src/module
- `make`（编译出 simple_firewall.ko ）

## 安装模块

`make install` 或 insmod simple_firewall.ko

安装完成后在 /proc 下会有一个 personal_firewall 的空文件，即安装完成。

## 卸载模块

`make remove` 或 rmmod simple_firewall

安装完成后在 /proc 下 personal_firewall 文件消失，即卸载完成。

## api

所有交互都以读写 `/proc/personal_firewall` 文件的形式进行。

**ip 与 port 似乎有字序问题，端口 20000 在这个模块中的值似乎是 8270（20000/256=78...32, 32*256+78=8270）**

### 读取目前规则

参考 src/module/main.h

```c
#define RULE_BANIP 0x00
#define RULE_BANPORT 0x01
```

读取 /proc/personal_firewall

一行两个正整数。

第一个整数表示封禁ip或者端口

第二个整数表示封禁的ip/端口的值。

例：

1 8270 表示该机封禁了 20000 号端口

### 修改规则

参考 src/module/main.h

```c
#define FIREWALL_RULE_CLEAR         0
#define FIREWALL_RULE_ADD_IP        1
#define FIREWALL_RULE_ADD_PORT      2
#define FIREWALL_RULE_REMOVE_IP     3
#define FIREWALL_RULE_REMOVE_PORT   4
```

写入 /proc/personal_firewall

写入形式为，两个正整数，中间以空格分隔

第一个正整数表示功能，第二个为值。

- FIREWALL_RULE_CLEAR 清除所有规则，第二个整数不起作用（但还是需要有）
- FIREWALL_RULE_ADD_IP 添加封禁ip，第二个值为ip（32位无符号整数）
- FIREWALL_RULE_ADD_PORT 添加封禁ip，第二个值为port （16位无符号整数）
- FIREWALL_RULE_REMOVE_IP 移除封禁ip，第二个值为ip
- FIREWALL_RULE_REMOVE_PORT 移除封禁ip，第二个值为port

例如想要封禁端口 20000， 可以通过命令 `echo "2 8270" > /proc/personal_firewall`

此时输入 `cat /proc/personal_firewall`，因该能看到“1 8270”

## 其他

在 src/app 中开发前端应用，可以 `#include "../module/main.h"`

ip地址如何用32位表示还不太确定，可以参考 reference/app/fwtcl.c，需要再测试确认

有什么功能需要尽量提，隐约觉得目前的还是有些简单了一些？