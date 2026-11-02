/* Project Euler 50: Consecutive Prime Sum.
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
  unsigned char *p = sieve(1000000);
  if (!p)
    return -1;
  unsigned count = 0;
  uint64_t prefix[80001] = {0};
  for (unsigned n = 2; n < 1000000; n++)
    if (p[n]) {
      prefix[count + 1] = prefix[count] + n;
      count++;
    }
  unsigned best_len = 0, best_prime = 0;
  for (unsigned i = 0; i < count; i++) {
    for (unsigned j = i + best_len + 1; j <= count; j++) {
      uint64_t sum = prefix[j] - prefix[i];
      if (sum >= 1000000)
        break;
      if (p[sum]) {
        best_len = j - i;
        best_prime = (unsigned)sum;
      }
    }
  }
  free(p);
  return best_prime;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
