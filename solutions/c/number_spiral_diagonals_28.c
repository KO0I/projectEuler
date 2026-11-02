/* Project Euler 28: Number Spiral Diagonals.
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
  long long sum = 1;
  for (long long side = 3; side <= 1001; side += 2)
    sum += 4 * side * side - 6 * (side - 1);
  return sum;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
