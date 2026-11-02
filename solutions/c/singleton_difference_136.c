/* Project Euler 136: https://projecteuler.net/problem=136 */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 50000000 };
    unsigned char *count = calloc(LIMIT, 1);
    if (!count) return EXIT_FAILURE;
    for (unsigned u = 1; (uint64_t)u * (u / 3 + 1) < LIMIT; ++u) {
        unsigned v = u / 3 + 1;
        v += (4 - (u + v) % 4) % 4;
        for (; (uint64_t)u * v < LIMIT; v += 4) {
            unsigned n = u * v;
            if (count[n] < 2) ++count[n];
        }
    }
    unsigned answer = 0;
    for (unsigned n = 1; n < LIMIT; ++n) answer += count[n] == 1;
    printf("%u\n", answer);
    free(count);
    return 0;
}
