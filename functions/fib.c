#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

int fibnum(long n, long * fib){
  if (n == 0) return -1;
  fib[0]=1;
  fib[1]=1;
  if (n==1) return 0;
  int j;

  for(j=2; j<n; j++){
  fib[j] = fib[j-1] + fib[j-2];
  }
  return 0;
}
