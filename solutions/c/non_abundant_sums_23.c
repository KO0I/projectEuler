/* Project Euler 23: https://projecteuler.net/problem=23
 * The statement supplies 28123 as an inclusive sufficient bound.
 * A divisor sieve identifies abundant numbers, then all their pair sums. */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 28123 };
    unsigned *divisors = euler_calloc(LIMIT + 1, sizeof *divisors);
    unsigned *abundant = euler_malloc((LIMIT + 1) * sizeof *abundant);
    unsigned char *represented = euler_calloc(LIMIT + 1, 1);
    for (unsigned d = 1; d <= LIMIT / 2; ++d)
        for (unsigned n = 2 * d; n <= LIMIT; n += d) divisors[n] += d;
    size_t count = 0;
    for (unsigned n = 1; n <= LIMIT; ++n)
        if (divisors[n] > n) abundant[count++] = n;
    for (size_t i = 0; i < count; ++i)
        for (size_t j = i; j < count; ++j) {
            unsigned sum = abundant[i] + abundant[j];
            if (sum > LIMIT) break;
            represented[sum] = 1;
        }
    uint64_t answer = 0;
    for (unsigned n = 1; n <= LIMIT; ++n)
        if (!represented[n]) answer += n;
    printf("%" PRIu64 "\n", answer);
    free(represented);
    free(abundant);
    free(divisors);
    return 0;
}
