# Code-Ana

词法分析 Lexical Analysis / 函数层次的CFG抽取 CFG Extraction / 同源性检测 Homology Detection / 代码漏洞检测 Vulnerability Detection



## Features

- GUI用QT编写；核心部分利用C++ STL实现，可独立运行
- 分析的目标语言为C / Simple C++
- 利用词法分析和函数层级的CFG抽取进行代码的同源性检测
- AC自动机扫描得到可能存在漏洞的函数，并根据自定义规则报告漏洞



## 功能

- R1：GUI
- R2：利用字符串匹配进行同源性检测
- R3：利用控制流程图（CFG）进行同源性检测
- A2：支持分布式任务调度
- R4: 栈缓冲区检测
- B1: 堆缓冲区检测
- R5: 格式化字符串漏洞检测
- B2: 整数宽度溢出检测
- B3: 整数运算溢出检测
- B4: 整数符号溢出检测
