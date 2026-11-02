/* Project Euler 12: Highly Divisible Triangular Number.
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
  for (uint64_t n = 1;; n++)
    if (count_divisors(n * (n + 1) / 2) > 500)
      return (long long)(n * (n + 1) / 2);
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
