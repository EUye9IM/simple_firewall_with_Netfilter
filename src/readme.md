## 软件设计

软件分为两个部分，一个内核模块与一个可执行应用程序。

### 内核模块 

personal_filter.ko

- 负责挂 netfilter 的 input 和 output 的钩子
- 负责在 /proc/personal_filter/rules 中写入目前规则
- 负责接受 socket 通信修改规则
- 
- 读取配置文件
- 维护规则链表

### 应用程序

personal_firewall

- 读取 /proc/personal_filter/rules 中的规则
- setsockopt 修改规则
- 启动、关闭 filter

