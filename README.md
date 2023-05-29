# lab 10

## Goal

1.	在Nachos现有页表的基础上，增加TLB快表机制，使得在做虚拟地址到物理地址的转换时，优先从TLB快表中读取；
2.	针对TLB增加NRU置换算法

## Code Modification

- userprog/exception.cc
- machine/machine.cc
- machine/mipssim.cc
- machine.translate.cc
- test/sort.cc
- build.linux/Makefile
