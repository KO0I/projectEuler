/*
 *
 *
 * The prime 41, can be written as the sum of six consecutive primes:
 * 41 = 2 + 3 + 5 + 7 + 11 + 13
 *
 * This is the longest sum of consecutive primes that adds to a prime below
 * one-hundred.
 *
 * The longest sum of consecutive primes below one-thousand that adds to a
 * prime, contains 21 terms, and is equal to 953.
 *
 * Which prime, below one-million, can be written as the sum of the most
 * consecutive primes?
 *
 */
#include <stdio.h>
#include <math.h>
//#define UPPER_BOUND  10000 // large prime whose previous seven primes
                           // are less than 1000000
#define UPPER_BOUND 10
#define MAX 10

FILE *fp;   // file pointer

int isPrime(int n){
  int i;
  // if it's even, give up right away
  if(n%2==0) return 1;
  for(i=3; i<(sqrt(n)); i+=2){
    if(n%i==0) return 1;
  }  
return 0;
}

int main(){
  fp = fopen("million_primes","r"); // file pointer
  int i=0;
  int primeList[UPPER_BOUND]; //largest legal primes
  int test,prime,highscore=0,offset=0,samples=5;
  char line[7]; 
  
  // generates an array of prime numbers from a file
  while(fgets(line, 8, fp) != NULL && (i < UPPER_BOUND)){
    sscanf (line, "%d", &prime);
    //printf("%d\n",prime);
    primeList[i]=prime; i++;
  }
  /*
  for(i=0;i<UPPER_BOUND;i++){
    printf("%d ",primeList[i]);
  }
  printf("\n");
*/

  //while(offset<(UPPER_BOUND/2)){
    while(test!=953){
      for(i=offset; i<=(21+offset); i++){
//      for(i=offset; ((i<=samples) && (test<MAX)); i++){
        if(i==offset) test=0; //initially, set test to zero
        test+=primeList[i]; 
        printf("%d ",primeList[i]); 
        if(i==(21+offset)){
          printf("= ");
        }
        else printf("+ ");
      }
      /*
      if(test>=MAX) break;
      printf("%d",test);
      if(isPrime(test)) {  // not so prime
        printf(" ...not prime!\n");
        samples++;
      }
      if(!isPrime(test)) { // it is prime!
        printf(" ... prime!\n");
        if(test>=highscore) highscore=test; //store the NUMBER that won 
        samples++;
      }
    }*/
      offset++;
  }
  //printf("\nLargest Consecutive Prime Sum Under %d is %d\n",MAX,highscore);

  fclose(fp);
  return 0;
}
