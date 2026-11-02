#include<math.h>

int isPrime(int n){ // returns 1 if a number is non-prime, 0 if prime
  int i;
  if(n%2==0) return 1;
  for(i=3; i<(sqrt(n)+2); i+=2){
    if(n%i==0) return 1;
  }
  return 0;
}

