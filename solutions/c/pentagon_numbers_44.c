/* Project Euler 44: Pentagon Numbers.
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
  long long best = 0;
  for (uint64_t j = 2; j < 5000; j++) {
    uint64_t pj = j * (3 * j - 1) / 2;
    for (uint64_t k = j - 1; k > 0; k--) {
      uint64_t pk = k * (3 * k - 1) / 2, diff = pj - pk;
      if (best && (long long)diff >= best)
        break;
      if (pentagonal(diff) && pentagonal(pj + pk))
        best = (long long)diff;
    }
    if (best && 3 * j - 2 > (uint64_t)best)
      break;
  }
  return best ? best : -1;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
