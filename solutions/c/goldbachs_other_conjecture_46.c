/* Project Euler 46: Goldbach's Other Conjecture.
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
  for (unsigned n = 9;; n += 2)
    if (!prime(n)) {
      int found = 0;
      for (unsigned p = 2; p < n; p++)
        if (prime(p)) {
          unsigned half = (n - p) / 2;
          if ((n - p) % 2)
            continue;
          unsigned root = (unsigned)sqrt((double)half);
          if (root * root == half) {
            found = 1;
            break;
          }
        }
      if (!found)
        return n;
    }
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
