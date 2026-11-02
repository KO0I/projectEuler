/* Project Euler 165: https://projecteuler.net/problem=165 */
#include "euler_extra.h"

typedef struct { int32_t xn, xd, yn, yd; } point165;
typedef struct { int x1, y1, x2, y2; } segment165;
enum { TABLE165 = 1 << 23 };
static point165 *points165;

static int64_t gcd165(int64_t a, int64_t b) {
    if (a < 0) a = -a;
    while (b) {
        int64_t remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

static uint64_t mix165(uint64_t value) {
    value ^= value >> 30;
    value *= 0xbf58476d1ce4e5b9ULL;
    value ^= value >> 27;
    value *= 0x94d049bb133111ebULL;
    return value ^ (value >> 31);
}

static bool insert165(point165 point) {
    uint64_t hash = mix165((uint32_t)point.xn);
    hash ^= mix165((uint32_t)point.xd + 0x9e3779b9U);
    hash ^= mix165((uint32_t)point.yn + 0x85ebca6bU);
    hash ^= mix165((uint32_t)point.yd + 0xc2b2ae35U);
    size_t slot = hash & (TABLE165 - 1);
    while (points165[slot].xd) {
        point165 old = points165[slot];
        if (old.xn == point.xn && old.xd == point.xd &&
            old.yn == point.yn && old.yd == point.yd) return false;
        slot = (slot + 1) & (TABLE165 - 1);
    }
    points165[slot] = point;
    return true;
}

int main(void) {
    segment165 segments[5000];
    uint64_t state = 290797;
    for (unsigned i = 0; i < 5000; ++i) {
        int *coordinates = &segments[i].x1;
        for (unsigned j = 0; j < 4; ++j) {
            state = state * state % 50515093;
            coordinates[j] = state % 500;
        }
    }
    points165 = calloc(TABLE165, sizeof(*points165));
    if (!points165) return EXIT_FAILURE;
    unsigned answer = 0;
    for (unsigned i = 0; i < 5000; ++i) {
        int64_t rx = segments[i].x2 - segments[i].x1;
        int64_t ry = segments[i].y2 - segments[i].y1;
        for (unsigned j = i + 1; j < 5000; ++j) {
            int64_t sx = segments[j].x2 - segments[j].x1;
            int64_t sy = segments[j].y2 - segments[j].y1;
            int64_t qx = segments[j].x1 - segments[i].x1;
            int64_t qy = segments[j].y1 - segments[i].y1;
            int64_t denominator = rx * sy - ry * sx;
            int64_t tnum = qx * sy - qy * sx;
            int64_t unum = qx * ry - qy * rx;
            if (denominator < 0) {
                denominator = -denominator;
                tnum = -tnum;
                unum = -unum;
            }
            if (tnum <= 0 || tnum >= denominator || unum <= 0 || unum >= denominator)
                continue;
            int64_t xn = segments[i].x1 * denominator + rx * tnum;
            int64_t yn = segments[i].y1 * denominator + ry * tnum;
            int64_t xgcd = gcd165(xn, denominator);
            int64_t ygcd = gcd165(yn, denominator);
            point165 point = {(int32_t)(xn / xgcd), (int32_t)(denominator / xgcd),
                              (int32_t)(yn / ygcd), (int32_t)(denominator / ygcd)};
            answer += insert165(point);
        }
    }
    printf("%u\n", answer);
    free(points165);
    return 0;
}
