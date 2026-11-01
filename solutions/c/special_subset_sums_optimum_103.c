/* Project Euler 103: https://projecteuler.net/problem=103
 * The stated optimum six-element set gives a valid seven-element upper
 * bound by the suggested middle-element construction. Then enumerate
 * ALL increasing positive seven-element sets of smaller total, not a
 * neighbourhood of that construction. Unique subset sums and necessary
 * smallest-(k+1) versus largest-k inequalities prune the finite search. */
#include "euler_extra.h"

enum { SIZE = 7, SUM_BOUND = 512, MASK_WORDS = SUM_BOUND / 64 };
static unsigned current[SIZE], best[SIZE], best_sum;

static bool cardinality_possible(unsigned chosen) {
    for (unsigned k = 1; 2 * k + 1 <= SIZE; ++k) {
        if (chosen < k + 1) continue;
        unsigned small = 0, large = 0;
        for (unsigned i = 0; i <= k; ++i) small += current[i];
        /* Unchosen entries take their smallest possible increasing values. */
        for (unsigned i = SIZE - k; i < SIZE; ++i)
            large += i < chosen ? current[i] : current[chosen - 1] + i - chosen + 1;
        if (small <= large) return false;
    }
    return true;
}
static void search(unsigned chosen, unsigned total, const unsigned *sums,
                   const uint64_t *seen) {
    if (chosen == SIZE) {
        best_sum = total;
        memcpy(best, current, sizeof best);
        return;
    }
    unsigned remaining = SIZE - chosen, count = 1U << chosen;
    unsigned first = chosen ? current[chosen - 1] + 1 : 1;
    for (unsigned value = first; ; ++value) {
        unsigned minimum_total = total + remaining * value + remaining * (remaining - 1) / 2;
        if (minimum_total >= best_sum) break;
        current[chosen] = value;
        if (!cardinality_possible(chosen + 1)) continue;
        bool collision = false;
        for (unsigned i = 0; i < count; ++i) {
            unsigned sum = sums[i] + value;
            if (seen[sum / 64] & (UINT64_C(1) << (sum % 64))) {
                collision = true; break;
            }
        }
        if (collision) continue;
        unsigned next_sums[1U << SIZE];
        uint64_t next_seen[MASK_WORDS];
        memcpy(next_sums, sums, count * sizeof *sums);
        memcpy(next_seen, seen, sizeof next_seen);
        for (unsigned i = 0; i < count; ++i) {
            unsigned sum = sums[i] + value;
            next_sums[count + i] = sum;
            next_seen[sum / 64] |= UINT64_C(1) << (sum % 64);
        }
        search(chosen + 1, total + value, next_sums, next_seen);
    }
}
static bool valid_seed(void) {
    memcpy(current, best, sizeof best);
    if (!cardinality_possible(SIZE)) return false;
    bool seen[SUM_BOUND] = {false};
    for (unsigned mask = 0; mask < (1U << SIZE); ++mask) {
        unsigned sum = 0;
        for (unsigned i = 0; i < SIZE; ++i) if (mask & (1U << i)) sum += best[i];
        if (sum >= SUM_BOUND || seen[sum]) return false;
        seen[sum] = true;
    }
    return true;
}
int main(void) {
    /* This six-element set is supplied by the problem statement. */
    const unsigned previous[] = {11, 18, 19, 20, 22, 25};
    unsigned middle = previous[3];
    best[0] = middle; best_sum = middle;
    for (unsigned i = 0; i < 6; ++i) {
        best[i + 1] = middle + previous[i];
        best_sum += best[i + 1];
    }
    if (!valid_seed()) euler_fail("The constructed search bound is not a special sum set");
    unsigned sums[1] = {0};
    uint64_t seen[MASK_WORDS] = {1};
    search(0, 0, sums, seen);
    for (unsigned i = 0; i < SIZE; ++i) printf("%u", best[i]);
    putchar('\n');
    return 0;
}
