/* Project Euler 34: Digit Factorials.
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
  unsigned f[10];
  for (int n = 0; n < 10; n++)
    f[n] = factorial(n);
  unsigned sum = 0;
  for (unsigned n = 3; n <= 2540160; n++)
    if (digit_power_sum(n, f) == n)
      sum += n;
  return sum;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
