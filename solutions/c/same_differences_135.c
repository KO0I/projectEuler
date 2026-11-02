/* Project Euler 135: https://projecteuler.net/problem=135 */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 1000000 };
    unsigned char *representations = calloc(LIMIT, 1);
    if (!representations) return EXIT_FAILURE;
    for (unsigned u = 1; u < LIMIT; ++u) {
        for (unsigned v = (u + 2) / 3; (uint64_t)u * v < LIMIT; ++v) {
            if (u >= 3 * v || ((u + v) & 3U)) continue;
            unsigned n = u * v;
            if (representations[n] < 11) ++representations[n];
        }
    }
    unsigned count = 0;
    for (unsigned n = 1; n < LIMIT; ++n) count += representations[n] == 10;
    printf("%u\n", count);
    free(representations);
    return 0;
}
