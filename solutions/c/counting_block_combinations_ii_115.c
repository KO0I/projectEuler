/* Project Euler 115: https://projecteuler.net/problem=115
 * The recurrence is the same separated-red-run DP as #114. Twenty
 * disjoint blocks of 51 cells already allow 2^20 choices, proving that
 * a row of 1020 cells exceeds one million. Counts saturate above target. */
#include "euler_extra.h"
int main(void) {
    enum { MINIMUM = 50, LIMIT = 20 * (MINIMUM + 1) };
    const uint64_t target = 1000000;
    uint64_t ways[LIMIT + 1] = {1};
    for (unsigned n = 1; n <= LIMIT; ++n) {
        ways[n] = ways[n - 1];
        for (unsigned red = MINIMUM; red <= n; ++red) {
            ways[n] += red == n ? 1 : ways[n - red - 1];
            if (ways[n] > target) { ways[n] = target + 1; break; }
        }
        if (ways[n] > target) { printf("%u\n", n); return 0; }
    }
    euler_fail("Proven block-count bound was not reached");
    return 1;
}
