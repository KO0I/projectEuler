/* Project Euler 92: Square Digit Chains.
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
  unsigned char cache[568] = {0};
  cache[1] = 1;
  cache[89] = 89;
  const unsigned sq[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};
  unsigned answer = 0;
  for (unsigned n = 1; n < 10000000; n++) {
    unsigned x = digit_power_sum(n, sq);
    while (!cache[x])
      x = digit_power_sum(x, sq);
    if (cache[x] == 89)
      answer++;
  }
  return answer;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
