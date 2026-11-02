/* Project Euler 193: https://projecteuler.net/problem=193

   Inclusion-exclusion over square divisors is exactly the Möbius identity
     Q(N) = sum_{d<=sqrt(N)} mu(d) floor(N/d^2).
   A prime sieve constructs mu: each prime flips multiples once and zeros
   multiples of its square.  N is 2^50-1 because the bound is strict. */
#include "euler_extra.h"
int main(void) {
    const uint64_t limit = (1ULL << 50) - 1;
    const unsigned root = 1u << 25;
    int8_t *mobius = malloc((root + 1) * sizeof(*mobius));
    bool *composite = calloc(root + 1, 1);
    if (!mobius || !composite) return EXIT_FAILURE;
    memset(mobius, 1, root + 1);
    for (unsigned prime = 2; prime <= root; ++prime) {
        if (composite[prime]) continue;
        for (unsigned multiple = prime; multiple <= root; multiple += prime) {
            composite[multiple] = multiple != prime ? composite[multiple] : false;
            mobius[multiple] = -mobius[multiple];
        }
        uint64_t square = (uint64_t)prime * prime;
        if (square <= root)
            for (uint64_t multiple = square; multiple <= root; multiple += square) mobius[multiple] = 0;
        if (square <= root)
            for (uint64_t multiple = square; multiple <= root; multiple += prime) composite[multiple] = true;
    }
    int64_t answer = 0;
    for (uint64_t d = 1; d <= root; ++d)
        answer += (int64_t)mobius[d] * (int64_t)(limit / (d * d));
    printf("%" PRId64 "\n", answer);
    free(mobius); free(composite);
    return 0;
}
