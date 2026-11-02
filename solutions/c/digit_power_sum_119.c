/* Project Euler 119: https://projecteuler.net/problem=119
 * Below 10^d a digit sum is at most 9*d. Enumerate every power of every
 * such base, sort and deduplicate, then enlarge d until 30 values exist.
 * This decimal cutoff proves that no smaller qualifying term is missed. */
#include "euler_extra.h"
static unsigned digit_sum(uint64_t n) {
    unsigned sum = 0;
    do { sum += n % 10; n /= 10; } while (n);
    return sum;
}
static int compare_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}
int main(void) {
    /* At most 171 bases and 64 powers per base for any uint64_t cutoff. */
    uint64_t values[171 * 64], limit = 100;
    for (unsigned digits = 2; digits <= 19; ++digits) {
        size_t count = 0;
        for (uint64_t base = 2; base <= 9 * digits; ++base) {
            for (uint64_t value = base * base; value < limit; ) {
                if (value >= 10 && digit_sum(value) == base) values[count++] = value;
                if (value > (limit - 1) / base) break;
                value *= base;
            }
        }
        qsort(values, count, sizeof values[0], compare_u64);
        size_t unique = 0;
        for (size_t i = 0; i < count; ++i)
            if (i == 0 || values[i] != values[i - 1]) values[unique++] = values[i];
        if (unique >= 30) { printf("%" PRIu64 "\n", values[29]); return 0; }
        if (limit > UINT64_MAX / 10) break;
        limit *= 10;
    }
    euler_fail("No 30th term within the complete uint64_t decimal ranges");
    return 1;
}
