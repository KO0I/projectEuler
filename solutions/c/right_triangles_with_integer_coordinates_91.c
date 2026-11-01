/* Project Euler 91: https://projecteuler.net/problem=91
 * A perpendicular primitive step at (x,y) is (-y/g,x/g), g=gcd(x,y).
 * Count both directions within the square; axes contribute 3*N*N. */
#include "euler_extra.h"

static uint64_t triangles(unsigned limit) {
    uint64_t total = 3ULL * limit * limit;
    for (unsigned x = 1; x <= limit; ++x) {
        for (unsigned y = 1; y <= limit; ++y) {
            unsigned g = (unsigned)euler_gcd(x, y);
            unsigned dx = y / g, dy = x / g;
            unsigned left = x / dx, up = (limit - y) / dy;
            unsigned right = (limit - x) / dx, down = y / dy;
            total += left < up ? left : up;
            total += right < down ? right : down;
        }
    }
    return total;
}
int main(void) {
    printf("%" PRIu64 "\n", triangles(50));
    return 0;
}
