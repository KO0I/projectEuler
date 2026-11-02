/* Project Euler 25: 1000-digit Fibonacci Number.
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
  mpz_t a, b, limit, tmp;
  mpz_inits(a, b, limit, tmp, NULL);
  mpz_set_ui(a, 1);
  mpz_set_ui(b, 1);
  mpz_ui_pow_ui(limit, 10, 999);
  unsigned i = 2;
  while (mpz_cmp(b, limit) < 0) {
    mpz_add(tmp, a, b);
    mpz_set(a, b);
    mpz_set(b, tmp);
    i++;
  }
  mpz_clears(a, b, limit, tmp, NULL);
  return i;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
