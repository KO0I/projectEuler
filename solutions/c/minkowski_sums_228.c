#include <stdio.h>

/*
 * Project Euler 228: Minkowski sums.
 *
 * The outward normals of S_n are angles 2*pi*k/n.  A Minkowski sum merges the
 * edge-normal lists of its convex summands, combining coincident directions,
 * so its side count is the size of their union.  Reducing k/n shows that a
 * direction with denominator d occurs exactly when d divides at least one n
 * in 1864..1909.  There are phi(d) reduced directions of denominator d.
 *
 * Mark all divisors in the interval, compute Euler's phi table by a sieve,
 * and sum phi(d) for marked d.  This uses exact integer direction identities;
 * no trigonometric equality comparisons are involved.
 */

enum { FIRST = 1864, LAST = 1909 };

int main(void) {
    int present[LAST + 1] = {0};
    int phi[LAST + 1];
    for (int n = 0; n <= LAST; ++n) phi[n] = n;
    for (int p = 2; p <= LAST; ++p) {
        if (phi[p] == p) {
            for (int multiple = p; multiple <= LAST; multiple += p) {
                phi[multiple] -= phi[multiple] / p;
            }
        }
    }
    present[1] = 1;
    for (int n = FIRST; n <= LAST; ++n) {
        for (int divisor = 1; divisor * divisor <= n; ++divisor) {
            if (n % divisor == 0) {
                present[divisor] = 1;
                present[n / divisor] = 1;
            }
        }
    }
    int sides = 0;
    for (int d = 1; d <= LAST; ++d) {
        if (present[d]) sides += phi[d];
    }
    printf("%d\n", sides);
    return 0;
}
