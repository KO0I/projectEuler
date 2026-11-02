/* Project Euler 152: Writing One Half as Inverse Squares
 *
 * Prime-denominator valuation arguments eliminate every denominator not in
 * the fixed `candidates` list: a largest prime power occurring only once could
 * never cancel in a sum equal to 1/2.  The remaining 31 terms are handled by
 * meet-in-the-middle rather than 2^31 direct subsets.
 *
 * Enumerate and sort every sum from the right 16 terms.  For each subset of
 * the left 15, binary-search the exact GMP rational `1/2-left_sum`; repeated
 * equal right sums are all counted.  Exact mpq arithmetic avoids any tolerance
 * question when many small reciprocal squares nearly coincide. */
#include "euler_extra.h"

typedef struct { mpq_t value; } rational152;

static int compare152(const void *left, const void *right) {
    const rational152 *a = left, *b = right;
    return mpq_cmp(a->value, b->value);
}

static size_t lower_bound152(rational152 *values, size_t count, const mpq_t key) {
    size_t low = 0, high = count;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (mpq_cmp(values[mid].value, key) < 0) low = mid + 1;
        else high = mid;
    }
    return low;
}

int main(void) {
    static const unsigned candidates[] = {
        2,3,4,5,6,7,8,9,10,12,13,14,15,18,20,21,
        24,28,30,35,36,39,40,42,45,52,56,60,63,70,72
    };
    enum { LEFT = 15, TOTAL = sizeof(candidates) / sizeof(candidates[0]) };
    const size_t right_count = (size_t)1 << (TOTAL - LEFT);
    rational152 *right = malloc(right_count * sizeof(*right));
    if (!right) return EXIT_FAILURE;
    mpq_t terms[TOTAL];
    for (unsigned i = 0; i < TOTAL; ++i) {
        mpq_init(terms[i]);
        mpq_set_ui(terms[i], 1, candidates[i] * candidates[i]);
    }
    mpq_init(right[0].value);
    mpq_set_ui(right[0].value, 0, 1);
    for (size_t mask = 1; mask < right_count; ++mask) {
        size_t bit = (size_t)__builtin_ctzl(mask);
        size_t rest = mask & (mask - 1);
        mpq_init(right[mask].value);
        mpq_add(right[mask].value, right[rest].value, terms[LEFT + bit]);
    }
    qsort(right, right_count, sizeof(*right), compare152);

    mpq_t current, needed;
    mpq_init(current);
    mpq_init(needed);
    uint64_t answer = 0;
    const size_t left_count = (size_t)1 << LEFT;
    for (size_t mask = 0; mask < left_count; ++mask) {
        mpq_set_ui(current, 0, 1);
        for (unsigned bit = 0; bit < LEFT; ++bit)
            if (mask & ((size_t)1 << bit)) mpq_add(current, current, terms[bit]);
        mpq_set_ui(needed, 1, 2);
        mpq_sub(needed, needed, current);
        size_t pos = lower_bound152(right, right_count, needed);
        while (pos < right_count && mpq_cmp(right[pos].value, needed) == 0) {
            ++answer;
            ++pos;
        }
    }
    printf("%" PRIu64 "\n", answer);
    mpq_clear(current);
    mpq_clear(needed);
    for (size_t i = 0; i < right_count; ++i) mpq_clear(right[i].value);
    for (unsigned i = 0; i < TOTAL; ++i) mpq_clear(terms[i]);
    free(right);
    return 0;
}
