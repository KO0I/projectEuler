/* https://projecteuler.net/problem=87
 * Enumerate prime fourth powers, cubes and squares, stopping each increasing
 * loop at fifty million. A bitset deduplicates sums reached in multiple ways. */
#include "euler_extra.h"

int main(void) {
    const uint64_t limit = 50000000;
    unsigned bound = (unsigned)euler_isqrt(limit - 1);
    unsigned char *prime = euler_sieve(bound);
    unsigned *list = euler_malloc(((size_t)bound + 1) * sizeof *list);
    size_t count = 0;
    for (unsigned n = 2; n <= bound; ++n) if (prime[n]) list[count++] = n;
    unsigned char *seen = euler_calloc((size_t)((limit + 7) / 8), 1);
    unsigned answer = 0;
    for (size_t i = 0; i < count; ++i) {
        uint64_t a = list[i], fourth = a * a * a * a;
        if (fourth >= limit) break;
        for (size_t j = 0; j < count; ++j) {
            uint64_t b = list[j], partial = fourth + b * b * b;
            if (partial >= limit) break;
            for (size_t k = 0; k < count; ++k) {
                uint64_t c = list[k], sum = partial + c * c;
                if (sum >= limit) break;
                unsigned char bit = (unsigned char)(1u << (sum % 8));
                if (!(seen[sum / 8] & bit)) { seen[sum / 8] |= bit; ++answer; }
            }
        }
    }
    printf("%u\n", answer);
    free(seen); free(list); free(prime);
    return 0;
}
