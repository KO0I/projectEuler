/* Project Euler 21: Amicable Numbers.
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
  unsigned sum = 0;
  for (unsigned n = 2; n < 10000; n++) {
    unsigned pair = divisor_sum(n);
    if (pair != n && pair > 0 && divisor_sum(pair) == n)
      sum += n;
  }
  return sum;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
