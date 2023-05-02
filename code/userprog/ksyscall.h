/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"




void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysMul(int op1, int op2)
{
  return op1 * op2;
}

int SysDiv(int op1, int op2)
{
  return op1 / op2;
}

int SysPow(int op1, int op2)
{
  int base = op1;
  for(int i = 0; i < op2; i++)
  {
    op1 = op1 * base;
  }
  return op1;
}







#endif /* ! __USERPROG_KSYSCALL_H__ */
