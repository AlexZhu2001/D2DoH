# D2DoH
一个本地DNS服务器，他将DNS请求转化为DNS over HTTPS

An local DNS server translate DNS datagram to DNS over HTTPS

![GitHub](https://img.shields.io/github/license/AlexZhu2001/D2DoH)
![GitHub issues](https://img.shields.io/github/issues/AlexZhu2001/D2DoH)
![GitHub last commit](https://img.shields.io/github/last-commit/AlexZhu2001/D2DoH)

## How To Use
双击即可运行，程序将监听127.0.0.1（开启IPv4）或 ::1（开启IPv6）

将系统DNS地址设置为127.0.0.1或::1即可

默认配置为Aliyun公共DNS，在托盘->设置中可以修改

Double click to run. The program will listen to 127.0.0.1 (if IPv4 enabled) or:: 1 (if IPv6 enabled)

Set the system DNS address to 127.0.0.1 or ::1

The default configuration is Aliyun public DNS, which can be modified in SystemTray->Options

## 3-rd Party Library
本项目使用QT开发 基于QT5.15.2

This project is based on QT framework 

The version of QT is 5.15.2

## License
该项目签署了MIT 授权许可，详情请参阅[***LICENSE***](LICENSE)

This project is under MIT license. For details, please refer to [***LICENSE***](LICENSE).

## Reference
[RFC1035 DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION](https://datatracker.ietf.org/doc/html/rfc1035)

[RFC8484 DNS Queries over HTTPS (DoH)](https://datatracker.ietf.org/doc/html/rfc8484)
