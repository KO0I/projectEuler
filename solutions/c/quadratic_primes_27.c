/* Project Euler 27: Quadratic Primes.
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
  int best = 0, result = 0;
  for (int a = -999; a < 1000; a++)
    for (int b = -999; b < 1000; b++) {
      int n = 0;
      while (1) {
        long long v = (long long)n * n + (long long)a * n + b;
        if (v < 2 || !prime((uint64_t)v))
          break;
        n++;
      }
      if (n > best) {
        best = n;
        result = a * b;
      }
    }
  return result;
}

int main(void) {
  long long answer = solve();
  printf("%lld\n", answer);
  return 0;
}
