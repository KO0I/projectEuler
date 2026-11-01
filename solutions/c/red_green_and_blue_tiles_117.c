/* Project Euler 117: https://projecteuler.net/problem=117
 * Partition arrangements by their final tile, of length 1, 2, 3 or 4. */
#include "euler_extra.h"
static uint64_t mixed_ways(unsigned length) {
    uint64_t ways[51] = {1};
    for (unsigned n = 1; n <= length; ++n)
        for (unsigned tile = 1; tile <= 4 && tile <= n; ++tile)
            ways[n] += ways[n - tile];
    return ways[length];
}
int main(void) {
    printf("%" PRIu64 "\n", mixed_ways(50));
    return 0;
}
