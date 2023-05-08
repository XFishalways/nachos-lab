# Lab 7

## Goal

- 模仿code/test/add.c，阅读并理解其实现过程，实现求乘法，除法、乘方三种系统调用

## Process

1. 修改文件
    1. "userprog/syscall.h"
    2. "test/start.s"
    3. "userprog/exception.cc"
    4. "userprog/ksyscall.h"
2. 在test目录下新建用户测试程序math.c，在Makefile的SOURCES中添加用户程序名
3. 分别对整个nachos系统和test目录重新编译: `make clean` -> `make`
4. 在code/test目录下运行命令 `../build.linux/nachos -x math.noff -d u` 得到实验结果

## Code Modification

- userprog/exception.cc
- userprog/ksyscall.h
- userprog/syscall.h
- test/math.c
- test/start.s
- test/Makefile
