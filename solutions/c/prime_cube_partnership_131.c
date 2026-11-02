/* Project Euler 131: Prime Cube Partnership
 *
 * From n^3+p n^2=m^3, the two cubes must be consecutive: writing m=n+k and
 * comparing factors shows the prime case forces k=1.  The possible primes are
 * therefore
 *
 *     p=(n+1)^3-n^3 = 3n^2+3n+1.
 *
 * Generate this increasing quadratic sequence, stop at one million, and test
 * each term for primality.  This replaces a three-variable search by a short
 * one-dimensional enumeration. */
#include "euler_extra.h"

int main(void) {
    unsigned count = 0;
    for (uint64_t n = 1;; ++n) {
        uint64_t p = 3 * n * n + 3 * n + 1;
        if (p >= 1000000) break;
        if (euler_is_prime(p)) ++count;
    }
    printf("%u\n", count);
    return 0;
}
