/* Project Euler 124: https://projecteuler.net/problem=124 */
#include "euler_extra.h"

struct radical_entry { unsigned radical, number; };

static int compare_entries(const void *left, const void *right) {
    const struct radical_entry *a = left, *b = right;
    if (a->radical != b->radical) return a->radical < b->radical ? -1 : 1;
    return a->number < b->number ? -1 : a->number != b->number;
}

int main(void) {
    enum { LIMIT = 100000 };
    struct radical_entry *entries = malloc((LIMIT + 1) * sizeof(*entries));
    if (!entries) return EXIT_FAILURE;
    for (unsigned n = 0; n <= LIMIT; ++n) entries[n] = (struct radical_entry){1, n};
    for (unsigned p = 2; p <= LIMIT; ++p)
        if (entries[p].radical == 1)
            for (unsigned n = p; n <= LIMIT; n += p) entries[n].radical *= p;
    qsort(entries + 1, LIMIT, sizeof(*entries), compare_entries);
    printf("%u\n", entries[10000].number);
    free(entries);
    return 0;
}
