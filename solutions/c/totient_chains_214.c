#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 214: totient chains.
 *
 * Euler's sieve initializes phi(n)=n and, on discovering a prime p, applies
 * phi(m) -= phi(m)/p to every multiple.  Once the table is complete,
 * phi(n)<n means chain lengths can be built in increasing n order:
 *
 *       length(n) = 1 + length(phi(n)),  length(1)=1.
 *
 * A byte is sufficient for these lengths.  For n>1, phi(n)=n-1 characterizes
 * primes, so the finished totient table doubles as the primality test and no
 * second 40-million-entry array is needed.
 */

enum { LIMIT = 40000000, TARGET_LENGTH = 25 };

int main(void) {
    uint32_t *phi = malloc((size_t)LIMIT * sizeof(*phi));
    unsigned char *length = malloc((size_t)LIMIT);
    if (phi == NULL || length == NULL) {
        free(length);
        free(phi);
        return 1;
    }
    for (uint32_t n = 0; n < LIMIT; ++n) {
        phi[n] = n;
    }
    for (uint32_t p = 2; p < LIMIT; ++p) {
        if (phi[p] == p) {
            for (uint32_t multiple = p; multiple < LIMIT; multiple += p) {
                phi[multiple] -= phi[multiple] / p;
            }
        }
    }

    length[1] = 1;
    uint64_t sum = 0;
    for (uint32_t n = 2; n < LIMIT; ++n) {
        length[n] = (unsigned char)(length[phi[n]] + 1);
        if (phi[n] == n - 1 && length[n] == TARGET_LENGTH) {
            sum += n;
        }
    }
    printf("%llu\n", (unsigned long long)sum);
    free(length);
    free(phi);
    return 0;
}
