/* Project Euler 17: Number Letter Counts.
 * This file owns the problem algorithm. */
#include "euler_common.h"
#include <ctype.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int number_letters(unsigned n) {
  static const int small[] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4,
                              3, 6, 6, 8, 8, 7, 7, 9, 8, 8};
  static const int tens[] = {0, 0, 6, 6, 5, 5, 5, 7, 6, 6};
  if (n == 1000)
    return 11;
  int length = 0;
  if (n >= 100) {
    length += small[n / 100] + 7;
    n %= 100;
    if (n)
      length += 3;
  }
  if (n >= 20) {
    length += tens[n / 10];
    n %= 10;
  }
  if (n)
    length += small[n];
  return length;
}

static long long solve(void) {
  unsigned sum = 0;
  for (unsigned n = 1; n <= 1000; n++)
    sum += number_letters(n);
  return sum;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
