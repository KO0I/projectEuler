/* Project Euler 120: https://projecteuler.net/problem=120
 * Binomial expansion gives remainder 2 for even n and 2*a*n mod a^2
 * for odd n. Its maximum is 2*a*floor((a-1)/2). */
#include "euler_extra.h"
static uint64_t maximum_remainder(unsigned a) {
    return 2ULL * a * ((a - 1) / 2);
}
int main(void) {
    uint64_t total = 0;
    for (unsigned a = 3; a <= 1000; ++a) total += maximum_remainder(a);
    printf("%" PRIu64 "\n", total);
    return 0;
}
