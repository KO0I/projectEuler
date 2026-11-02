/* Project Euler 22: Names Scores.
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
  FILE *f = fopen("names.txt", "r");
  if (!f)
    return -1;
  char *names[6000];
  size_t count = 0;
  int ch;
  while ((ch = fgetc(f)) != EOF) {
    if (ch != '"')
      continue;
    char buf[128];
    size_t len = 0;
    while ((ch = fgetc(f)) != EOF && ch != '"') {
      if (len + 1 >= sizeof buf) {
        fclose(f);
        return -1;
      }
      buf[len++] = (char)ch;
    }
    buf[len] = 0;
    if (count >= 6000) {
      fclose(f);
      return -1;
    }
    names[count] = malloc(len + 1);
    if (!names[count]) {
      fclose(f);
      return -1;
    }
    memcpy(names[count], buf, len + 1);
    count++;
  }
  fclose(f);
  if (count != 5163)
    return -1;
  qsort(names, count, sizeof names[0], namecmp);
  long long total = 0;
  for (size_t i = 0; i < count; i++) {
    unsigned score = 0;
    for (char *p = names[i]; *p; p++)
      if (*p >= 'A' && *p <= 'Z')
        score += *p - 'A' + 1;
    total += (long long)(i + 1) * score;
    free(names[i]);
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
