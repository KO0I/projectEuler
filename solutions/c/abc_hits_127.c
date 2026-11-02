/* Project Euler 127: https://projecteuler.net/problem=127 */
#include "euler_extra.h"

struct radical_number { unsigned radical, number; };
static int by_radical(const void *left, const void *right) {
    const struct radical_number *a = left, *b = right;
    if (a->radical != b->radical) return a->radical < b->radical ? -1 : 1;
    return a->number < b->number ? -1 : a->number != b->number;
}

int main(void) {
    enum { LIMIT = 120000 };
    unsigned *rad = malloc(LIMIT * sizeof(*rad));
    struct radical_number *ordered = malloc(LIMIT * sizeof(*ordered));
    if (!rad || !ordered) { free(rad); free(ordered); return EXIT_FAILURE; }
    for (unsigned n = 0; n < LIMIT; ++n) rad[n] = 1;
    for (unsigned p = 2; p < LIMIT; ++p)
        if (rad[p] == 1)
            for (unsigned n = p; n < LIMIT; n += p) rad[n] *= p;
    for (unsigned n = 1; n < LIMIT; ++n) ordered[n - 1] = (struct radical_number){rad[n], n};
    qsort(ordered, LIMIT - 1, sizeof(*ordered), by_radical);
    uint64_t total = 0;
    for (unsigned c = 3; c < LIMIT; ++c) {
        for (unsigned i = 0; i < LIMIT - 1; ++i) {
            unsigned a = ordered[i].number;
            if ((uint64_t)ordered[i].radical * rad[c] >= c) break;
            if (a >= (c + 1) / 2) continue;
            unsigned b = c - a;
            if (euler_gcd(a, c) == 1 &&
                (uint64_t)rad[a] * rad[b] * rad[c] < c) total += c;
        }
    }
    printf("%" PRIu64 "\n", total);
    free(rad); free(ordered);
    return 0;
}
