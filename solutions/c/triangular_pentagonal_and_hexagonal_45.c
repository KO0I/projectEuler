/* Project Euler 45: https://projecteuler.net/problem=45
 * H_n=T_(2n-1), so test only hexagonal numbers after H_143=40755.
 * A positive x is pentagonal exactly when 24x+1 is a square whose
 * positive square root is congruent to five modulo six. */
#include "euler_extra.h"

int main(void) {
    for (uint64_t n = 144; ; ++n) {
        if (n > UINT64_MAX / 2) euler_fail("Hexagonal index overflow");
        if (n > UINT64_MAX / (2 * n - 1)) euler_fail("Hexagonal search overflow");
        uint64_t value = n * (2 * n - 1);
        if (value > (UINT64_MAX - 1) / 24) euler_fail("Pentagonal test overflow");
        uint64_t discriminant = 24 * value + 1, root = euler_isqrt(discriminant);
        if (root * root == discriminant && (root + 1) % 6 == 0) {
            printf("%" PRIu64 "\n", value);
            return 0;
        }
    }
}
