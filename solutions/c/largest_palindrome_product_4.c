/* Project Euler 4: Largest Palindrome Product.
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
  int best = 0;
  for (int a = 100; a <= 999; a++)
    for (int b = a; b <= 999; b++) {
      int product = a * b;
      if (product > best && palindrome(product))
        best = product;
    }
  return best;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
