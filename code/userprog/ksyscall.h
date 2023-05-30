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

int SysCreate(char* name) {
  if (kernel->fileSystem->Create(name)) return 1;
  else return -1;
}

OpenFileId SysOpen(char* name) {
  int fileId;
  fileId = OpenForReadWrite(name, FALSE);
  if(!fileId) return -1;
  else return fileId;
}

int SysWrite(char* buffer, int size, OpenFileId id) {
  OpenFile* openfile = new OpenFile(id);
  int wlength;
  wlength = openfile->Length();
  int num;
  num = openfile->WriteAt(buffer, size, wlength);
  if(num) return num;
  else return -1;
}

int SysRead(char* buffer, int size, int id) {
  OpenFile* openfile = new OpenFile(id);
  int readnum = 0;
  if(readnum = openfile->Read(buffer, size)) {
    return readnum;
  } else {
    return -1;
  }
}

int SysRemove(char* name) {
  if(kernel->fileSystem->Remove(name)) return 1;
  else return -1;
}

int SysClose(int fileID) {
  if(!Close(fileID)) return -1;
  return 1;
}

int SysExec(int addr) {
  int count = 0;
  int ch;
  char cmd[60];
  do {
    kernel->machine->ReadMem(addr, 1, &ch);
    addr++;
    cmd[count] = (char)ch;
  } while (ch != '\0' && count++ < 59);
  cmd[count] = '\0';

  if(cmd != NULL) {
    AddrSpace* space = new AddrSpace;
    ASSERT(space != (AddrSpace*)NULL);
    if (space->Load(cmd)) {
      space->Execute();
    }
  }
}

int SysJoin(int procid) {
  pid_t retid;
  retid = waitpid((pid_t)procid, (int*)0, 0);
  return retid;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
