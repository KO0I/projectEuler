/* Project Euler 170: Pandigital Concatenating Products
 *
 * Generate 0-to-9 pandigital result strings from largest to smallest.  For
 * every split into left and right products, any common multiplicand must divide
 * gcd(left,right), so only divisors of that gcd need testing.  Dividing both
 * halves by a candidate produces the two multiplicands.
 *
 * `append170` verifies that the common factor and both quotients together use
 * each decimal digit exactly once.  Because result permutations are visited in
 * descending lexicographic order, the first successful split is globally
 * maximal and the recursion can stop immediately. */
#include "euler_extra.h"

static uint64_t answer170;
static unsigned digits170[10];

static uint64_t gcd170(uint64_t a, uint64_t b) {
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

static bool append170(uint64_t value, unsigned *mask, unsigned *length) {
    unsigned local[20], count = 0;
    do { local[count++] = value % 10; value /= 10; } while (value);
    for (unsigned i = count; i-- > 0;) {
        unsigned bit = 1u << local[i];
        if (*mask & bit) return false;
        *mask |= bit;
        ++*length;
    }
    return true;
}

static bool factor_works170(uint64_t factor, uint64_t left, uint64_t right) {
    if (factor <= 1 || left % factor || right % factor) return false;
    unsigned mask = 0, length = 0;
    return append170(factor, &mask, &length) &&
           append170(left / factor, &mask, &length) &&
           append170(right / factor, &mask, &length) &&
           length == 10 && mask == 1023;
}

static bool inspect170(void) {
    uint64_t whole = 0;
    for (unsigned i = 0; i < 10; ++i) whole = whole * 10 + digits170[i];
    uint64_t left = 0;
    for (unsigned cut = 1; cut < 10; ++cut) {
        left = left * 10 + digits170[cut - 1];
        if (digits170[cut] == 0) continue;
        uint64_t power = 1;
        for (unsigned i = cut; i < 10; ++i) power *= 10;
        uint64_t right = whole % power;
        uint64_t common = gcd170(left, right);
        for (uint64_t divisor = 2; divisor * divisor <= common; ++divisor) {
            if (common % divisor) continue;
            if (factor_works170(divisor, left, right) ||
                factor_works170(common / divisor, left, right)) {
                answer170 = whole;
                return true;
            }
        }
        if (factor_works170(common, left, right)) {
            answer170 = whole;
            return true;
        }
    }
    return false;
}

static bool search170(unsigned position, unsigned used) {
    if (position == 10) return inspect170();
    for (int digit = 9; digit >= 0; --digit) {
        if (used & (1u << digit) || (position == 0 && digit == 0)) continue;
        digits170[position] = digit;
        if (search170(position + 1, used | (1u << digit))) return true;
    }
    return false;
}

int main(void) {
    search170(0, 0);
    printf("%" PRIu64 "\n", answer170);
    return 0;
}
