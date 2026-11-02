/* Project Euler 11: Largest Product in a Grid.
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
  FILE *f = fopen("tests/data/p011.txt", "r");
  if (!f)
    return -1;
  int grid[20][20];
  for (int y = 0; y < 20; y++)
    for (int x = 0; x < 20; x++)
      if (fscanf(f, "%d", &grid[y][x]) != 1) {
        fclose(f);
        return -1;
      }
  fclose(f);
  long long best = 0;
  const int dx[] = {1, 0, 1, -1}, dy[] = {0, 1, 1, 1};
  for (int y = 0; y < 20; y++)
    for (int x = 0; x < 20; x++)
      for (int d = 0; d < 4; d++) {
        int xx = x + 3 * dx[d], yy = y + 3 * dy[d];
        if (xx < 0 || xx >= 20 || yy < 0 || yy >= 20)
          continue;
        long long product = 1;
        for (int k = 0; k < 4; k++)
          product *= grid[y + k * dy[d]][x + k * dx[d]];
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
