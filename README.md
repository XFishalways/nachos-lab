# lab 9

## Goal

利用Lock与Condition实现一个生产者-消费者问题，要求如下：
1. 创建线程时，利用fork第二个参数作为线程的编号；
2. 共有两个线程，一个生产者线程，一个消费者线程；
3. 两个线程共享一个cache；
4. 按照生产者消费者的流程打印出结果；

## Code Modification

- threads/thread.h && thread.cc
- threads/kernel.cc
