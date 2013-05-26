#include<stdio.h>
#include<math.h>

    /* Find out the prime factors
     * of a given number and print
     * them on the screen */
void factorize(unsigned long n){
  unsigned long d = 2;
  if(n < 2) return 1;
  printf("Largest prime factor of '%lu': ", n);
  /* while the factor being tested
  is lower than the number to factorize */
    while(d < n) {
    /* if valid prime factor */
      if(n % d == 0) {
        n /= d;
    }
  /* else: invalid prime factor */
  else {
  if(d == 2) d = 3;
  else d += 2;
    }
  }
  /* print  largest prime factor */
  printf("%lu\n", d);
}

