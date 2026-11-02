/* https://projecteuler.net/problem=88
 * Enumerate nondecreasing factors >=2. Padding with ones gives
 * k=product-sum+factor_count. Every k has a representation at 2*k, using
 * factors 2,k and k-2 ones, so products above 2*K cannot improve any minimum.
 * Adding factors never decreases k, which supplies another safe pruning rule. */
#include "euler_extra.h"

static void p88_search(unsigned product, unsigned sum, unsigned count, unsigned first,
                       unsigned limit, unsigned *minimum) {
    unsigned k = product - sum + count;
    if (k > limit) return;
    if (k >= 2 && product < minimum[k]) minimum[k] = product;
    for (unsigned factor = first; factor <= 2 * limit / product; ++factor)
        p88_search(product * factor, sum + factor, count + 1, factor, limit, minimum);
}

static uint64_t p88_sum(unsigned limit) {
    unsigned *minimum = euler_malloc(((size_t)limit + 1) * sizeof *minimum);
    for (unsigned k = 0; k <= limit; ++k) minimum[k] = 2 * limit;
    p88_search(1, 0, 0, 2, limit, minimum);
    bool *seen = euler_calloc(2 * (size_t)limit + 1, sizeof *seen);
    uint64_t sum = 0;
    for (unsigned k = 2; k <= limit; ++k) if (!seen[minimum[k]]) {
        seen[minimum[k]] = true; sum += minimum[k];
    }
    free(seen); free(minimum);
    return sum;
}

int main(void) {
    printf("%" PRIu64 "\n", p88_sum(12000));
    return 0;
}
