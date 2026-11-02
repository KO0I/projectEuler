/* Project Euler 153: Investigating Gaussian Integers
 *
 * Real Gaussian divisors contribute the ordinary divisor sum, accumulated as
 * sum_d d*floor(N/d).  A non-real primitive direction a+bi has gcd(a,b)=1 and
 * norm a^2+b^2.  Every scaled divisor k(a+bi), together with its conjugate,
 * contributes 2ak for each multiple of k*norm not exceeding N.
 *
 * Enumerating coprime positive (a,b) prevents counting the same direction at
 * several scales.  The norm bound terminates both loops, and uint64_t is used
 * for the large accumulated divisor sum. */
#include "euler_extra.h"

static unsigned gcd153(unsigned a, unsigned b) {
    while (b) {
        unsigned remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

int main(void) {
    const unsigned limit = 100000000;
    uint64_t answer = 0;
    for (unsigned divisor = 1; divisor <= limit; ++divisor)
        answer += (uint64_t)divisor * (limit / divisor);
    for (unsigned real = 1; (uint64_t)real * real < limit; ++real) {
        for (unsigned imaginary = 1;; ++imaginary) {
            unsigned norm = real * real + imaginary * imaginary;
            if (norm > limit) break;
            if (gcd153(real, imaginary) != 1) continue;
            unsigned max_scale = limit / norm;
            for (unsigned scale = 1; scale <= max_scale; ++scale)
                answer += 2ULL * real * scale * (limit / (norm * scale));
        }
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
