/* Project Euler 183: https://projecteuler.net/problem=183

   For k equal parts, the product is (N/k)^k and its logarithm is maximized
   at k=N/e.  Therefore only floor(N/e) and ceil(N/e) need comparison.  After
   reducing N/k, the decimal terminates exactly when its denominator contains
   no prime factors other than 2 and 5. */
#include "euler_extra.h"

static unsigned gcd183(unsigned a, unsigned b) {
    while (b) { unsigned r = a % b; a = b; b = r; }
    return a;
}

int main(void) {
    int64_t answer = 0;
    for (unsigned n = 5; n <= 10000; ++n) {
        unsigned low = (unsigned)(n / exp(1.0));
        unsigned high = low + 1;
        unsigned k = low * log((double)n / low) > high * log((double)n / high) ? low : high;
        unsigned denominator = k / gcd183(n, k);
        while (denominator % 2 == 0) denominator /= 2;
        while (denominator % 5 == 0) denominator /= 5;
        answer += denominator == 1 ? -(int64_t)n : n;
    }
    printf("%" PRId64 "\n", answer);
    return 0;
}
