/* Project Euler 33: https://projecteuler.net/problem=33
 * Check every proper two-digit fraction and both positions of its common
 * nonzero digit. Equality is an exact cross multiplication. */
#include "euler_extra.h"

int main(void) {
    uint64_t numerator = 1, denominator = 1;
    for (unsigned n = 10; n < 100; ++n)
        for (unsigned d = n + 1; d < 100; ++d) {
            unsigned nd[2] = {n / 10, n % 10}, dd[2] = {d / 10, d % 10};
            bool valid = false;
            for (unsigned i = 0; i < 2; ++i)
                for (unsigned j = 0; j < 2; ++j)
                    if (nd[i] && nd[i] == dd[j] && dd[1 - j] &&
                        n * dd[1 - j] == d * nd[1 - i]) valid = true;
            if (valid) { numerator *= n; denominator *= d; }
        }
    printf("%" PRIu64 "\n", denominator / euler_gcd(numerator, denominator));
    return 0;
}
