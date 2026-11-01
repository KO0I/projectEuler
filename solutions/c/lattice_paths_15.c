/* Project Euler 15: https://projecteuler.net/problem=15
 * A vertex receives every path arriving from above or from its left.
 * Updating one row in place counts all right/down paths to (20,20). */
#include "euler_extra.h"

int main(void) {
    uint64_t paths[21];
    for (unsigned x = 0; x <= 20; ++x) paths[x] = 1;
    for (unsigned y = 1; y <= 20; ++y)
        for (unsigned x = 1; x <= 20; ++x) paths[x] += paths[x - 1];
    printf("%" PRIu64 "\n", paths[20]);
    return 0;
}
