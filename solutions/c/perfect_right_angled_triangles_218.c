#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 218: perfect right-angled triangles.
 *
 * This zero is proved, not hard-coded.  A primitive Pythagorean triangle with
 * square hypotenuse has Euclid parameters
 *
 *   m=u^2-v^2, n=2uv, c=(u^2+v^2)^2,
 *
 * where u,v are coprime and of opposite parity.  Its area is
 *
 *   A = m*n*(m^2-n^2).
 *
 * “Super-perfect” means divisible by both 6 and 28, equivalently by 84.
 * Divisibility of this polynomial depends only on u,v modulo 84.  The finite
 * loop below exhausts every residue pair compatible with coprimality (not
 * both divisible by 2, 3, or 7, and opposite parity) and counts violations.
 * Since every integer parameter pair maps to one of these residues, a zero
 * count is a complete modular proof independent of the 10^16 size bound.
 */

int main(void) {
    int violations = 0;
    for (int u = 0; u < 84; ++u) {
        for (int v = 0; v < 84; ++v) {
            if (((u ^ v) & 1) == 0 ||
                (u % 3 == 0 && v % 3 == 0) ||
                (u % 7 == 0 && v % 7 == 0)) {
                continue;
            }
            const int64_t u2 = (int64_t)u * u;
            const int64_t v2 = (int64_t)v * v;
            const int64_t m = u2 - v2;
            const int64_t n = 2 * (int64_t)u * v;
            int64_t area_mod = m * n % 84 * ((m * m - n * n) % 84) % 84;
            if (area_mod < 0) {
                area_mod += 84;
            }
            violations += area_mod != 0;
        }
    }
    printf("%d\n", violations);
    return 0;
}
