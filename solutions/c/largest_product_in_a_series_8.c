/* Project Euler 8: Largest Product in a Series.
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
  FILE *f = fopen("tests/data/p008.txt", "r");
  if (!f)
    return -1;
  char digits[1024];
  unsigned n = 0;
  int ch;
  while ((ch = fgetc(f)) != EOF)
    if (ch >= '0' && ch <= '9') {
      if (n >= sizeof digits) {
        fclose(f);
        return -1;
      }
      digits[n++] = (char)ch;
    }
  fclose(f);
  if (n != 1000)
    return -1;
  long long best = 0;
  for (unsigned i = 0; i + 13 <= n; i++) {
    long long product = 1;
    for (unsigned j = 0; j < 13; j++)
      product *= digits[i + j] - '0';
    if (product > best)
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
