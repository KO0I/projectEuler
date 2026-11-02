/* Project Euler 182: https://projecteuler.net/problem=182

   For RSA exponent e, a message is fixed by encryption modulo a prime p in
   1 + gcd(e-1,p-1) ways.  CRT multiplies the counts for p and q.  We scan the
   valid exponents, retain the smallest fixed-message count, and sum every e
   attaining it.  This is only phi=(p-1)(q-1) iterations. */
#include "euler_extra.h"

static uint64_t gcd182(uint64_t a, uint64_t b) {
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

int main(void) {
    const uint64_t p = 1009, q = 3643, phi = (p - 1) * (q - 1);
    uint64_t minimum = UINT64_MAX, answer = 0;
    for (uint64_t e = 2; e < phi; ++e) {
        if (gcd182(e, phi) != 1) continue;
        uint64_t unconcealed = (gcd182(e - 1, p - 1) + 1) *
                               (gcd182(e - 1, q - 1) + 1);
        if (unconcealed < minimum) { minimum = unconcealed; answer = e; }
        else if (unconcealed == minimum) answer += e;
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
