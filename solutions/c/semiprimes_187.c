/* Project Euler 187: https://projecteuler.net/problem=187

   A semiprime has one unique ordered-by-size representation p*q with p<=q.
   Sieve all possible primes q below LIMIT/2.  For each p, move a shared upper
   index down until p*q is below LIMIT; adding the interval length counts every
   prime pair once. */
#include "euler_extra.h"
int main(void) {
    enum { LIMIT = 100000000, SIEVE = LIMIT / 2 };
    bool *composite = calloc(SIEVE, 1);
    unsigned *primes = malloc(3100000 * sizeof(*primes));
    if (!composite || !primes) return EXIT_FAILURE;
    size_t count = 0;
    for (unsigned n = 2; n < SIEVE; ++n) {
        if (composite[n]) continue;
        primes[count++] = n;
        if ((uint64_t)n * n < SIEVE)
            for (unsigned multiple = n * n; multiple < SIEVE; multiple += n) composite[multiple] = true;
    }
    uint64_t answer = 0;
    size_t high = count;
    for (size_t low = 0; low < count; ++low) {
        while (high > low && (uint64_t)primes[low] * primes[high - 1] >= LIMIT) --high;
        if (high <= low) break;
        answer += high - low;
    }
    printf("%" PRIu64 "\n", answer);
    free(composite); free(primes);
    return 0;
}
