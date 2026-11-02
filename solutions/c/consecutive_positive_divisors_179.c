/* Project Euler 179: Consecutive Positive Divisors
 *
 * A divisor sieve computes tau(n) for every n<=10^7: divisor d increments all
 * multiples d,2d,3d,... exactly once.  The largest divisor count in this range
 * is tiny, so unsigned short storage is ample and keeps the sieve cache-friendly.
 *
 * A final linear pass compares tau(n) with tau(n+1) for 2<=n<10^7.  The upper
 * array endpoint is allocated because the last comparison reads tau(10^7). */
#include "euler_extra.h"
int main(void) {
    enum { LIMIT = 10000000 };
    unsigned short *divs = calloc(LIMIT + 1, sizeof(*divs));
    if (!divs) return EXIT_FAILURE;
    for (unsigned d = 1; d <= LIMIT; ++d)
        for (unsigned n = d; n <= LIMIT; n += d) ++divs[n];
    unsigned answer = 0;
    for (unsigned n = 2; n < LIMIT; ++n) answer += divs[n] == divs[n + 1];
    printf("%u\n", answer);
    free(divs);
    return 0;
}
