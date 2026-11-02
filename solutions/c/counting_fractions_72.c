/* https://projecteuler.net/problem=72
 * Exactly phi(d) reduced proper fractions have denominator d. An Eratosthenes
 * totient sieve computes all of them before their 64-bit sum is accumulated. */
#include "euler_extra.h"

static uint64_t p72_count(unsigned limit) {
    unsigned *phi = euler_malloc(((size_t)limit + 1) * sizeof *phi);
    for (unsigned n = 0; n <= limit; ++n) phi[n] = n;
    for (unsigned p = 2; p <= limit; ++p) if (phi[p] == p)
        for (unsigned n = p; n <= limit; n += p) phi[n] -= phi[n] / p;
    uint64_t count = 0;
    for (unsigned n = 2; n <= limit; ++n) count += phi[n];
    free(phi);
    return count;
}

int main(void) {
    printf("%" PRIu64 "\n", p72_count(1000000));
    return 0;
}
