/* Project Euler 47: Distinct Primes Factors.
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
  unsigned streak = 0;
  for (unsigned n = 2; n < 1000000; n++) {
    if (distinct_factors(n) == 4) {
      if (++streak == 4)
        return n - 3;
    } else
      streak = 0;
  }
  return -1;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
