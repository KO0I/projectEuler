/* Project Euler 114: https://projecteuler.net/problem=114
 * Either begin with one black square, or a red run of length at least
 * three, followed by a mandatory black separator unless the row ends. */
#include "euler_extra.h"
static uint64_t block_ways(unsigned length, unsigned minimum) {
    uint64_t *ways = euler_calloc(length + 1, sizeof *ways);
    ways[0] = 1;
    for (unsigned n = 1; n <= length; ++n) {
        ways[n] = ways[n - 1];
        for (unsigned red = minimum; red <= n; ++red)
            ways[n] += red == n ? 1 : ways[n - red - 1];
    }
    uint64_t answer = ways[length];
    free(ways);
    return answer;
}
int main(void) {
    printf("%" PRIu64 "\n", block_ways(50, 3));
    return 0;
}
