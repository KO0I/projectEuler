/* Project Euler 2: Even Fibonacci Numbers.
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
  long long a = 1, b = 2, s = 0;
  while (b <= 4000000) {
    if (!(b % 2))
      s += b;
    long long t = a + b;
    a = b;
    b = t;
  }
  return s;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
