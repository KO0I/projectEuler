#include <math.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 210: obtuse-angled triangles.
 *
 * Write r=4k and B=(x,y).  The dot products at O, C=(k,k), and B show that
 * the three mutually exclusive obtuse regions are
 *
 *   at O: x+y < 0,
 *   at C: x+y > 2k,
 *   at B: x^2+y^2-kx-ky < 0.
 *
 * On diagonal x+y=s, parity leaves r+1 points for even s and r points for
 * odd s.  Summing the negative diagonals and those above 2k gives
 * r(2r+1)/2 + r(2r+1)/4 points.
 * Completing squares in the third inequality, and using even k, translates
 * it to the integer circle
 *
 *       X^2+Y^2 < k^2/2.
 *
 * Strictness is represented by L=(k^2-1)/2.  To count the circle efficiently,
 * inspect only positive points with X<=Y.  A monotone Y pointer decreases as
 * X increases, so the loop performs O(k) additions/decrements and no costly
 * square root per lattice column.  Reflection across X=Y and across the axes
 * reconstructs the complete circle exactly.  Finally, remove the 4k-1
 * non-vertex lattice points collinear with O and C: the dot-product tests
 * include them, but their largest angle is 180 degrees, explicitly excluded.
 */

static uint64_t integer_sqrt(uint64_t value) {
    uint64_t root = (uint64_t)sqrtl((long double)value);
    while ((root + 1) * (root + 1) <= value) {
        ++root;
    }
    while (root * root > value) {
        --root;
    }
    return root;
}

static uint64_t circle_points(uint64_t k) {
    const uint64_t limit = (k * k - 1) / 2;
    const uint64_t radius = integer_sqrt(limit);
    const uint64_t diagonal_limit = integer_sqrt(limit / 2);

    uint64_t y = radius;
    uint64_t upper_half = 0;
    for (uint64_t x = 1; x <= diagonal_limit; ++x) {
        while (x * x + y * y > limit) {
            --y;
        }
        /* Positive points (x,y') with x<=y'<=y. */
        upper_half += y - x + 1;
    }

    /* Mirror across the diagonal.  Its diagonal_limit points were included
       in both halves and are therefore removed once. */
    const uint64_t positive_quadrant = 2 * upper_half - diagonal_limit;
    return 1 + 4 * radius + 4 * positive_quadrant;
}

int main(void) {
    const uint64_t r = UINT64_C(1000000000);
    const uint64_t k = r / 4;
    const uint64_t linear_regions = r * (2 * r + 1) / 2 + r * (2 * r + 1) / 4;
    const uint64_t degenerate = 4 * k - 1;
    printf("%llu\n",
           (unsigned long long)(linear_regions + circle_points(k) - degenerate));
    return 0;
}
