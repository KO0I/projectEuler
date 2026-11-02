#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 234: Semidivisible Numbers
 *
 * Between consecutive prime squares p^2 and q^2, the primes immediately
 * below and above sqrt(n) are fixed: p and q.  Consequently n is
 * semidivisible precisely when it is divisible by exactly one of p and q.
 * We process each complete (or final truncated) interval as three arithmetic
 * progressions:
 *
 *   sum(multiples of p) + sum(multiples of q)
 *       - 2*sum(multiples of p*q).
 *
 * A common multiple was included twice but must be included zero times, which
 * explains the factor two.  This interval formula avoids examining any of the
 * roughly 10^12 candidate integers individually.
 */

static const uint64_t LIMIT = UINT64_C(999966663333);

/* Sum multiples of d in the inclusive interval [low, high].  The largest
   intermediate is about LIMIT^2/d, so unsigned __int128 prevents overflow. */
static unsigned __int128 multiple_sum(uint64_t d, uint64_t low,
                                      uint64_t high) {
    const uint64_t first = (low + d - 1) / d;
    const uint64_t last = high / d;
    if (first > last)
        return 0;
    const unsigned __int128 count = last - first + 1;
    return (unsigned __int128)d * (first + last) * count / 2;
}

int main(void) {
    enum { SIEVE_LIMIT = 1010000 };
    unsigned char *composite = calloc(SIEVE_LIMIT + 1, 1);
    int *primes = malloc((SIEVE_LIMIT / 2) * sizeof(*primes));
    int count = 0;
    if (composite == NULL || primes == NULL)
        return 1;

    /* The sieve deliberately extends past sqrt(LIMIT), because the final
       interval needs the first prime on the far side of that square root. */
    for (int p = 2; p <= SIEVE_LIMIT; ++p) {
        if (!composite[p]) {
            primes[count++] = p;
            if ((int64_t)p * p <= SIEVE_LIMIT)
                for (int m = p * p; m <= SIEVE_LIMIT; m += p)
                    composite[m] = 1;
        }
    }

    unsigned __int128 total = 0;
    for (int i = 0; i + 1 < count; ++i) {
        const uint64_t p = (uint64_t)primes[i];
        const uint64_t q = (uint64_t)primes[i + 1];
        const uint64_t low = p * p + 1;
        if (low > LIMIT)
            break;
        uint64_t high = q * q - 1;
        if (high > LIMIT)
            high = LIMIT;

        total += multiple_sum(p, low, high);
        total += multiple_sum(q, low, high);
        total -= 2 * multiple_sum(p * q, low, high);
    }

    /* The known result fits in 64 bits; the wide accumulator merely protects
       the arithmetic used to obtain it. */
    printf("%llu\n", (unsigned long long)total);
    free(primes);
    free(composite);
    return 0;
}
