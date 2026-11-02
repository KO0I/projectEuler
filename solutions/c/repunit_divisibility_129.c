/* Project Euler 129: Repunit Divisibility
 *
 * A(k) is found without constructing the k-digit repunit.  If r is the
 * current remainder modulo n, appending a decimal 1 changes it to
 * (10r+1) mod n.  For gcd(n,10)=1 a zero remainder must eventually occur,
 * and the iteration count is exactly A(n).
 *
 * The answer exceeds one million and must be odd and not divisible by five,
 * so the scan starts immediately above the bound, skips even values, and
 * rejects multiples of five before running the remainder recurrence.  Every
 * remainder fits in `unsigned`; 10r+1 also fits for this search range. */
#include "euler_extra.h"

static unsigned repunit_length(unsigned n) {
    unsigned remainder = 1 % n, length = 1;
    while (remainder) { remainder = (10 * remainder + 1) % n; ++length; }
    return length;
}

int main(void) {
    for (unsigned n = 1000001;; n += 2) {
        if (n % 5 && repunit_length(n) > 1000000) {
            printf("%u\n", n);
            return 0;
        }
    }
}
