/* Project Euler 190: https://projecteuler.net/problem=190

   Lagrange multipliers applied to product x_i^i with sum x_i=m give
   x_i=2i/(m+1).  Long double has ample separation from the relevant integer
   boundaries; multiplying the nineteen terms directly yields floor(P_m). */
#include "euler_extra.h"
int main(void) {
    uint64_t answer = 0;
    for (unsigned m = 2; m <= 15; ++m) {
        long double product = 1;
        for (unsigned i = 1; i <= m; ++i)
            product *= powl(2.0L * i / (m + 1), i);
        answer += (uint64_t)floorl(product);
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
