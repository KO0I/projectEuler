/* Project Euler 126: https://projecteuler.net/problem=126 */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 30000, TARGET = 1000 };
    unsigned *count = calloc(LIMIT, sizeof(*count));
    if (!count) return EXIT_FAILURE;
    for (unsigned a = 1; 6 * a * a < LIMIT; ++a) {
        for (unsigned b = a; 2 * (a * b + a * b + b * b) < LIMIT; ++b) {
            for (unsigned c = b;; ++c) {
                unsigned first = 2 * (a * b + a * c + b * c);
                if (first >= LIMIT) break;
                for (unsigned layer = 1, cubes = first; cubes < LIMIT; ++layer) {
                    ++count[cubes];
                    cubes += 4 * (a + b + c) + 8 * (layer - 1);
                }
            }
        }
    }
    for (unsigned n = 1; n < LIMIT; ++n) {
        if (count[n] == TARGET) { printf("%u\n", n); free(count); return 0; }
    }
    free(count);
    return EXIT_FAILURE;
}
