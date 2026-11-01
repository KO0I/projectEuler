#include <stdio.h>
#include <assert.h>

// Problem 47
// Distinct Prime Factors
// Uses the million_primes list to quickly find the prime factorization of
// numbers. Also uses the primeFact.c function to get the number of prime
// factors for each test-case integer

int main(){
  //Fetch them primes
  /*
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
  */

  //  first two CONSECUTIVE numbers to have TWO distinct prime numbers are:
  //  14=(2*7)and 15=(3*5)
  //  first three CONSECUTIVE numbers to have THREE distinct prime numbers are:
  //  644=((2^2)*7*23
  //  645=(3*5*43)
  //  646=(2*17*19)
  //  The task of this problem is to find the first FOUR consecutive composites
  //  that have FOUR distinct prime numbers, where the first of the four
  //  numbers is the answer to problem 47.

  //  score is the max number of primes and consecutive numbers to be sought out
  int score = 4;
  //  start with the first composite number
  int i = 4;
  //  jth prime
  int j = 0;
  assert(distinctpfactors(60)==3);
  for(i=4; i<20000000; i++){
  //  time to factorize!
    if((distinctpfactors(i)   == score) && 
       (distinctpfactors(i+1) == score) && 
       (distinctpfactors(i+2) == score) && 
       (distinctpfactors(i+3) == score)
       ){
      printf("%d,%d,%d,%d\n",i,i+1,i+2,i+3);
      break;
    }
  }
return 0;
}
