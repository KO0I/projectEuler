/* Project Euler 14: Longest Collatz Sequence.
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
  unsigned best_n = 1, best_len = 0;
  for (unsigned start = 1; start < 1000000; start++) {
    uint64_t n = start;
    unsigned len = 1;
    while (n != 1) {
      n = (n % 2) ? 3 * n + 1 : n / 2;
      len++;
    }
    if (len > best_len) {
      best_len = len;
      best_n = start;
    }
  }
  return best_n;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
