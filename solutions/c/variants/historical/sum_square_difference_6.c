#include<stdio.h>
#include<math.h>

unsigned long squareSum(unsigned long n){
  unsigned long i;
  unsigned long p=0;
  for(i=0; i<=n; i++){
    p += i;
  }
  p*=p;
  return p;
}

unsigned long sumSquare(unsigned long n){
  unsigned long j;
  unsigned long q=0,r=0;
  for(j=0; j<=n; j++){
    q = j*j;
    r+=q;
  }
  return r;
}

int main(){
  unsigned long n = 100;
  unsigned long sol=0;
  sol=squareSum(n)-sumSquare(n);
  printf("%lu - %lu = %lu\n",squareSum(n),sumSquare(n),sol);
  return 0;
}
