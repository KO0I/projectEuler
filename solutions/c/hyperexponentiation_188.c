/* Project Euler 188: https://projecteuler.net/problem=188

   Because 1777 is coprime to every modulus in the Euler-totient chain of
   10^8, Euler's theorem lets each exponent be reduced modulo phi(m).  The
   modulus reaches 1 after very few levels, so height 1855 costs only a short
   recursion, not a tower-sized integer. */
#include "euler_extra.h"

static uint64_t phi188(uint64_t n) {
    uint64_t result = n;
    for (uint64_t p = 2; p * p <= n; ++p) if (n % p == 0) {
        while (n % p == 0) n /= p;
        result -= result / p;
    }
    if (n > 1) result -= result / n;
    return result;
}
static uint64_t power188(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    while (exponent) {
        if (exponent & 1) result = result * base % modulus;
        base = base * base % modulus;
        exponent >>= 1;
    }
    return result;
}
static uint64_t tower188(unsigned height, uint64_t modulus) {
    if (modulus == 1) return 0;
    if (height == 1) return 1777 % modulus;
    return power188(1777, tower188(height - 1, phi188(modulus)), modulus);
}
int main(void) { printf("%08" PRIu64 "\n", tower188(1855, 100000000)); return 0; }
