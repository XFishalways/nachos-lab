#include "syscall.h"

int main()
{
  int result1, result2, result3;
  
  result1 = Mul(42, 23);
  result2 = Div(2, 4);
  result3 = Pow(42, 2);

  Halt();
  /* not reached */
}