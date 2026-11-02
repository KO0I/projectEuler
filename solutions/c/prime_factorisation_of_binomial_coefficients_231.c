#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 231: prime factorisation of binomial coefficients.
 *
 * Legendre's formula gives the exponent of prime p in n! as
 * floor(n/p)+floor(n/p^2)+... .  Subtract the exponents for 15,000,000! and
 * 5,000,000! from that for 20,000,000!, multiply each remaining exponent by
 * p, and sum.  A compact Eratosthenes table supplies exactly the needed primes.
 */

enum { N = 20000000, K = 15000000 };

static int factorial_exponent(int n, int prime) {
    int exponent = 0;
    while (n != 0) {
        n /= prime;
        exponent += n;
    }
    return exponent;
}

int main(void) {
    unsigned char *composite = calloc(N + 1, 1);
    if (composite == NULL) return 1;
    for (int p = 2; (int64_t)p * p <= N; ++p) {
        if (!composite[p]) {
            for (int multiple = p * p; multiple <= N; multiple += p) composite[multiple] = 1;
        }
    }
    uint64_t sum = 0;
    for (int p = 2; p <= N; ++p) {
        if (!composite[p]) {
            const int exponent = factorial_exponent(N, p) - factorial_exponent(K, p) -
                                 factorial_exponent(N - K, p);
            sum += (uint64_t)p * (uint64_t)exponent;
        }
    }
    printf("%llu\n", (unsigned long long)sum);
    free(composite);
    return 0;
}
