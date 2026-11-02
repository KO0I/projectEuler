/* Project Euler 19: Counting Sundays.
 * This file owns the problem algorithm. */
#include "euler_common.h"
#include <ctype.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int leap(int year) {
  return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static long long solve(void) {
  int w = 1, total = 0;
  static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (int y = 1900; y <= 2000; y++)
    for (int m = 0; m < 12; m++) {
      if (y >= 1901 && w == 0)
        total++;
      w = (w + days[m] + (m == 1 && leap(y))) % 7;
    }
  return total;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
