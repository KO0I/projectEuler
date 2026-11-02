/* Project Euler 9: Special Pythagorean Triplet.
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
  for (int a = 1; a < 334; a++)
    for (int b = a + 1; b < 500; b++) {
      int c = 1000 - a - b;
      if (c > b && a * a + b * b == c * c)
        return a * b * c;
    }
  return -1;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
