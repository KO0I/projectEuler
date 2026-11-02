/* https://projecteuler.net/problem=64
 * The continued-fraction state (m,d,a) is entirely integral. A nonsquare
 * sqrt(N) completes its period when a reaches twice its integer part. */
#include "euler_extra.h"

static unsigned p64_period(unsigned n) {
    unsigned root = (unsigned)euler_isqrt(n);
    if (root * root == n) return 0;
    unsigned m = 0, d = 1, a = root, period = 0;
    do {
        m = d * a - m;
        d = (n - m * m) / d;
        a = (root + m) / d;
        ++period;
    } while (a != 2 * root);
    return period;
}

int main(void) {
    unsigned count = 0;
    for (unsigned n = 2; n <= 10000; ++n) count += p64_period(n) % 2;
    printf("%u\n", count);
    return 0;
}
