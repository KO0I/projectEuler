#include <stdio.h>
#include <stdlib.h>

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

int main(){
//first, create an array which is comprised of the max number of primes
//required for this problem:

//create the array j in size, which corresponds to the number of primes to
//be tested
int i,j=10,k=0,l,test,score=1,highscore=0;
int p[j];
int somePrime;
FILE *primes;
primes = fopen("million_primes","rt");
if(primes==NULL){
  printf("Bad file!\n");
  exit(0);
}


//read in primes from million_primes file as an array:
for (i=0; i<j; i++){
  fscanf(primes, "%d", &somePrime);
  p[i]=somePrime;
  printf("i:%d, p:%d\n",i,p[i]);
}
  //so now we have a lot of primes to begin working with, it's time to 
  //begin summing them in consecutive order, and checking primality on each
  //step with isPrime(), which is in the functions directory.
  //The algorithm will be:
for(k=0;k<j;k++){
  score=1;
  l=k+1;
  test=p[k]+p[l];
//  printf("\nscore:%d  l:%d  k:%d test:%d \n",score,l,k,test);
  
}

fclose(primes);
return 0;
}
