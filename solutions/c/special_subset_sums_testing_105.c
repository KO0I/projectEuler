/* Project Euler 105: https://projecteuler.net/problem=105
 * Sorted smallest-(k+1) versus largest-k sums settle the size rule.
 * Two equal subset sums imply equal disjoint sums after cancelling their
 * intersection, so sorting every subset sum settles the other rule. */
#include "euler_extra.h"

static int compare_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}
static bool special(uint64_t *values, unsigned n) {
    qsort(values, n, sizeof *values, compare_u64);
    for (unsigned i = 1; i < n; ++i) if (values[i] == values[i - 1]) return false;
    for (unsigned k = 1; 2 * k + 1 <= n; ++k) {
        uint64_t small = 0, large = 0;
        for (unsigned i = 0; i <= k; ++i) small += values[i];
        for (unsigned i = 0; i < k; ++i) large += values[n - 1 - i];
        if (small <= large) return false;
    }
    uint64_t sums[1U << 12] = {0};
    unsigned count = 1;
    for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < count; ++j) sums[count + j] = sums[j] + values[i];
        count *= 2;
    }
    qsort(sums, count, sizeof *sums, compare_u64);
    for (unsigned i = 1; i < count; ++i) if (sums[i] == sums[i - 1]) return false;
    return true;
}
int main(void) {
    FILE *file = euler_open("tests/data/p105_sets.txt");
    char line[2048];
    unsigned lines = 0;
    uint64_t answer = 0;
    while (fgets(line, sizeof line, file)) {
        uint64_t values[12], sum = 0;
        unsigned count = 0;
        char *p = line;
        for (;;) {
            char *end;
            while (isspace((unsigned char)*p)) ++p;
            if (!isdigit((unsigned char)*p)) euler_fail("Invalid special-sum-set element");
            errno = 0;
            unsigned long value = strtoul(p, &end, 10);
            if (p == end || errno || !value || value > UINT32_MAX || count == 12)
                euler_fail("Invalid special-sum-set element");
            values[count++] = value; sum += value;
            while (isspace((unsigned char)*end)) ++end;
            if (!*end) break;
            if (*end != ',') euler_fail("Invalid special-sum-set separator");
            p = end + 1;
        }
        if (count < 7) euler_fail("Incomplete special-sum set");
        if (special(values, count)) answer += sum;
        ++lines;
    }
    if (lines != 100 || ferror(file)) euler_fail("Incomplete special-sum-set input");
    fclose(file);
    printf("%" PRIu64 "\n", answer);
    return 0;
}
