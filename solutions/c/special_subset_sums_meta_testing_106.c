/* Project Euler 106: https://projecteuler.net/problem=106
 * For 2k selected elements there are C(2k,k)/2 unordered equal-size
 * partitions. The Catalan number counts those already ordered term by
 * term; only the remaining partitions require a sum comparison. */
#include "euler_extra.h"

static uint64_t choose(unsigned n, unsigned k) {
    uint64_t result = 1;
    for (unsigned i = 1; i <= k; ++i) result = result * (n - k + i) / i;
    return result;
}
static uint64_t comparisons(unsigned n) {
    uint64_t total = 0;
    for (unsigned k = 2; 2 * k <= n; ++k) {
        uint64_t middle = choose(2 * k, k);
        total += choose(n, 2 * k) * (middle / 2 - middle / (k + 1));
    }
    return total;
}
int main(void) {
    printf("%" PRIu64 "\n", comparisons(12));
    return 0;
}
