/* Project Euler 110: https://projecteuler.net/problem=110
 * (x-n)(y-n)=n^2 gives (d(n^2)+1)/2 unordered positive solutions.
 * A minimal n uses consecutive primes with nonincreasing exponents:
 * moving a larger exponent onto a smaller prime can only decrease n.
 * Enumerate every such exponent vector below a constructed feasible
 * primorial. Bounds are proven; no guessed maximum search value is used. */
#include "euler_extra.h"

static const unsigned primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
static uint64_t best, target;

static void search(unsigned index, unsigned maximum_exponent,
                   uint64_t value, uint64_t divisor_product) {
    if (divisor_product > target) {
        if (value < best) best = value;
        return;
    }
    if (index == sizeof primes / sizeof *primes) return;
    for (unsigned exponent = 1; exponent <= maximum_exponent; ++exponent) {
        if (value > (best - 1) / primes[index]) break;
        value *= primes[index];
        uint64_t factor = 2 * exponent + 1;
        uint64_t next_product = divisor_product > target / factor ?
                                target + 1 : divisor_product * factor;
        search(index + 1, exponent, value, next_product);
    }
}
static uint64_t least_denominator(uint64_t solutions) {
    target = 2 * solutions;
    best = 1;
    uint64_t divisors = 1;
    unsigned count = 0;
    while (divisors <= target) {
        if (count == sizeof primes / sizeof *primes ||
            best > UINT64_MAX / primes[count])
            euler_fail("Cannot construct the initial reciprocal-search bound");
        best *= primes[count++];
        divisors *= 3;
    }
    /* 2^64 is unrepresentable, so no feasible exponent of 2 exceeds 63. */
    search(0, 63, 1, 1);
    return best;
}
int main(void) {
    printf("%" PRIu64 "\n", least_denominator(4000000));
    return 0;
}
