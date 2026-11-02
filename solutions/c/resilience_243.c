#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 243: Resilience
 *
 * R(d)=phi(d)/(d-1).  For a fixed number of distinct prime factors,
 * phi(d)/d=product(1-1/p) is minimized by choosing the smallest primes;
 * raising their exponents does not change this product.  Therefore the first
 * possible scale is the primorial whose product ratio drops below 15499/94744.
 * We build that square-free base, then test its increasing multiples.  This is
 * the standard minimality construction: omitting one of the smaller primes
 * can only worsen the ratio, while a denominator below the base cannot contain
 * all the required distinct factors.
 *
 * The final comparison is cross-multiplied, avoiding floating-point rounding
 * near the strict inequality.
 */

static uint64_t totient(uint64_t n) {
    uint64_t result = n;
    uint64_t remaining = n;
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p != 0)
            continue;
        result -= result / p;
        do { remaining /= p; } while (remaining % p == 0);
    }
    if (remaining > 1)
        result -= result / remaining;
    return result;
}

int main(void) {
    static const uint64_t primes[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    const uint64_t target_numerator = 15499;
    const uint64_t target_denominator = 94744;
    uint64_t base = 1;
    uint64_t phi_base = 1;

    for (size_t i = 0; i < sizeof(primes) / sizeof(primes[0]); ++i) {
        base *= primes[i];
        phi_base *= primes[i] - 1;
        if (phi_base * target_denominator
            < base * target_numerator)
            break;
    }

    for (uint64_t multiplier = 1; ; ++multiplier) {
        const uint64_t d = base * multiplier;
        const uint64_t phi = totient(d);
        if (phi * target_denominator
            < (d - 1) * target_numerator) {
            printf("%llu\n", (unsigned long long)d);
            break;
        }
    }
    return 0;
}
