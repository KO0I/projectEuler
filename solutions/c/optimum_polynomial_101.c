/* Project Euler 101: https://projecteuler.net/problem=101
 * Newton's forward-difference interpolation uses only integers. The
 * degree-(k-1) fit is sum_j binomial(n-1,j)*Delta^j u(1). */
#include "euler_extra.h"

static int64_t sequence(unsigned n) {
    int64_t value = 1;
    for (unsigned power = 0; power < 10; ++power) value = 1 - n * value;
    return value;
}
static int64_t fit(const int64_t *first, unsigned terms, unsigned n) {
    int64_t value = 0, choose = 1;
    for (unsigned j = 0; j < terms; ++j) {
        value += first[j] * choose;
        choose = choose * (n - 1 - j) / (j + 1);
    }
    return value;
}
int main(void) {
    int64_t total = 0;
    for (unsigned k = 1; k <= 10; ++k) {
        int64_t differences[10], first[10];
        for (unsigned i = 0; i < k; ++i) differences[i] = sequence(i + 1);
        for (unsigned order = 0; order < k; ++order) {
            first[order] = differences[0];
            for (unsigned i = 0; i + order + 1 < k; ++i)
                differences[i] = differences[i + 1] - differences[i];
        }
        unsigned n = k + 1;
        while (fit(first, k, n) == sequence(n)) ++n;
        total += fit(first, k, n);
    }
    printf("%" PRId64 "\n", total);
    return 0;
}
