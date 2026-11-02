/* Project Euler 125: https://projecteuler.net/problem=125 */
#include "euler_extra.h"

static bool palindrome(unsigned value) {
    unsigned original = value, reverse = 0;
    do { reverse = reverse * 10 + value % 10; value /= 10; } while (value);
    return reverse == original;
}

static int compare_unsigned(const void *left, const void *right) {
    unsigned a = *(const unsigned *)left, b = *(const unsigned *)right;
    return a < b ? -1 : a != b;
}

int main(void) {
    enum { LIMIT = 100000000, ROOT = 10000 };
    uint64_t prefix[ROOT + 1] = {0};
    unsigned capacity = 1024, count = 0;
    unsigned *values = malloc(capacity * sizeof(*values));
    if (!values) return EXIT_FAILURE;
    for (unsigned n = 1; n <= ROOT; ++n) prefix[n] = prefix[n - 1] + (uint64_t)n * n;
    for (unsigned first = 1; first < ROOT; ++first) {
        for (unsigned last = first + 1; last <= ROOT; ++last) {
            uint64_t sum = prefix[last] - prefix[first - 1];
            if (sum >= LIMIT) break;
            if (!palindrome((unsigned)sum)) continue;
            if (count == capacity) {
                capacity *= 2;
                unsigned *grown = realloc(values, capacity * sizeof(*values));
                if (!grown) { free(values); return EXIT_FAILURE; }
                values = grown;
            }
            values[count++] = (unsigned)sum;
        }
    }
    qsort(values, count, sizeof(*values), compare_unsigned);
    uint64_t total = 0;
    for (unsigned i = 0; i < count; ++i)
        if (i == 0 || values[i] != values[i - 1]) total += values[i];
    printf("%" PRIu64 "\n", total);
    free(values);
    return 0;
}
