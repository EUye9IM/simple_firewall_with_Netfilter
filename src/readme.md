## 开发环境搭建

以 Rocky8 为例

```bash
dnf install kernel-headers-`uname -r` kernel-devel-`uname -r` gcc-c++ make elfutils-libelf-devel
```

## 编译

`make`

## 安装

`make install`

## 卸载

`make remove` 

## pfw-cli 用法说明

## 内核 API

所有交互都以读写 `/proc/personal_firewall` 文件的形式进行。

ip 与 port 似乎有字序问题，端口 20000 在这个模块中的值似乎是 8270（20000/256=78...32, 32\*256+78=8270）

### 读取目前规则

参考 src/module/main.h

```c
#define RULE_BANIP     0x00
#define RULE_BANPORT   0x01
#define RULE_BANSUBNET 0x02
```

读取 /proc/personal_firewall

一行三个正整数。

第一个整数表示规则说明

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
#define FIREWALL_RULE_ADD_SUBNET    5
#define FIREWALL_RULE_REMOVE_SUBNET 6
```

写入 /proc/personal_firewall

写入形式为，若干个正整数，中间以空格分隔

第一个正整数表示功能，第二第三个为值。

- FIREWALL_RULE_CLEAR 清除所有规则，第二个整数不起作用（但还是需要有）
- FIREWALL_RULE_ADD_IP 添加封禁ip，第二个值为ip（32位无符号整数）
- FIREWALL_RULE_ADD_PORT 添加封禁ip，第二个值为port （16位无符号整数）
- FIREWALL_RULE_REMOVE_IP 移除封禁ip，第二个值为ip
- FIREWALL_RULE_REMOVE_PORT 移除封禁ip，第二个值为port
- FIREWALL_RULE_ADD_SUBNET 添加封禁子网，第二个整数为ip，第三个整数为mask
- FIREWALL_RULE_REMOVE_SUBNET 移除封禁子网，第二个整数为ip，第三个整数为mask

例如想要封禁端口 20000， 可以通过命令 `echo "2 8270" > /proc/personal_firewall`

此时输入 `cat /proc/personal_firewall`，因该能看到“1 8270 0”
