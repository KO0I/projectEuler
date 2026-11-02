/* Project Euler 69: Totient Maximum.
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
  unsigned n = 1;
  for (unsigned p = 2;; p++)
    if (prime(p)) {
      if (n > 1000000 / p)
        break;
      n *= p;
    }
  return n;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
