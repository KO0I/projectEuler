/* Project Euler 29: Distinct Powers.
 *
 * Count the distinct integers a^b for 2 <= a <= 100 and 2 <= b <= 100.
 * Different pairs can produce the same integer, such as 4^3 = 8^2 = 64.
 *
 * Compute every power exactly with GMP, store its decimal representation,
 * sort the strings, and count groups of equal strings. Floating-point pow()
 * would introduce rounding, and powers such as 100^100 exceed the range of
 * the built-in integer types.
 */
#include "euler_common.h"
#include <ctype.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long solve(void) {
  /* Each inclusive range contains 99 choices, so there are 99 * 99 = 9801
   * pairs. Each array entry will own one allocated, null-terminated string.
   * idx tracks how many entries have been filled. */
  char *values[9801];
  size_t idx = 0;

  /* mpz_t is GMP's arbitrary-precision integer type. Initialize one temporary
   * value and reuse it for every power, rather than keeping 9801 GMP objects. */
  mpz_t v;
  mpz_init(v);
  for (unsigned a = 2; a <= 100; a++)
    for (unsigned b = 2; b <= 100; b++) {
      /* Compute a^b with exact integer arithmetic. This replaces v's previous
       * value while allowing GMP to reuse its allocated storage. */
      mpz_ui_pow_ui(v, a, b);

      /* NULL asks GMP to allocate the output string; 10 selects decimal.
       * The string is an independent copy, so the next power can overwrite v.
       * Equal positive integers have identical decimal strings. */
      values[idx++] = mpz_get_str(NULL, 10, v);
    }

  /* The strings retain all results, so the temporary integer is no longer
   * needed. mpz_clear releases the storage GMP allocated for v. */
  mpz_clear(v);

  /* Sort the array of string pointers using the shared strcmp-based namecmp
   * comparator. Lexicographic order is sufficient: numeric order does not
   * matter, provided equal strings become adjacent. sizeof values[0] is the
   * size of one pointer, which is the array element size qsort requires. */
  qsort(values, idx, sizeof values[0], namecmp);

  /* The first string starts a group. Every later string starts a new group
   * only when it differs from its predecessor. The || operator short-circuits,
   * so values[i - 1] is never accessed when i is zero. */
  long long count = 0;
  for (size_t i = 0; i < idx; i++)
    if (!i || strcmp(values[i], values[i - 1]) != 0)
      count++;

  /* Free every allocated string, including duplicate strings. qsort moved
   * their pointers but did not free or combine any allocations. The values
   * array itself has automatic storage and needs no explicit deallocation. */
  for (size_t i = 0; i < idx; i++)
    free(values[i]);
  return count;
}

int main(void) {
  long long answer = solve();
  /* Follow the executable's convention that a negative solver result means
   * failure. Successful runs print just the answer and a trailing newline. */
  if (answer < 0)
    return 1;
  printf("%lld\n", answer);
  return 0;
}
