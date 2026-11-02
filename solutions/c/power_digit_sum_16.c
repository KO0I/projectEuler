/* Project Euler 16: Power Digit Sum.
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
  mpz_t n;
  mpz_init(n);
  mpz_ui_pow_ui(n, 2, 1000);
  long long s = digit_sum(n);
  mpz_clear(n);
  return s;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
