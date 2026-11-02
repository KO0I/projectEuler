/* Project Euler 94: https://projecteuler.net/problem=94
 * With equal sides a and base a+delta, x=3*a-delta and y=2*height
 * satisfy x*x-3*y*y=4. Multiplication by 2+sqrt(3) enumerates the
 * positive solutions in increasing order; perimeters are x+/-2. */
#include "euler_extra.h"

int main(void) {
    const uint64_t limit = UINT64_C(1000000000);
    uint64_t x = 2, y = 0, total = 0;
    while (x <= limit + 2) {
        uint64_t a, b, perimeter;
        if (x % 3 == 2) {
            a = (x + 1) / 3; b = a + 1; perimeter = x + 2;
        } else {
            a = (x - 1) / 3; b = a ? a - 1 : 0; perimeter = x - 2;
        }
        if (a > 1 && b && perimeter <= limit && b * y % 4 == 0)
            total += perimeter;
        uint64_t next_x = 2 * x + 3 * y;
        y = x + 2 * y;
        x = next_x;
    }
    printf("%" PRIu64 "\n", total);
    return 0;
}
