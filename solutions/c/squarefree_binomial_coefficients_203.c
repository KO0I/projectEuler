/* Project Euler 203: https://projecteuler.net/problem=203

   Generate Pascal rows 0..50 exactly in 64 bits, collect all coefficients,
   sort away duplicates, and reject a value if any prime square divides it.
   Trial division only reaches sqrt(C(50,25)), which is tiny for this set. */
#include "euler_extra.h"
static int compare203(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : x > y;
}
static bool squarefree203(uint64_t value) {
    for (uint64_t p = 2; p * p <= value; ++p)
        if (value % (p * p) == 0) return false;
    return true;
}
int main(void) {
    uint64_t values[1326]; size_t count = 0;
    for (unsigned n = 0; n <= 50; ++n) {
        uint64_t coefficient = 1;
        for (unsigned k = 0; k <= n; ++k) {
            values[count++] = coefficient;
            coefficient = k == n ? 1 : coefficient * (n - k) / (k + 1);
        }
    }
    qsort(values, count, sizeof(*values), compare203);
    uint64_t answer = 0;
    for (size_t i = 0; i < count; ++i)
        if ((!i || values[i] != values[i - 1]) && squarefree203(values[i])) answer += values[i];
    printf("%" PRIu64 "\n", answer);
    return 0;
}
