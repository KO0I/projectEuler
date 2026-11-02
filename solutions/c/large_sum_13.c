/* Project Euler 13: Large Sum.
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
  FILE *f = fopen("lsum", "r");
  if (!f)
    return -1;
  mpz_t total, tmp;
  mpz_inits(total, tmp, NULL);
  char line[256];
  int count = 0;
  while (fgets(line, sizeof line, f))
    if (mpz_set_str(tmp, line, 10) == 0) {
      mpz_add(total, total, tmp);
      count++;
    }
  fclose(f);
  char *s = mpz_get_str(NULL, 10, total);
  long long out = -1;
  if (count == 100 && s && strlen(s) >= 10) {
    char prefix[11];
    memcpy(prefix, s, 10);
    prefix[10] = 0;
    out = atoll(prefix);
  }
  free(s);
  mpz_clears(total, tmp, NULL);
  return out;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
