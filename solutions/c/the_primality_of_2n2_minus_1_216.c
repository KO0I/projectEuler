#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 216: primality of 2n^2-1.
 *
 * Sieve the polynomial rather than applying a primality test 50 million
 * times.  If an odd prime p divides 2n^2-1, n lies in one of the two residue
 * classes ±sqrt(1/2) mod p.  Such roots exist exactly for p == 1 or 7 mod 8.
 * Tonelli-Shanks finds one root; both progressions are then marked composite.
 * Primes only through sqrt(2N^2-1) are required, because every composite
 * polynomial value has a factor no larger than its square root.
 *
 * Marking by p also marks the exceptional n for which 2n^2-1 equals p rather
 * than merely having p as a proper factor.  After the sieve, those few small
 * n are restored using the already-built prime table.
 */

enum { N_LIMIT = 50000000, PRIME_LIMIT = 70710678 };

static uint64_t power_mod(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    while (exponent != 0) {
        if ((exponent & 1U) != 0U) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1U;
    }
    return result;
}

static uint64_t square_root_mod_prime(uint64_t value, uint64_t prime) {
    if (prime % 4 == 3) {
        return power_mod(value, (prime + 1) / 4, prime);
    }

    uint64_t odd = prime - 1;
    unsigned twos = 0;
    while ((odd & 1U) == 0U) {
        odd >>= 1U;
        ++twos;
    }
    uint64_t nonresidue = 2;
    while (power_mod(nonresidue, (prime - 1) / 2, prime) != prime - 1) {
        ++nonresidue;
    }
    uint64_t c = power_mod(nonresidue, odd, prime);
    uint64_t x = power_mod(value, (odd + 1) / 2, prime);
    uint64_t t = power_mod(value, odd, prime);
    unsigned m = twos;
    while (t != 1) {
        unsigned i = 1;
        uint64_t squared = t * t % prime;
        while (squared != 1) {
            squared = squared * squared % prime;
            ++i;
        }
        const uint64_t b = power_mod(c, UINT64_C(1) << (m - i - 1), prime);
        x = x * b % prime;
        c = b * b % prime;
        t = t * c % prime;
        m = i;
    }
    return x;
}

int main(void) {
    unsigned char *prime_composite = calloc((size_t)PRIME_LIMIT + 1, 1);
    unsigned char *value_composite = calloc((size_t)N_LIMIT + 1, 1);
    if (prime_composite == NULL || value_composite == NULL) {
        free(value_composite);
        free(prime_composite);
        return 1;
    }

    for (int p = 2; (int64_t)p * p <= PRIME_LIMIT; ++p) {
        if (!prime_composite[p]) {
            for (int multiple = p * p; multiple <= PRIME_LIMIT; multiple += p) {
                prime_composite[multiple] = 1;
            }
        }
    }

    for (uint64_t p = 3; p <= PRIME_LIMIT; p += 2) {
        if (prime_composite[p] || (p % 8 != 1 && p % 8 != 7)) {
            continue;
        }
        const uint64_t inverse_two = (p + 1) / 2;
        const uint64_t root = square_root_mod_prime(inverse_two, p);
        for (uint64_t n = root; n <= N_LIMIT; n += p) {
            if (n >= 2) value_composite[n] = 1;
        }
        const uint64_t other = p - root;
        if (other != root) {
            for (uint64_t n = other; n <= N_LIMIT; n += p) {
                if (n >= 2) value_composite[n] = 1;
            }
        }
    }

    /* Restore values that were marked by themselves rather than a proper
       factor.  They satisfy n <= sqrt((PRIME_LIMIT+1)/2). */
    const int restore_limit = (int)sqrt((PRIME_LIMIT + 1) / 2.0) + 1;
    for (int n = 2; n <= restore_limit; ++n) {
        const int64_t value = 2 * (int64_t)n * n - 1;
        if (value <= PRIME_LIMIT && !prime_composite[value]) {
            value_composite[n] = 0;
        }
    }

    int count = 0;
    for (int n = 2; n <= N_LIMIT; ++n) {
        count += !value_composite[n];
    }
    printf("%d\n", count);
    free(value_composite);
    free(prime_composite);
    return 0;
}
