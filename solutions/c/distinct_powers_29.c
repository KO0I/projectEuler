/* Project Euler 29: Distinct Powers.
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
  char *values[9801];
  size_t idx = 0;
  mpz_t v;
  mpz_init(v);
  for (unsigned a = 2; a <= 100; a++)
    for (unsigned b = 2; b <= 100; b++) {
      mpz_ui_pow_ui(v, a, b);
      values[idx++] = mpz_get_str(NULL, 10, v);
    }
  mpz_clear(v);
  qsort(values, idx, sizeof values[0], namecmp);
  long long count = 0;
  for (size_t i = 0; i < idx; i++)
    if (!i || strcmp(values[i], values[i - 1]) != 0)
      count++;
  for (size_t i = 0; i < idx; i++)
    free(values[i]);
  return count;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
