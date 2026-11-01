/* Project Euler 116: https://projecteuler.net/problem=116
 * For one colour, the last tile has length one or that colour's length.
 * Subtract the all-black arrangement before adding the three colours. */
#include "euler_extra.h"
static uint64_t coloured_ways(unsigned length, unsigned tile) {
    uint64_t ways[51] = {1};
    for (unsigned n = 1; n <= length; ++n)
        ways[n] = ways[n - 1] + (n >= tile ? ways[n - tile] : 0);
    return ways[length] - 1;
}
int main(void) {
    uint64_t total = 0;
    for (unsigned tile = 2; tile <= 4; ++tile) total += coloured_ways(50, tile);
    printf("%" PRIu64 "\n", total);
    return 0;
}
