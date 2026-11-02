/* Project Euler 130: Composites with Prime Repunit Property
 *
 * Repeatedly applying r <- (10r+1) mod n computes A(n), the least repunit
 * length divisible by n, without storing that repunit.  The requested
 * composite analogue of Fermat's property is precisely A(n) | n-1.
 *
 * Only numbers coprime to 10 can divide a repunit, so the outer loop visits
 * odd values and excludes multiples of five.  Primes are excluded explicitly;
 * qualifying composites are accumulated until the first 25 have been found. */
#include "euler_extra.h"

static unsigned repunit_length(unsigned n) {
    unsigned remainder = 1 % n, length = 1;
    while (remainder) { remainder = (10 * remainder + 1) % n; ++length; }
    return length;
}

int main(void) {
    unsigned count = 0, sum = 0;
    for (unsigned n = 3; count < 25; n += 2) {
        if (n % 5 == 0 || euler_is_prime(n)) continue;
        if ((n - 1) % repunit_length(n) == 0) { sum += n; ++count; }
    }
    printf("%u\n", sum);
    return 0;
}
