/* Project Euler 41: https://projecteuler.net/problem=41
 * Enumerate permutations in decreasing numerical order. Digit lengths
 * whose triangular digit sum is divisible by three cannot give a prime. */
#include "euler_extra.h"

static uint64_t descending(unsigned length, unsigned remaining,
                           unsigned used, uint64_t value) {
    if (!remaining) return euler_is_prime(value) ? value : 0;
    for (unsigned digit = length; digit; --digit)
        if (!(used & (1u << digit))) {
            uint64_t answer = descending(length, remaining - 1,
                                        used | (1u << digit), 10 * value + digit);
            if (answer) return answer;
        }
    return 0;
}

int main(void) {
    for (unsigned length = 9; length; --length) {
        if ((length * (length + 1) / 2) % 3 == 0) continue;
        uint64_t answer = descending(length, length, 0, 0);
        if (answer) { printf("%" PRIu64 "\n", answer); return 0; }
    }
    euler_fail("No pandigital prime found");
    return 1;
}
