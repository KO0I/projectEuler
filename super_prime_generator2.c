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
  printf("%d\n",2); // print out 2, the first prime
  unsigned long countDone = 18;
  unsigned long count = 1;
  unsigned long n;
  double time_spent;
  clock_t begin, end;
//  gmp_printf("%Zu\n",candidate);

  mpz_t i;      mpz_init (i); // initialize incrementer
  mpz_t r;      mpz_init (r); // initialize modulo result
  mpz_t limit;  mpz_init (limit);
  // begin timer
  begin=clock();
  while(count<countDone){
      // this sequence only checks even numbers
      // first, check if the number is a perfect square
      // if it is not a perfect square, it may be prime
      for(n=2; n<9; n++){
        if(mpz_root(r, candidate,n)==1){
          //gmp_printf("%Zd was not prime\n",candidate);
          mpz_add_ui(candidate,candidate,2);
        }
      }
        mpz_sqrt(limit, candidate);
        mpz_add_ui(limit,limit,2);

        for(mpz_set_ui(i,2); mpz_cmp(i,limit)<0; mpz_add_ui(i,i,1)){
          mpz_mod(r, candidate, i);
          if(mpz_cmp_ui(r,0)==0){ // it is a composite number
            mpz_add_ui(candidate,candidate,2); // get a new candidate
            mpz_set_ui(i,3);                    // reset i
          }
        }
      // since the candidate survived the process, it's prime
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
  time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
  printf("%f\n",time_spent);
  return 0;
}
