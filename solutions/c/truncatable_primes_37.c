/* Project Euler 37: https://projecteuler.net/problem=37
 * Grow all right-truncatable primes from 2,3,5,7. Appending any digit
 * except 1,3,7,9 makes a composite. Test each surviving node's left
 * truncations; the recursion explores every right-truncatable prime. */
#include "euler_extra.h"

static uint64_t sum;
static unsigned found;

static void visit(uint64_t prime) {
    uint64_t power = 1;
    while (power <= prime / 10) power *= 10;
    bool left_prime = prime >= 10;
    for (; power > 1; power /= 10)
        if (!euler_is_prime(prime % power)) { left_prime = false; break; }
    if (left_prime) { sum += prime; ++found; }
    static const unsigned digits[] = {1,3,7,9};
    if (prime > (UINT64_MAX - 9) / 10) euler_fail("Truncatable-prime search overflow");
    for (size_t i = 0; i < sizeof digits / sizeof *digits; ++i) {
        uint64_t next = 10 * prime + digits[i];
        if (euler_is_prime(next)) visit(next);
    }
}

int main(void) {
    static const unsigned roots[] = {2,3,5,7};
    for (size_t i = 0; i < sizeof roots / sizeof *roots; ++i) visit(roots[i]);
    if (found != 11) euler_fail("The expected eleven truncatable primes were not found");
    printf("%" PRIu64 "\n", sum);
    return 0;
}
