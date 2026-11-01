/* Project Euler 100: https://projecteuler.net/problem=100
 * t(t-1)=2*b(b-1) is a negative Pell equation. Its positive solutions
 * are generated in increasing t by the recurrence below. */
#include "euler_extra.h"
int main(void) {
    uint64_t blue = 1, total = 1;
    while (total <= UINT64_C(1000000000000)) {
        uint64_t next_blue = 3 * blue + 2 * total - 2;
        total = 4 * blue + 3 * total - 3;
        blue = next_blue;
    }
    printf("%" PRIu64 "\n", blue);
    return 0;
}
