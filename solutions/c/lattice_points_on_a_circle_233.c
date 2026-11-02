#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 233: Lattice Points on a Circle
 *
 * Doubling the coordinates changes the circle equation to
 *
 *     (2x-N)^2 + (2y-N)^2 = 2N^2.
 *
 * The sum-of-two-squares formula therefore says that the number of lattice
 * points is 4*product(2e+1), where e ranges over exponents of primes congruent
 * to 1 modulo 4 in N.  (The extra factor 2 and primes 3 mod 4 do not affect
 * the product.)  Requiring 420 points means product(2e+1)=105, whose exponent
 * patterns are exactly
 *
 *     [52], [17,1], [10,2], [7,3], [3,2,1].
 *
 * We enumerate the small "core" made from those 1-mod-4 prime powers.  Every
 * valid N is core*m, where m has no 1-mod-4 prime factor.  A sieve and prefix
 * sum let us add all such m <= 10^11/core at once.  The smallest possible core
 * is 5^3*13^2*17=359125, so the cofactor sieve only needs 278454 entries.
 */

enum { PRIME_LIMIT = 4800000, COFACTOR_LIMIT = 278454 };
static const uint64_t LIMIT = UINT64_C(100000000000);

static int *primes;
static int prime_count;
static uint64_t cofactor_sum[COFACTOR_LIMIT + 1];
static uint64_t answer;

/* Return base^exponent, or LIMIT+1 as an overflow/too-large sentinel. */
static uint64_t bounded_power(uint64_t base, int exponent, uint64_t bound) {
    uint64_t result = 1;
    for (int i = 0; i < exponent; ++i) {
        if (result > bound / base)
            return bound + 1;
        result *= base;
    }
    return result;
}

static void enumerate_cores(const int *exponents, int length, int depth,
                            uint64_t core, int used0, int used1) {
    if (depth == length) {
        const uint64_t max_cofactor = LIMIT / core;
        answer += core * cofactor_sum[max_cofactor];
        return;
    }

    const uint64_t room = LIMIT / core;
    for (int i = 0; i < prime_count; ++i) {
        if (i == used0 || i == used1)
            continue; /* one prime cannot carry two different exponents */
        const uint64_t power = bounded_power((uint64_t)primes[i],
                                             exponents[depth], room);
        if (power > room)
            break; /* primes are increasing, hence later powers are larger */
        enumerate_cores(exponents, length, depth + 1, core * power,
                        depth == 0 ? i : used0,
                        depth == 1 ? i : used1);
    }
}

int main(void) {
    unsigned char *composite = calloc(PRIME_LIMIT + 1, 1);
    unsigned char *forbidden = calloc(COFACTOR_LIMIT + 1, 1);
    primes = malloc((PRIME_LIMIT / 2) * sizeof(*primes));
    if (composite == NULL || forbidden == NULL || primes == NULL)
        return 1;

    /* Sieve all possible 1-mod-4 core primes.  The upper bound follows from
       LIMIT/(5^3*13^2), the largest possible exponent-one prime. */
    for (int p = 2; p <= PRIME_LIMIT; ++p) {
        if (!composite[p]) {
            if (p % 4 == 1)
                primes[prime_count++] = p;
            if ((int64_t)p * p <= PRIME_LIMIT)
                for (int q = p * p; q <= PRIME_LIMIT; q += p)
                    composite[q] = 1;
        }
    }

    /* A cofactor is forbidden as soon as it contains any 1-mod-4 prime. */
    for (int i = 0; i < prime_count && primes[i] <= COFACTOR_LIMIT; ++i)
        for (int m = primes[i]; m <= COFACTOR_LIMIT; m += primes[i])
            forbidden[m] = 1;
    for (int m = 1; m <= COFACTOR_LIMIT; ++m)
        cofactor_sum[m] = cofactor_sum[m - 1]
                          + (forbidden[m] ? 0U : (uint64_t)m);

    static const int patterns[][3] = {
        {52, 0, 0}, {17, 1, 0}, {10, 2, 0}, {7, 3, 0}, {3, 2, 1}
    };
    static const int lengths[] = {1, 2, 2, 2, 3};
    for (int i = 0; i < 5; ++i)
        enumerate_cores(patterns[i], lengths[i], 0, 1, -1, -1);

    printf("%llu\n", (unsigned long long)answer);
    free(primes);
    free(forbidden);
    free(composite);
    return 0;
}
