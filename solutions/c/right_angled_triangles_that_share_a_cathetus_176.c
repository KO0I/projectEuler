/* Project Euler 176: https://projecteuler.net/problem=176 */
#include "euler_extra.h"

static const uint64_t primes176[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
static uint64_t best176 = UINT64_MAX;

static void search176(unsigned position, unsigned max_exponent,
                      uint64_t remaining_divisors, uint64_t value) {
    if (remaining_divisors == 1) {
        if (value < best176) best176 = value;
        return;
    }
    if (position == sizeof(primes176) / sizeof(primes176[0])) return;
    uint64_t power = 1;
    for (unsigned exponent = 1; exponent <= max_exponent; ++exponent) {
        if (power > best176 / primes176[position]) break;
        power *= primes176[position];
        uint64_t factor = 2 * exponent + 1;
        if (remaining_divisors % factor == 0 && value <= best176 / power)
            search176(position + 1, exponent, remaining_divisors / factor,
                      value * power);
    }
}

int main(void) {
    /* For even n, the number of triangles is (d((n/2)^2)-1)/2. */
    search176(0, 31, 2ULL * 47547 + 1, 1);
    printf("%" PRIu64 "\n", 2 * best176);
    return 0;
}
