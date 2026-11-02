/* Project Euler 156: Counting Digits
 *
 * `occurrences156(n,d)` evaluates f(n,d) position by position.  At decimal
 * place p, complete high cycles contribute high*p occurrences, followed by a
 * partial cycle determined by the current digit and lower suffix.
 *
 * To find fixed points f(n,d)=n, recursively inspect an interval [lo,hi].
 * Monotonicity gives a rigorous rejection: if f(lo)>hi or f(hi)<lo, its graph
 * cannot meet the diagonal inside.  Remaining intervals are bisected down to
 * singletons.  Digits 1..9 avoid the leading-zero complications of d=0, and
 * all counts and the final sum require uint64_t. */
#include "euler_extra.h"

static uint64_t occurrences156(uint64_t n, unsigned digit) {
    uint64_t count = 0;
    for (uint64_t place = 1; place <= n;) {
        uint64_t lower = n % place;
        uint64_t current = (n / place) % 10;
        uint64_t higher = n / place / 10;
        count += higher * place;
        if (current > digit) count += place;
        else if (current == digit) count += lower + 1;
        if (place > UINT64_MAX / 10) break;
        place *= 10;
    }
    return count;
}

static uint64_t search156(uint64_t low, uint64_t high, unsigned digit) {
    uint64_t at_low = occurrences156(low, digit);
    uint64_t at_high = occurrences156(high, digit);
    if (at_low > high || at_high < low) return 0;
    if (low == high) return at_low == low ? low : 0;
    uint64_t middle = low + (high - low) / 2;
    return search156(low, middle, digit) +
           search156(middle + 1, high, digit);
}

int main(void) {
    uint64_t answer = 0;
    for (unsigned digit = 1; digit <= 9; ++digit)
        answer += search156(0, 1000000000000ULL, digit);
    printf("%" PRIu64 "\n", answer);
    return 0;
}
