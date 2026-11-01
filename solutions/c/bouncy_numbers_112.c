/* Project Euler 112: https://projecteuler.net/problem=112
 * A bouncy number contains both a rising and a falling adjacent digit
 * pair. Test the target proportion by integer cross multiplication. */
#include "euler_extra.h"
static bool bouncy(unsigned n) {
    bool rises = false, falls = false;
    unsigned previous = n % 10;
    while ((n /= 10)) {
        unsigned digit = n % 10;
        rises |= digit < previous;
        falls |= digit > previous;
        previous = digit;
    }
    return rises && falls;
}
int main(void) {
    uint64_t count = 0;
    for (unsigned n = 1; ; ++n) {
        count += bouncy(n);
        if (100 * count == 99ULL * n) {
            printf("%u\n", n);
            return 0;
        }
    }
}
