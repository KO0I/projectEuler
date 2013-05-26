#include <stdio.h>
#include <time.h>

//  Problem 46
//  Golbach's Other Conjecture
//    Uses powi.c and isPrime.c from functions
//  Here, we will attempt to find the first composite number that is not 
//  the sum of a prime number and twice any square.

//  for optimization:
clock_t begin,end;
double time_spent;

int main(){
  begin = clock();
  //First, let's get a million primes.
  FILE *primefile;
  if((primefile = fopen("million_primes","r")) == NULL) {
    printf("cannot open million_primes for reading.\n");
  return 1;
}
  int prime[1000000];
  int n, i = 0;
  while(fscanf(primefile, "%d", &n) > 0) {
    prime[i] = n;
    i++;
  }
  fclose(primefile);

//  Now for the logic:
//  the equation for Golbach's conjecture follows:
//  z = p + 2*(m)^2
//  where z is an odd composite number
//  p is a prime number
//  m is any number

int j=0,k,m,p,z=2,counterexample=1;
int odd[1000000];
  // find suitable odd, composite numbers
  while(j<1000000){
    if ( (isPrime(z)==1) && ((z+1)%2==0)){
      //printf("%d\n",z);
      odd[j]=z;
      j++;
      z++;
  }else z++;
}

// reset i and j for the main while loop
i=0; j=0;

//let's first create an algorithm that effectively searches for a
//solution to the conjecture for known working cases.
while(i<=1000000){
  //counterexample state assumed to be 1 until proven otherwise.
  counterexample=1; 
    for(m=1; m <= odd[i]; m++){
      for(j=0; (prime[j] <= odd[i]); j++){
        if( prime[j]+(2*(powi(m,2))) == odd[i] ){
          //follows the conjecture, that's nice, let's try something
          //different 
          i++;
          counterexample=0;
          }
        }//  end of prime loop
      }//  end of m loop
  if (counterexample==1) {
    printf("%d is the first counterexample\n",odd[i]);
    break;
    }
}
end = clock();
time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
printf("runtime: %f\n seconds",time_spent);
return 0;
}
