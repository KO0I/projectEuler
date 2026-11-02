/* https://projecteuler.net/problem=70
 * Sieve every totient below ten million, so the minimum does not depend on a
 * semiprime conjecture. Digit counts establish permutations; cross products
 * compare n/phi(n) exactly without floating-point rounding. */
#include "euler_extra.h"

static uint64_t p70_digits(uint32_t n) {
    uint64_t key = 0;
    do { key += UINT64_C(1) << (4 * (n % 10)); n /= 10; } while (n);
    return key;
}

int main(void) {
    const uint32_t limit = 10000000;
    uint32_t *phi = euler_malloc((size_t)limit * sizeof *phi);
    for (uint32_t n = 0; n < limit; ++n) phi[n] = n;
    for (uint32_t p = 2; p < limit; ++p) if (phi[p] == p)
        for (uint32_t n = p; n < limit; n += p) phi[n] -= phi[n] / p;
    uint32_t best = 0, best_phi = 1;
    for (uint32_t n = 2; n < limit; ++n) {
        if (p70_digits(n) != p70_digits(phi[n])) continue;
        if (!best || (uint64_t)n * best_phi < (uint64_t)best * phi[n]) {
            best = n; best_phi = phi[n];
        }
    }
    printf("%" PRIu32 "\n", best);
    free(phi);
    return 0;
}
