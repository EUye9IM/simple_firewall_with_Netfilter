#include <arpa/inet.h>
#include <bits/types/FILE.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std::literals;

void Usage(const char* exeName) {
  std::cout << "Personal Firewall CLI —— 简易个人防火墙规则管理命令行接口\n";
  std::cout << "usage: " << exeName << " <command> <args>";
  std::cout
      << "<command>:\n"
         "\tlist\t列出当前所有规则\n"
         "\tclear\t清除所有规则\n"
         "\tblock-ip [ip地址列表]\t增加规则：拦截目的地址在列表中的所有包\n"
         "\tblock-port [端口列表]\t增加规则：拦截目的端口在列表中的所有包\n"
         "\tallow-ip [ip地址列表]\t移除规则：不再拦截目的端口在列表中的包\n"
         "\tallow-port [端口列表]\t移除规则：不再拦截目的端口在列表中的包\n";
}

enum class RuleType { IP, Port };

const int BufferSize = 10240;
static char buf[BufferSize + 1];  // 内核模块的要求是一次性读满10240字节

struct Rule {
  RuleType type;
  std::string value;
};

const char* firewall_proc_addr = "/proc/personal_firewall";

template <typename T>
void write_rule(int rule_id, T value) {
  FILE* fout = fopen(firewall_proc_addr, "w");
  if (!fout) {
    std::cerr << "Personal Firewall模块异常退出！\n";
    exit(-1);
  }

  std::ostringstream sout;
  sout << rule_id << " " << value;
  size_t n = sout.tellp();
  strncpy(buf, sout.str().c_str(), n + 1);
  fwrite(buf, n + 1, 1, fout);
  fclose(fout);
}

void write_ip_rule(int rule_id, const char* addr) {
  uint32_t value;
  if (inet_aton(addr, reinterpret_cast<in_addr*>(&value)) != 0) {
    write_rule(rule_id, value);
  } else {
    std::cout << "Warning: 非法的IP地址" << addr << "\n";
  }
}

void write_port_rule(int rule_id, const char* port) {
  int value = std::atoi(port);
  if (value >= 0 && value <= 65536) {
    write_rule(rule_id, htons(value));
  } else {
    std::cout << "Warning: 非法的端口" << port << "\n";
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    Usage(argv[0]);
    return 0;
  }

  if (access(firewall_proc_addr, F_OK) != 0) {
    std::cout << "Personal Firewall模块未启动！\n";
    return 0;
  }

  if (access(firewall_proc_addr, R_OK | W_OK) != 0) {
    std::cout << "权限不足\n";
    return 0;
  }

  if (argv[1] == "list"s) {
    FILE* fin = fopen(firewall_proc_addr, "r");

    size_t n = fread(buf, 1, BufferSize, fin);
    buf[n] = '\0';  //当作字符串处理

    std::stringstream sin(buf);

    std::vector<std::string> blocked_ip;
    std::vector<ushort> blocked_port;

    int type;
    uint32_t data;
    while (sin >> type >> data) { // 现在一行三个正整数了 记得改，或者按行读
      switch (type) {
        case 0:
          blocked_ip.push_back(
              std::string(inet_ntoa(*reinterpret_cast<in_addr*>(&data))));
          break;
        case 1:
          blocked_port.push_back(static_cast<ushort>(data));
          break;
        default:
          std::cout << "Warning: 读取到不支持的协议类型" << std::ios::hex
                    << type << ", 请检查客户端版本\n";
          break;
      }
    }
    std::cout << "当前规则列表：\n"
              << "\t禁止访问的ip:\n";
    for (auto& ip : blocked_ip) {
      std::cout << "\t\t" << ip << "\n";
    }

    std::cout << "\t禁止访问的端口:\n";
    for (auto& port : blocked_port) {
      std::cout << "\t\t" << ntohs(port) << "\n";
    }
  } else if (argv[1] == "clear"s) {
    write_rule(0, 0);
  } else if (argv[1] == "block-ip"s) {
    for (int i = 2; i < argc; i++) {
      write_ip_rule(1, argv[i]);
    }
  } else if (argv[1] == "allow-ip"s) {
    for (int i = 2; i < argc; i++) {
      write_ip_rule(3, argv[i]);
    }
  } else if (argv[1] == "block-port"s) {
    for (int i = 2; i < argc; i++) {
      write_port_rule(2, argv[i]);
    }
  } else if (argv[1] == "allow-port"s) {
    for (int i = 2; i < argc; i++) {
      write_port_rule(4, argv[i]);
    }
  } else {
    std::cout << "命令非法。\n";
    Usage(argv[0]);
  }
  return 0;
}