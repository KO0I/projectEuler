#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 184: triangles containing the origin.
 *
 * Enumerating triples would be hopeless: the circle contains about 35,000
 * non-origin lattice points.  Instead, sort those points by polar angle.
 * A non-degenerate triangle misses the origin precisely when its three
 * vertices fit in some open semicircle.  In angular order that triangle has
 * one first vertex; the other two are in the following half-turn.  A sliding
 * window therefore counts every such triangle once as C(window_size, 2).
 *
 * An open window deliberately omits points in the direction opposite its
 * anchor.  Triples containing an antipodal pair put the origin on an edge
 * (or are collinear), so they are not wanted either.  We count those boundary
 * triples by primitive, unoriented lattice direction and subtract them once.
 * All combinatorial totals fit in signed 64 bits for radius 105.
 */

enum { RADIUS = 105 };

typedef struct {
    int x;
    int y;
} Point;

static int half(const Point p) {
    return p.y > 0 || (p.y == 0 && p.x > 0) ? 0 : 1;
}

static int angle_compare(const void *left, const void *right) {
    const Point a = *(const Point *)left;
    const Point b = *(const Point *)right;
    const int ha = half(a);
    const int hb = half(b);
    if (ha != hb) {
        return ha - hb;
    }
    const int64_t cross = (int64_t)a.x * b.y - (int64_t)a.y * b.x;
    if (cross != 0) {
        return cross > 0 ? -1 : 1;
    }
    /* Equal-angle points may appear in either distance order; keeping a total
       order makes qsort deterministic and the semicircle proof unchanged. */
    const int da = a.x * a.x + a.y * a.y;
    const int db = b.x * b.x + b.y * b.y;
    return (da > db) - (da < db);
}

static int gcd_positive(int a, int b) {
    while (b != 0) {
        const int remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

static int64_t choose2(int64_t n) {
    return n * (n - 1) / 2;
}

static int64_t choose3(int64_t n) {
    return n * (n - 1) * (n - 2) / 6;
}

int main(void) {
    const int diameter = 2 * RADIUS + 1;
    const int capacity = diameter * diameter;
    Point *points = malloc((size_t)capacity * sizeof(*points));
    if (points == NULL) {
        return 1;
    }

    int count = 0;
    for (int x = -RADIUS + 1; x < RADIUS; ++x) {
        for (int y = -RADIUS + 1; y < RADIUS; ++y) {
            if ((x != 0 || y != 0) && x * x + y * y < RADIUS * RADIUS) {
                points[count++] = (Point){x, y};
            }
        }
    }
    qsort(points, (size_t)count, sizeof(*points), angle_compare);

    /* Duplicate the angular array conceptually.  The window never crosses a
       closed half-turn, so coordinate tests distinguish the open interval. */
    int64_t outside = 0;
    int end = 1;
    for (int start = 0; start < count; ++start) {
        if (end < start + 1) {
            end = start + 1;
        }
        while (end < start + count) {
            const Point a = points[start];
            const Point b = points[end % count];
            const int64_t cross = (int64_t)a.x * b.y - (int64_t)a.y * b.x;
            const int64_t dot = (int64_t)a.x * b.x + (int64_t)a.y * b.y;
            if (cross > 0 || (cross == 0 && dot > 0 && end < count)) {
                ++end;
            } else {
                break;
            }
        }
        outside += choose2(end - start - 1);
    }

    /* Count triples having points on both rays of one line through zero.
       Canonicalizing the primitive direction to its upper/right half makes
       every such line appear exactly once. */
    int *positive = calloc((size_t)capacity, sizeof(*positive));
    int *negative = calloc((size_t)capacity, sizeof(*negative));
    if (positive == NULL || negative == NULL) {
        free(negative);
        free(positive);
        free(points);
        return 1;
    }
    for (int i = 0; i < count; ++i) {
        int x = points[i].x;
        int y = points[i].y;
        const int g = gcd_positive(abs(x), abs(y));
        x /= g;
        y /= g;
        int side = 1;
        if (y < 0 || (y == 0 && x < 0)) {
            x = -x;
            y = -y;
            side = -1;
        }
        const int index = (y + RADIUS) * diameter + (x + RADIUS);
        if (side > 0) {
            ++positive[index];
        } else {
            ++negative[index];
        }
    }

    int64_t boundary = 0;
    for (int i = 0; i < capacity; ++i) {
        const int64_t a = positive[i];
        const int64_t b = negative[i];
        if (a != 0 && b != 0) {
            boundary += a * b * (count - a - b) + choose2(a) * b + a * choose2(b);
        }
    }

    printf("%lld\n", (long long)(choose3(count) - outside - boundary));
    free(negative);
    free(positive);
    free(points);
    return 0;
}
