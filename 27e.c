/* Quadratic Primes
 * 
 * Special formulas generate consecutive prime numbers. Some examples are:
 * n^2 + 1n + 41  --> generates 40 primes with n=[0,39]
 * n^2 -79n +1601 --> generates 80 primes with n=[0,79]
 *
 * In general, the form of these quadratics are:
 * n^2 + (a n) + b where |a|<1000, |b|<1000 and |n| is the modulus/abs value n
 *
 * Find a quadratic expression the produces the largest number of consecutive
 * primes, and then take the product of the coefficients a and b for this
 * expression
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int isPrime(int candidate){
  int result=1, i;
  double dcan = (double) candidate;
  int sq = (long) ((sqrt(dcan))+0.5); //sqrt of the candidate number
  if(candidate<1){ 
    result=1; // 0 and 1 are not primes; ignore negative numbers
  }
  if(candidate>1){
    for(i=2; i<=sq; i++){
      if(candidate % i == 0){ 
        result = 1; 
        break;
      }
      else result = 0;
    }
  }
  return result; // 0 is prime, 1 is not prime
}

int ipow(int base, int exp){
  int result=1;
  while(exp){
    if(exp & 1) result *=base;
    exp >>=1;
    base *= base;
  }
  return result;
}

int n=0, a, b, z, valid_seq=1, score=0, highscore=1, besta, bestb, finalanswer;

int main(){
  a=1; b=0;
  for(a=-1000; a<1000; a++){
    for(b=-1000; b<1000; b++){
      valid_seq=1;
      //printf("testing: n^2 + %d n + %d\n",a,b);
      while(valid_seq==1){
        int x, y;
        //n^2+ (a*n)+b
        //x      y
        x = ipow(n,2);
        y = (a*n);
        z = (x+y+b);
        //printf("%d = %d^2 + (%d %d) + %d\n",z,n,a,n,b);
        if(isPrime(z)!=0){// if the result is not a consecutive prime
          if(score > highscore){
            highscore=score;
            besta=a; bestb=b;
            printf("highscore is: %d using n^2 + %d n + %d --> a=%d, b=%d\n",highscore,besta,bestb,besta,bestb);
          }
          valid_seq=0;
          score=0;
          n=0;
        }
        else{
          valid_seq=1;
          score++;
          n++;
        }
      }
    }
  }
  finalanswer = (besta * bestb);
  printf("product is: %d\n",finalanswer);
  return 0;
}
