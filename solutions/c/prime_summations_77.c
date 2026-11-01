/* https://projecteuler.net/problem=77
 * Unordered prime partitions use the coin-change recurrence. Double the search
 * bound until the first count exceeds 5000; saturating at 5001 is sufficient
 * for this comparison and prevents unnecessary growth in the counts. */
#include "euler_extra.h"

int main(void) {
    for (unsigned limit = 16;; limit *= 2) {
        unsigned char *prime = euler_sieve(limit);
        unsigned *ways = euler_calloc((size_t)limit + 1, sizeof *ways);
        ways[0] = 1;
        for (unsigned p = 2; p <= limit; ++p) if (prime[p])
            for (unsigned n = p; n <= limit; ++n) {
                unsigned sum = ways[n] + ways[n - p];
                ways[n] = sum > 5000 ? 5001 : sum;
            }
        unsigned answer = 0;
        for (unsigned n = 2; n <= limit; ++n) if (ways[n] > 5000) { answer = n; break; }
        free(ways); free(prime);
        if (answer) { printf("%u\n", answer); return 0; }
        if (limit > UINT_MAX / 2) euler_fail("Prime partition search range exceeded");
    }
}
