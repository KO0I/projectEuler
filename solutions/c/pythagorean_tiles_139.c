/* Project Euler 139: https://projecteuler.net/problem=139 */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 100000000 };
    uint64_t count = 0;
    for (unsigned m = 2; 2ULL * m * (m + 1) < LIMIT; ++m) {
        for (unsigned n = 1; n < m; ++n) {
            if (((m - n) & 1U) == 0 || euler_gcd(m, n) != 1) continue;
            uint64_t a = (uint64_t)m * m - (uint64_t)n * n;
            uint64_t b = 2ULL * m * n;
            uint64_t c = (uint64_t)m * m + (uint64_t)n * n;
            uint64_t difference = a > b ? a - b : b - a;
            if (c % difference == 0) count += (LIMIT - 1) / (a + b + c);
        }
    }
    printf("%" PRIu64 "\n", count);
    return 0;
}
