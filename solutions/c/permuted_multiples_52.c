/* Project Euler 52: https://projecteuler.net/problem=52
 * For each digit length, n and 6n must stay in the same decimal interval.
 * Compare exact digit multiplicities for n,2n,...,6n. */
#include "euler_extra.h"

static uint64_t signature(uint64_t value) {
    uint64_t result = 0;
    do { result += UINT64_C(1) << (5 * (value % 10)); value /= 10; } while (value);
    return result;
}

int main(void) {
    uint64_t lower = 1, upper = 9;
    for (;;) {
        for (uint64_t n = lower; n <= upper / 6; ++n) {
            bool same = true;
            uint64_t original = signature(n);
            for (unsigned multiplier = 2; multiplier <= 6; ++multiplier)
                if (signature(n * multiplier) != original) { same = false; break; }
            if (same) { printf("%" PRIu64 "\n", n); return 0; }
        }
        lower = upper + 1;
        if (upper > (UINT64_MAX - 9) / 10) euler_fail("Permutation search overflow");
        upper = 10 * upper + 9;
    }
}
