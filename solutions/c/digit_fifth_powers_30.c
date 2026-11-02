/* Project Euler 30: Digit Fifth Powers.
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
  unsigned pow5[10];
  for (unsigned n = 0; n < 10; n++) {
    pow5[n] = 1;
    for (int k = 0; k < 5; k++)
      pow5[n] *= n;
  }
  unsigned sum = 0;
  for (unsigned n = 2; n <= 354294; n++)
    if (digit_power_sum(n, pow5) == n)
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
