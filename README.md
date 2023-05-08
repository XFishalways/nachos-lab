## lab 8

### Goal

1. 给线程类增加两个成员：usrID, ThreadID,用来记录线程所属用户进程和线程自身ID；
2. 修改Nachos线程管理模式，根据用户自身ID限制系统最多只能产生128个线程；
3. 将线程调度算法从FCFS转变为基于优先级的调度算法。

### Code modification

- threads/scheduler.h && scheduler.cc
- threads/thread.h && thread.cc
- threads/kernel.cc
- threads/main.cc
- lib/sysdep.h && sysdep.cc

