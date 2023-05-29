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