/* Project Euler 39: Integer Right Triangles.
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
  int best = 0, best_p = 0;
  for (int p = 12; p <= 1000; p++) {
    int count = 0;
    for (int a = 1; a < p / 3; a++)
      for (int b = a + 1; b < (p - a) / 2; b++) {
        int c = p - a - b;
        if (a * a + b * b == c * c)
          count++;
      }
    if (count > best) {
      best = count;
      best_p = p;
    }
  }
  return best_p;
}

int main(void) {
  long long answer = solve();
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
