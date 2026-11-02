/* Project Euler 133: https://projecteuler.net/problem=133 */
#include "euler_extra.h"

int main(void) {
    uint64_t sum = 0;
    for (unsigned p = 2; p < 100000; ++p) {
        if (!euler_is_prime(p)) continue;
        uint64_t residue = 10 % (9ULL * p);
        for (unsigned n = 0; n < 20; ++n)
            residue = euler_powmod(residue, 10, 9ULL * p);
        if (residue != 1) sum += p;
    }
    printf("%" PRIu64 "\n", sum);
    return 0;
}
