/* Project Euler 144: https://projecteuler.net/problem=144 */
#include "euler_extra.h"

int main(void) {
    double x0 = 0.0, y0 = 10.1, x = 1.4, y = -9.6;
    unsigned reflections = 0;
    for (;;) {
        double vx = x - x0, vy = y - y0;
        double nx = 4 * x, ny = y;
        double scale = 2 * (vx * nx + vy * ny) / (nx * nx + ny * ny);
        double rx = vx - scale * nx, ry = vy - scale * ny;
        double qa = 4 * rx * rx + ry * ry;
        double qb = 8 * x * rx + 2 * y * ry;
        double t = -qb / qa;
        x0 = x; y0 = y; x += t * rx; y += t * ry;
        ++reflections;
        if (fabs(x) <= 0.01 && y > 0) break;
    }
    printf("%u\n", reflections);
    return 0;
}
