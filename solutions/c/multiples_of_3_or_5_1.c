/* Project Euler 1: Multiples of 3 or 5.
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
  long long s = 0;
  for (int n = 1; n < 1000; n++)
    if (n % 3 == 0 || n % 5 == 0)
      s += n;
  return s;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
