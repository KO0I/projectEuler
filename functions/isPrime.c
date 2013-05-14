#include<math.h>

int isPrime(int n){
//if n is prime, return 0. If n is not prime, return 1.

//rule out corner cases 0,1,2:
if (n==0 || n==1) return 1;
if(n==2) return 0;
if(n%2==0) return 1;

//we only need to test up to the Sqrt[n], so our largest test value will
//be this value.
int i=3,max;
double dn = (double) n;
max=(int) sqrt(dn);
//printf("%d,%f,%d\n",n,dn,max);

while (i<=max){
  if(n % i == 0) return 1;
  i+=2;
  }
return 0;
}
