#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 211: divisor square sum.
 *
 * Factoring all 64 million integers with a full smallest-prime-factor table
 * would consume hundreds of megabytes.  Instead, process one million values
 * at a time.  A small sieve supplies primes through sqrt(64,000,000).  For
 * each prime p, divide p^e from every affected residual and multiply sigma2
 * by 1+p^2+...+p^(2e).  Any residual greater than one after that pass is one
 * final prime factor.  This is the usual segmented factor sieve and keeps the
 * working set near 16 MB.
 *
 * A long-double square-root estimate is corrected with integer comparisons;
 * this prevents rounding at a perfect-square boundary from changing the sum.
 */

enum { LIMIT = 64000000, BLOCK = 1000000, PRIME_LIMIT = 8000 };

static uint64_t integer_sqrt(uint64_t value) {
    uint64_t root = (uint64_t)sqrtl((long double)value);
    while ((root + 1) * (root + 1) <= value) {
        ++root;
    }
    while (root * root > value) {
        --root;
    }
    return root;
}

int main(void) {
    unsigned char composite[PRIME_LIMIT + 1] = {0};
    int primes[1100];
    int prime_count = 0;
    for (int n = 2; n <= PRIME_LIMIT; ++n) {
        if (!composite[n]) {
            primes[prime_count++] = n;
            if (n * n <= PRIME_LIMIT) {
                for (int multiple = n * n; multiple <= PRIME_LIMIT; multiple += n) {
                    composite[multiple] = 1;
                }
            }
        }
    }

    uint64_t *residual = malloc((size_t)BLOCK * sizeof(*residual));
    uint64_t *sigma2 = malloc((size_t)BLOCK * sizeof(*sigma2));
    if (residual == NULL || sigma2 == NULL) {
        free(sigma2);
        free(residual);
        return 1;
    }

    uint64_t answer = 0;
    for (int low = 1; low < LIMIT; low += BLOCK) {
        const int high = low + BLOCK < LIMIT ? low + BLOCK : LIMIT;
        const int length = high - low;
        for (int i = 0; i < length; ++i) {
            residual[i] = (uint64_t)(low + i);
            sigma2[i] = 1;
        }

        for (int pi = 0; pi < prime_count; ++pi) {
            const uint64_t p = (uint64_t)primes[pi];
            if (p * p >= (uint64_t)high) {
                break;
            }
            int64_t first = ((int64_t)low + (int64_t)p - 1) / (int64_t)p * (int64_t)p;
            for (int64_t value = first; value < high; value += (int64_t)p) {
                const int index = (int)(value - low);
                uint64_t term = 1;
                uint64_t power = 1;
                const uint64_t p_squared = p * p;
                while (residual[index] % p == 0) {
                    residual[index] /= p;
                    power *= p_squared;
                    term += power;
                }
                sigma2[index] *= term;
            }
        }

        for (int i = 0; i < length; ++i) {
            if (residual[i] > 1) {
                sigma2[i] *= 1 + residual[i] * residual[i];
            }
            const uint64_t root = integer_sqrt(sigma2[i]);
            if (root * root == sigma2[i]) {
                answer += (uint64_t)(low + i);
            }
        }
    }

    printf("%llu\n", (unsigned long long)answer);
    free(sigma2);
    free(residual);
    return 0;
}
