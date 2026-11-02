/* Project Euler 164: Three Consecutive Digital Sum Limit
 *
 * Whether a new digit is legal depends only on the previous two digits, so a
 * 10x10 table is the complete DP state.  `dp[a][b]` counts prefixes ending in
 * a,b; appending d is allowed exactly when a+b+d<=9 and moves to (b,d).
 *
 * Initialization uses a synthetic leading zero followed by a nonzero first
 * digit, thereby excluding leading-zero 20-digit numbers without a special
 * transition.  After nineteen extensions the table represents length 20, and
 * summing all terminal pairs produces the answer in uint64_t. */
#include "euler_extra.h"
int main(void) {
    uint64_t dp[10][10] = {{0}}, next[10][10];
    for (unsigned d = 1; d <= 9; ++d) dp[0][d] = 1;
    for (unsigned pos = 1; pos < 20; ++pos) {
        memset(next, 0, sizeof(next));
        for (unsigned a = 0; a < 10; ++a)
            for (unsigned b = 0; b < 10; ++b)
                for (unsigned d = 0; a + b + d <= 9; ++d)
                    next[b][d] += dp[a][b];
        memcpy(dp, next, sizeof(dp));
    }
    uint64_t total = 0;
    for (unsigned a = 0; a < 10; ++a)
        for (unsigned b = 0; b < 10; ++b) total += dp[a][b];
    printf("%" PRIu64 "\n", total);
    return 0;
}
