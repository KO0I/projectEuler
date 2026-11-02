/* Project Euler 35: Circular Primes.
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
  unsigned char *p = sieve(1000000);
  if (!p)
    return -1;
  unsigned count = 0;
  for (unsigned n = 2; n < 1000000; n++)
    if (p[n]) {
      unsigned power = 1;
      while (power <= n / 10)
        power *= 10;
      unsigned rotated = n;
      int good = 1;
      do {
        rotated = rotated / 10 + (rotated % 10) * power;
        if (!p[rotated]) {
          good = 0;
          break;
        }
      } while (rotated != n);
      count += good;
    }
  free(p);
  return count;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
