/* Project Euler 55: Lychrel Numbers.
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
  mpz_t n, rev;
  mpz_inits(n, rev, NULL);
  unsigned total = 0;
  for (unsigned start = 1; start < 10000; start++) {
    mpz_set_ui(n, start);
    int reached = 0;
    for (int i = 0; i < 50; i++) {
      char *str = mpz_get_str(NULL, 10, n);
      size_t len = strlen(str);
      for (size_t l = 0; l < len / 2; l++) {
        char t = str[l];
        str[l] = str[len - 1 - l];
        str[len - 1 - l] = t;
      }
      mpz_set_str(rev, str, 10);
      free(str);
      mpz_add(n, n, rev);
      str = mpz_get_str(NULL, 10, n);
      len = strlen(str);
      reached = 1;
      for (size_t l = 0; l < len / 2; l++)
        if (str[l] != str[len - 1 - l]) {
          reached = 0;
          break;
        }
      free(str);
      if (reached)
        break;
    }
    if (!reached)
      total++;
  }
  mpz_clears(n, rev, NULL);
  return total;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
