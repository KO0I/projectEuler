#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#define NUMT unsigned int
#define DEFAULTPRECISION 3
#define PRECISIONFACTOR   3
short precision=0;
unsigned long precisioncheck=10;

unsigned int fastroot(unsigned int input){
  unsigned int a,b,x,i;
  x=input;
  b=x;
  a=x=0x3f;
  // this loop will run more times as the
  // precision variable increases. This is to deal
  // with the fact that given a large number and only a few
  // iterations, the fastroot returns a value larger than the
  // actual sqrt
  for(i=0; i<(DEFAULTPRECISION+precision); i++){
  x=b/x;
  a=x=(x+a)>>1;
  }
  x=b/x;
  x=(x+a)>>1;
  //printf("sqrt(%u) = %u\n",input,x);
  return((x+1));
}

int main(void){
  NUMT countDone = 1000000; //this is the number of primes the program will search for
  NUMT count = 0; 
  NUMT prime[countDone]; 
  NUMT sq,i;
  //for problem 10, where all primes below 2 million are summed together, the
  //final prime is 1999993 and it is the 148933th prime
  //for problem 35, we only consider primes less than 1 million - so only up
  //to the 78498th prime is relevant.

  //set up a timer to gauge the efficiency of the program
  clock_t begin, end;
  double time_spent;
  begin=clock();

  //set the first prime to 2, and the second prime to 3
  if(count == 0){
    prime[0]=2;
    prime[1]=3;
    count=2;
  }

  NUMT candidate=5;                        //first candidate number is set
  assert(count>=2);
  for(count=2; count<=countDone; count++){ //this loop finds "countDone" number of primes
    //double dcan = (double) candidate;      //(double) candidate;
    //NUMT sq = (NUMT) ((sqrt(dcan))+0.5);   //sqrt of the candidate number
    sq = fastroot(candidate);                //sqrt using a faster algorithm
    for(i=2; i<=sq; i++){
      if(candidate % i == 0){
        candidate+=2;
        i=2;
      }
    }
    //prime number is now added to prime[count]
    prime[count]=candidate;
    candidate+=2;
    if(candidate>precisioncheck){
      precisioncheck*=PRECISIONFACTOR;
      precision++;
      //printf("precision is increasing to %d\n",precision);
    }
  }
  // stop the timer, and print out how long it took to find the primes
  end=clock();
  time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
  printf("Time spent finding %ld primes %f s\n",countDone, time_spent);

  //prints our prime list to a file named "million primes"
  //begin=clock();
  const char filename[] = "prime_test";
  FILE *fp = fopen(filename,"w");

  if (fp==NULL){
    printf("failed to open file for write\n");
    return -1;
  }
  NUMT j;
  for(j=0; j<=countDone; ++j){
    fprintf(fp, "%lu", prime[j]);
    fprintf(fp, "\n");
  }
  fclose(fp);
  //end=clock();
  //time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
  //printf("Time spent writing primes to a file: %f s\n",time_spent);
  return 0;
}
