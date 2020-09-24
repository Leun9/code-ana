# Code-Ana

词法分析 Lexical Analysis / 函数层次的CFG抽取 CFG Extraction / 同源性检测 Homology Detection



# Features

- GUI用QT编写；核心部分仅利用STL实现，与QT无关
- 分析的目标语言为C或简单的C++
- 利用词法分析和函数层级的CFG抽取进行代码的同源性检测
- 



## 功能

- R1：GUI
- R2：利用字符串匹配进行同源性检测
- R3：利用控制流程图（CFG）进行同源性检测
- A2：支持分布式任务调度



### R1: GUI

QT实现。



### R2: 利用字符串匹配进行同源性检测

利用确定有限状态自动机（Deterministic Finite Automaton， DFA）进行词法分析，将程序转换为token形成的行链表。得到源代码和目标代码的token行链表后，利用时间复杂度为O(nm)、空间复杂度为O(n)的动态规划算法，得到所有行数大于阈值的公共代码块，根据代码块的总长度计算代码之间的相似度。



### R3: 利用控制流程图（CFG）进行同源性检测

扫描代码，得到函数调用关系图。考虑可能的代码跳转情况，将执行代码对应的token顺序连接，形成CFG对应的工作流程。利用时间复杂度为O(nm)、空间复杂度为O(n)的动态规划算法，得到所有token个数大于阈值的公共代码块，根据公共代码块的总长度计算代码之间的相似度。



### A2: 支持分布式任务调度

利用C++11的thread类和QT的signals & slots机制实现。线程完成子任务后，发出signals返回执行结果；主线程slots逐一处理队列中的signals。

