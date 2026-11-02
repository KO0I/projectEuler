/* Project Euler 40: Champernowne's Constant.
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
  unsigned targets[] = {1, 10, 100, 1000, 10000, 100000, 1000000};
  int pos = 0;
  unsigned digit_pos = 0;
  long long product = 1;
  char buf[24];
  for (unsigned n = 1; pos < 7; n++) {
    sprintf(buf, "%u", n);
    for (char *p = buf; *p; p++) {
      digit_pos++;
      if (digit_pos == targets[pos]) {
        product *= *p - '0';
        pos++;
        if (pos == 7)
          break;
      }
    }
  }
  return product;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
