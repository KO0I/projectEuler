/* https://projecteuler.net/problem=76
 * The coin-change recurrence counts unordered partitions. Excluding the part
 * 100 removes exactly the forbidden single-term representation of 100. */
#include "euler_extra.h"

int main(void) {
    uint64_t ways[101] = {1};
    for (unsigned part = 1; part < 100; ++part)
        for (unsigned sum = part; sum <= 100; ++sum) ways[sum] += ways[sum - part];
    printf("%" PRIu64 "\n", ways[100]);
    return 0;
}
