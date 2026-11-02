/* Project Euler 10: Summation of Primes.
 * This file owns the problem algorithm. */
#include "euler_common.h"
#include <ctype.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long solve(void) {
  unsigned char *p = sieve(2000000);
  if (!p)
    return -1;
  long long sum = 0;
  for (int n = 2; n < 2000000; n++)
    if (p[n])
      sum += n;
  free(p);
  return sum;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
