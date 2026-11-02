/* Project Euler 49: https://projecteuler.net/problem=49
 * Exhaust all increasing three-term four-digit arithmetic progressions.
 * A five-bit counter per decimal digit gives an order-free signature. */
#include "euler_extra.h"

static uint64_t signature(unsigned value) {
    uint64_t result = 0;
    do { result += UINT64_C(1) << (5 * (value % 10)); value /= 10; } while (value);
    return result;
}

int main(void) {
    unsigned char *prime = euler_sieve(9999);
    for (unsigned a = 1000; a < 10000; ++a) if (prime[a])
        for (unsigned b = a + 1; 2 * b - a < 10000; ++b) {
            unsigned c = 2 * b - a;
            if (a == 1487 && b == 4817) continue; /* The supplied example. */
            if (prime[b] && prime[c] && signature(a) == signature(b) &&
                signature(a) == signature(c)) {
                uint64_t result = UINT64_C(100000000) * a + UINT64_C(10000) * b + c;
                printf("%" PRIu64 "\n", result);
                free(prime);
                return 0;
            }
        }
    free(prime);
    euler_fail("No second prime progression found");
    return 1;
}
