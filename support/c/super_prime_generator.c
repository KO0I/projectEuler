#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
// Yet another prime number generator, except this baby will be using GMP
// the idea is to generate very large primes, which may be handy to use
// in future Project Euler problems

int main(void){
  mpz_t candidate;
  mpz_init (candidate);
  unsigned long start = 3; // 3 is the second prime
  mpz_set_ui(candidate, start); 
  printf("%d\n%d\n",2,3); // print out 2 and 3, the first prime
  unsigned long countDone = 1000000;
  unsigned long count = 1;
  double time_spent;
  clock_t begin, end;
  //  gmp_printf("%Zu\n",candidate);

  mpz_t i;      mpz_init (i); // initialize incrementer
  mpz_t r;      mpz_init (r); // initialize modulo result
  mpz_t limit;  mpz_init (limit);
  // begin timer
  begin=clock();
  while(count != countDone-1){
    if(mpz_odd_p(candidate)){ //don't bother unless the number is odd
      mpz_sqrt(limit, candidate);
      mpz_add_ui(limit,limit,3);
      //gmp_printf("%Zu\n",limit);  // the largest value to check

      for(mpz_set_ui(i,3); mpz_cmp(i,limit)<0; mpz_add_ui(i,i,1)){
        mpz_mod(r, candidate, i);
        if(mpz_cmp_ui(r,0)==0){ // it is a composite number
          mpz_add_ui(candidate,candidate,2); // get a new candidate
          mpz_set_ui(i,2);                    // reset i
        }
      }
      mpz_clear(i);
      mpz_clear(limit);
      mpz_clear(r);
    }
    // since the candidate survived the loop, print it and increment count
    // check every 10000 primes
    //if(count%10000==0) gmp_printf("%d, %Zd \n", count, candidate);
    gmp_printf("%Zd\n",candidate);
    mpz_add_ui(candidate,candidate,2);
    count++;
  }
  //gmp_printf("%Zd ", candidate);
  //printf("\n");
  //print time
  end=clock();
  //gmp_printf("%Zd\n",candidate);
  time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
  //printf("%f\n",time_spent);
  return 0;
}
