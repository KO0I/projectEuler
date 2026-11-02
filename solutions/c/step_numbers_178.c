/* Project Euler 178: Step Numbers
 *
 * A state records the final digit and a ten-bit mask of digits already seen.
 * The next digit of a step number must be final-1 or final+1; OR-ing that digit
 * into the mask preserves exactly the information needed to recognize a
 * pandigital prefix.  Initial states use digits 1..9, excluding leading zero.
 *
 * At each length from 10 through 40, states with mask 1023 are accumulated.
 * Two rolling 10x1024 tables avoid a length dimension, and the total fits in
 * uint64_t. */
#include "euler_extra.h"
int main(void) {
    uint64_t dp[10][1024] = {{0}}, next[10][1024], answer = 0;
    for (unsigned d = 1; d <= 9; ++d) dp[d][1u << d] = 1;
    for (unsigned len = 1; len <= 40; ++len) {
        if (len >= 10)
            for (unsigned d = 0; d < 10; ++d) answer += dp[d][1023];
        memset(next, 0, sizeof(next));
        for (unsigned d = 0; d < 10; ++d) {
            for (unsigned mask = 0; mask < 1024; ++mask) {
                if (d) next[d - 1][mask | (1u << (d - 1))] += dp[d][mask];
                if (d < 9) next[d + 1][mask | (1u << (d + 1))] += dp[d][mask];
            }
        }
        memcpy(dp, next, sizeof(dp));
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
