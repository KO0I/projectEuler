/* Project Euler 140: https://projecteuler.net/problem=140
 * With x=5n+7, integral nuggets solve x^2-5y^2=44.  The six reduced
 * signed seeds generate every positive orbit under multiplication by 9+4sqrt(5). */
#include "euler_extra.h"

static int compare_u64(const void *left, const void *right) {
    uint64_t a = *(const uint64_t *)left, b = *(const uint64_t *)right;
    return a < b ? -1 : a != b;
}

int main(void) {
    const int64_t seeds[6][2] = {{7,1},{7,-1},{8,2},{8,-2},{13,5},{13,-5}};
    uint64_t values[240];
    unsigned count = 0;
    for (unsigned seed = 0; seed < 6; ++seed) {
        int64_t x = seeds[seed][0], y = seeds[seed][1];
        for (unsigned step = 0; step < 35; ++step) {
            if (x > 7 && (x - 7) % 5 == 0) values[count++] = (uint64_t)(x - 7) / 5;
            if (x > INT64_MAX / 29 || y > INT64_MAX / 29) break;
            int64_t next_x = 9 * x + 20 * y;
            int64_t next_y = 4 * x + 9 * y;
            x = next_x; y = next_y;
        }
    }
    qsort(values, count, sizeof(*values), compare_u64);
    uint64_t sum = 0, previous = 0;
    unsigned accepted = 0;
    for (unsigned i = 0; i < count && accepted < 30; ++i) {
        if (values[i] == previous) continue;
        previous = values[i];
        sum += values[i];
        ++accepted;
    }
    printf("%" PRIu64 "\n", sum);
    return accepted == 30 ? 0 : EXIT_FAILURE;
}
