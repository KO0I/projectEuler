/* Project Euler 48: Self Powers.
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
  mpz_t sum, base, term, mod;
  mpz_inits(sum, base, term, mod, NULL);
  mpz_ui_pow_ui(mod, 10, 10);
  for (unsigned n = 1; n <= 1000; n++) {
    mpz_set_ui(base, n);
    mpz_powm_ui(term, base, n, mod);
    mpz_add(sum, sum, term);
    mpz_mod(sum, sum, mod);
  }
  /* sum < 10^10. Decimal conversion avoids unsigned-long truncation
   * on LLP64 targets such as 64-bit Windows. */
  char decimal[32];
  mpz_get_str(decimal, 10, sum);
  long long result = strtoll(decimal, NULL, 10);
  mpz_clears(sum, base, term, mod, NULL);
  return result;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
