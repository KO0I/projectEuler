/* Project Euler 113: https://projecteuler.net/problem=113
 * Digit DP counts monotone positive integers at each length. Nine
 * constant-digit integers occur in both directions at every length. */
#include "euler_extra.h"
static uint64_t non_bouncy(unsigned digits) {
    uint64_t up[10] = {0}, down[10] = {0}, total = 0;
    for (unsigned d = 1; d <= 9; ++d) up[d] = down[d] = 1;
    for (unsigned length = 1; length <= digits; ++length) {
        for (unsigned d = 0; d <= 9; ++d) total += up[d] + down[d];
        total -= 9;
        uint64_t next_up[10] = {0}, next_down[10] = {0};
        for (unsigned previous = 0; previous <= 9; ++previous) {
            for (unsigned d = previous; d <= 9; ++d) next_up[d] += up[previous];
            for (unsigned d = 0; d <= previous; ++d) next_down[d] += down[previous];
        }
        memcpy(up, next_up, sizeof up); memcpy(down, next_down, sizeof down);
    }
    return total;
}
int main(void) {
    printf("%" PRIu64 "\n", non_bouncy(100));
    return 0;
}
