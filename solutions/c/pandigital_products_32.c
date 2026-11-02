/* Project Euler 32: https://projecteuler.net/problem=32
 * A nine-digit identity must use 1x4=4 or 2x3=4 digits. Consequently the
 * smaller factor is below 100 and the product below 10000. Mark products
 * so identities sharing a product contribute only once. */
#include "euler_extra.h"

static bool add_digits(unsigned n, unsigned *mask) {
    do {
        unsigned digit = n % 10, bit = 1u << digit;
        if (!digit || (*mask & bit)) return false;
        *mask |= bit;
        n /= 10;
    } while (n);
    return true;
}

int main(void) {
    bool seen[10000] = {false};
    unsigned sum = 0;
    for (unsigned a = 1; a < 100; ++a)
        for (unsigned b = a + 1; b <= 9999 / a; ++b) {
            unsigned product = a * b, mask = 0;
            if (add_digits(a, &mask) && add_digits(b, &mask) &&
                add_digits(product, &mask) && mask == 0x3fe && !seen[product]) {
                seen[product] = true;
                sum += product;
            }
        }
    printf("%u\n", sum);
    return 0;
}
