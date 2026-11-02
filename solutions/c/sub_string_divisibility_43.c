/* Project Euler 43: https://projecteuler.net/problem=43
 * Extend a 0..9 permutation, checking each three-digit divisibility
 * condition as soon as its final digit is available. */
#include "euler_extra.h"

static uint64_t search(unsigned position, unsigned used, uint64_t value) {
    static const unsigned divisors[] = {2,3,5,7,11,13,17};
    if (position == 10) return value;
    uint64_t sum = 0;
    for (unsigned digit = position ? 0 : 1; digit < 10; ++digit) {
        if (used & (1u << digit)) continue;
        uint64_t next = 10 * value + digit;
        if (position >= 3 && (next % 1000) % divisors[position - 3]) continue;
        sum += search(position + 1, used | (1u << digit), next);
    }
    return sum;
}

int main(void) {
    printf("%" PRIu64 "\n", search(0, 0, 0));
    return 0;
}
