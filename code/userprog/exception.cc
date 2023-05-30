// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

void SimpleTLBMissHandler(int virtAddr);
void TLBMissHandler(int virtAddr);
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
	case PageFaultException:
		#ifdef USE_TLB
			// SimpleTLBMissHandler(kernel->machine->ReadRegister(BadVAddrReg));
			TLBMissHandler(kernel->machine->ReadRegister(BadVAddrReg));
		#endif
		
    case SyscallException:
      switch(type) {
      case SC_Halt:
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

	SysHalt();

	ASSERTNOTREACHED();
	break;

      case SC_Add:
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
	int resultAdd;
	resultAdd = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << resultAdd << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)resultAdd);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;
	
      case SC_Mul:
	DEBUG(dbgSys, "Mul " << kernel->machine->ReadRegister(4) << " * " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysMul Systemcall*/
	int resultMul;
	resultMul = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << resultMul << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)resultMul);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;
	
      case SC_Div:
	DEBUG(dbgSys, "Div " << kernel->machine->ReadRegister(4) << " / " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysDiv Systemcall*/
	int resultDiv;
	resultDiv = SysDiv(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << resultDiv << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)resultDiv);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;
	
      case SC_Pow:
	DEBUG(dbgSys, "Pow " << kernel->machine->ReadRegister(4) << " ^ " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
	int resultPow;
	resultPow = SysPow(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << resultPow << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)resultPow);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;

	// exception handler "create"
	  case SC_Create:
	DEBUG(dbgSys, "Create " << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysCreate Systemcall*/
	int base, value, count;
	base = kernel->machine->ReadRegister(4);
	count = 0;
	char* paramStr;
	paramStr = new char[128];
	do {
		kernel->machine->ReadMem(base + count, 1, &value);
		paramStr[count] = *(char*) &value;
		count++;
	} while (*(char*) &value != '\0' && count < 128);
	
	/* Prepare Result */
	int CreReturn;
	CreReturn = SysCreate(paramStr);
	if (CreReturn == -1) {
		printf("create file failed\n");
		kernel->machine->WriteRegister(2, -1);
	} else {
		printf("create return with %d", CreReturn);
		kernel->machine->WriteRegister(2, 1);
	}

	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "open"
	  case SC_Open:
	DEBUG(dbgSys, "Create " << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysOpen Systemcall*/
	base = kernel->machine->ReadRegister(4);
	count = 0;

	int fileid;

	paramStr = new char[128];
	do {
		kernel->machine->ReadMem(base + count, 1, &value);
		paramStr[count] = *(char*) &value;
		count++;
	} while (*(char*) &value != '\0' && count < 128);
	
	/* Prepare Result */
	fileid = SysOpen(paramStr);
	if (fileid < 1) {
		printf("failed to open file %s\n", paramStr);
		kernel->machine->WriteRegister(2, -1);
	} else {
		printf("file %s successfully opened, fileId: %d\n", paramStr, fileid);
		kernel->machine->WriteRegister(2, 1);
	}
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "write"
	  case SC_Write:
	DEBUG(dbgSys, "Write " << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysWrite Systemcall*/
	int wbase, wsize, wfileId, wcount, wvalue;
	wbase = kernel->machine->ReadRegister(4);
	wsize = kernel->machine->ReadRegister(5);
	wfileId = kernel->machine->ReadRegister(6);
	wcount = 0;
	char* wparamStr;
	wparamStr = new char[128];
	do {
		kernel->machine->ReadMem(wbase + wcount, 1, &wvalue);
		wparamStr[wcount] = *(char*) &wvalue;
		wcount++;
	} while (*(char*) &wvalue != '\0' && wcount < wsize);

	/* Prepare Result */
	wcount = SysWrite(wparamStr, wsize, wfileId);
	if (wcount > -1) {
		printf("\"%s\" has been written to file successfully\n", wparamStr);
		kernel->machine->WriteRegister(2, wcount);
	} else {
		printf("failed to write\n");
		kernel->machine->WriteRegister(2, 1);
	}
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "read"
	  case SC_Read:
	DEBUG(dbgSys, "Read " << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysRead Systemcall*/
	int rbase, rsize, rfileId, rcount;
	rbase = kernel->machine->ReadRegister(4);
	rsize = kernel->machine->ReadRegister(5);
	rfileId = kernel->machine->ReadRegister(6);
	char* rparamStr;
	rparamStr = new char[128];

	/* Prepare Result */
	rcount = SysRead(rparamStr, rsize, rfileId);
	if (rcount) {
		for (int i = 0; i < rcount; i++) {
			kernel->machine->WriteMem(rbase + i, 1, (int)rparamStr[i]);
		}
		printf("succcessfully read, length: %d, content: %s\n", rcount, rparamStr);
	} else {
		printf("failed to read\n");
	}
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "close"
	  case SC_Close:
	DEBUG(dbgSys, "Close" << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysWrite Systemcall*/
	int fileId;
	fileId = kernel->machine->ReadRegister(4);

	/* Prepare Result */
	if (SysClose(fileId)) {
		printf("file %d successfully closed\n", fileId);
		kernel->machine->WriteRegister(2, 1);
	} else {
		printf("file %d failed to close\n", fileid);
		kernel->machine->WriteRegister(2, -1);
	}
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "remove"
	  case SC_Remove:
	DEBUG(dbgSys, "Remove" << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysWrite Systemcall*/
	fileId = kernel->machine->ReadRegister(4);

	for (count = 0; (count < 128 && (char)value != '\0'); count++) {
		kernel->machine->ReadMem(fileId + count, 1, &value);
		paramStr[count] = (char)value;
	}
	
	/* Prepare Result */
	if (SysRemove(paramStr)) {
		printf("\"%s\" has been removed successfully\n", paramStr);
		kernel->machine->WriteRegister(2, 1);
	} else {
		printf("file %s has failed to write\n", paramStr);
		kernel->machine->WriteRegister(2, -1);
	}

	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "exec"
	  case SC_Exec:
	DEBUG(dbgSys, "Exec" << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysExec Systemcall*/
	result = SysExec((int)kernel->machine->ReadRegister(4));
	DEBUG(dbgSys, "Exec returning with " << result << "\n");

	/* Prepare Result */
	kernel->machine->WriteRegister(2, result);
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;


	// exception handler "join"
	  case SC_Join:
	DEBUG(dbgSys, "Join" << kernel->machine->ReadRegister(4) << "\n");
	
	/* Process SysJoin Systemcall*/
	result = SysJoin((int)kernel->machine->ReadRegister(4));
	DEBUG(dbgSys, "Join returning with " << result << "\n");


	/* Prepare Result */
	kernel->machine->WriteRegister(2, result);
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;

      default:
	cerr << "Unexpected system call " << type << "\n";
	break;
      }
      
      break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    // ASSERTNOTREACHED();
}


int pointer = 0;
void SimpleTLBMissHandler(int virtAddr) 
{
	unsigned int vpn;
	vpn = (unsigned)virtAddr / PageSize;
	DEBUG('a', "\n>>>>>>>add page number " << vpn << " to tlb pointer " << kernel->machine->tlb->virtualPage << " : " << kernel->machine->tlb->physicalPage);
	kernel->machine->tlb[pointer] = kernel->machine->pageTable[vpn];
	DEBUG('a', ">>>>>>>>This is the current tlb table pointer 0: " << kernel->machine->tlb->virtualPage << " : " << kernel->machine->tlb->physicalPage);
	pointer = pointer ? 0 : 1;
}

void TLBMissHandler(int virtAddr) 
{
	unsigned int vpn;
	vpn = (unsigned)virtAddr / PageSize;
	int clockpoint = -1;

	while (clockpoint == -1) {
		DEBUG('a', "tlb NRU started\n");
		cout << "tlb NRU started\n";
		for (int i = 0; i < TLBSize; ++i) {
			if (!kernel->machine->tlb[i].use && !kernel->machine->tlb[i].dirty) {
				clockpoint = i;
				break;
			}
		}

		if (clockpoint == -1) {
			for (int i = 0; i < TLBSize; ++i) {
				if (!kernel->machine->tlb[i].use && !kernel->machine->tlb[i].dirty) {
					clockpoint = i;
					break;
				} else {
					kernel->machine->tlb[i].use = false;
				}
			}
		}
	}

	cout << "(1) TLB before replacement:\n";
	for (int i = 0; i < TLBSize; ++i) {
		cout << "tlb[" << i << "] use : " << kernel->machine->tlb[i].use;
	}
	cout << "(2) tlb" << clockpoint << "has been changed";
	cout << vpn;
}