#include "syscall.h"

int main()
{
  int result1;
  int result2;
  int result3;

  result1 = Mul(2, 3);
  result2 = Div(6, 2);
  result3 = Pow(2, 3);

  Halt();
  /* not reached */
}