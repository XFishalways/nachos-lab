# lab 11

## Goal

1.	实现用户程序中的以下文件系统调用，从而支持NachOS的用户程序中的文件创建、打开、读出、写入、关闭操作: create(), open(), read(), write(), close()
2.	实现支持NachOS Shell的以下系统调用，从而支持NachOS的用户程序中的shell命令执行: exec(), join()


## Code Modification

- userprog/exception.cc
- machine/machine.cc
- machine/mipssim.cc
- machine.translate.cc
- test/sort.cc
- build.linux/Makefile
