/* Project Euler 3: Largest Prime Factor.
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
  uint64_t n = 600851475143ULL, p = 2, last = 1;
  while (p <= n / p) {
    if (n % p == 0) {
      last = p;
      n /= p;
    } else
      p += (p == 2 ? 1 : 2);
  }
  return (long long)(n > 1 ? n : last);
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
