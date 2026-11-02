#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 224: almost right-angled triangles II.
 *
 * The three integral Lorentz transformations below preserve
 * c^2-a^2-b^2=1.  Starting from (2,2,3), they form a ternary tree containing
 * all positive solutions.  The tree retains both a,b orientations because a
 * child that currently has a>b can later produce an ordered descendant; a
 * node is counted only when a<=b.
 *
 * Every transformation strictly increases c for a positive solution, hence
 * once the perimeter exceeds 75,000,000 the entire subtree can be pruned.
 * The recursion depth is small even though the finite tree has millions of
 * nodes, so no large explicit queue is required.
 */

static const int64_t PERIMETER_LIMIT = INT64_C(75000000);

static uint64_t visit(int64_t a, int64_t b, int64_t c) {
    if (a + b + c > PERIMETER_LIMIT) {
        return 0;
    }
    uint64_t count = a <= b;

    count += visit(a - 2 * b + 2 * c,
                   2 * a - b + 2 * c,
                   2 * a - 2 * b + 3 * c);
    count += visit(a + 2 * b + 2 * c,
                   2 * a + b + 2 * c,
                   2 * a + 2 * b + 3 * c);
    count += visit(-a + 2 * b + 2 * c,
                   -2 * a + b + 2 * c,
                   -2 * a + 2 * b + 3 * c);
    return count;
}

int main(void) {
    printf("%llu\n", (unsigned long long)visit(2, 2, 3));
    return 0;
}
