/* Project Euler 123: https://projecteuler.net/problem=123 */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 300000 };
    bool *composite = calloc(LIMIT, sizeof(*composite));
    if (!composite) return EXIT_FAILURE;
    unsigned index = 0;
    for (unsigned p = 2; p < LIMIT; ++p) {
        if (composite[p]) continue;
        ++index;
        if ((uint64_t)p * p < LIMIT)
            for (unsigned multiple = p * p; multiple < LIMIT; multiple += p)
                composite[multiple] = true;
        if ((index & 1U) && 2ULL * index * p > 10000000000ULL) {
            printf("%u\n", index);
            free(composite);
            return 0;
        }
    }
    free(composite);
    return EXIT_FAILURE;
}
