/*
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
#include <time.h>
#define UPPER_BOUND 10000
#define MAX 1000000

FILE *fp;   // file pointer
/*
int isPrime(int n){
  int i;
  // if it's even, give up right away
  if(n%2==0) return 1;
  for(i=3; i<((sqrt(n)+5)); i+=2){
    if(n%i==0) return 1;
  }
return 0;  
}
*/

int main(){
  clock_t begin,end;
  double time_spent;
  begin = clock();
  fp = fopen("million_primes","r"); // file pointer
  int i=0,j=0;
  int primeList[UPPER_BOUND]; //largest legal primes
  int prevtest,curtest=0,prime,highscore=0,offset=0;
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
  while(offset<(UPPER_BOUND/2)){
    curtest=0;j=0;
    while(curtest<MAX){
      prevtest=curtest;
      curtest+=primeList[j+offset];
      j++;
    }
    //print status
    //printf("tested: %d, prime: %d, offset: %d, score: %d\n",prevtest,isPrime(prevtest),offset,j);
    if(isPrime(prevtest)){ // not prime
      offset++;
    }
    if(!(isPrime(prevtest))){  // prime!
      if(j>=highscore){
        highscore=prevtest;
      }
      offset++;
    }
  }
  printf("Prime that is Composed of the Most Consecutive Primes\n  %d\n",highscore);

  fclose(fp);
  end=clock();
  time_spent=(double)(end-begin)/CLOCKS_PER_SEC;
  printf("Time spent: %f [ms]\n",(time_spent*1000));
  return 0;
}
