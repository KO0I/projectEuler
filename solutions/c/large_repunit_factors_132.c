/* Project Euler 132: Large Repunit Factors
 *
 * R(k)=(10^k-1)/9.  For a prime p other than 2 or 5,
 *
 *     p | R(k)  <=>  10^k == 1 (mod 9p).
 *
 * The modulus 9p retains the division by nine exactly, avoiding an enormous
 * billion-digit repunit.  Binary modular exponentiation in `euler_powmod`
 * tests each prime in O(log k) multiplications.  We scan primes in increasing
 * order, add the first 40 factors, and use 64-bit intermediates for 9p. */
#include "euler_extra.h"

int main(void) {
    unsigned found = 0, sum = 0;
    for (unsigned p = 2; found < 40; ++p) {
        if (!euler_is_prime(p) || p == 2 || p == 5) continue;
        if (euler_powmod(10, 1000000000ULL, 9ULL * p) == 1) {
            sum += p;
            ++found;
        }
    }
    printf("%u\n", sum);
    return 0;
}
