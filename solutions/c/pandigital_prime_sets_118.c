/* Project Euler 118: https://projecteuler.net/problem=118
 * Count primes for each digit subset by enumerating its permutations.
 * Partition the remaining mask; requiring the next subset to contain
 * its lowest digit counts every unordered set of prime blocks once. */
#include "euler_extra.h"

static uint64_t prime_count[512], memo[512];
static bool known[512];
static void enumerate_primes(unsigned mask, uint64_t value, unsigned digit_sum) {
    if (mask && (value == 2 || value == 3 || value == 5 || value == 7 ||
        (digit_sum % 3 && value % 2 && value % 5 && euler_is_prime(value))))
        ++prime_count[mask];
    for (unsigned digit = 1; digit <= 9; ++digit) {
        unsigned bit = 1U << (digit - 1);
        if (!(mask & bit)) enumerate_primes(mask | bit, 10 * value + digit, digit_sum + digit);
    }
}
static uint64_t partitions(unsigned mask) {
    if (!mask) return 1;
    if (known[mask]) return memo[mask];
    unsigned lowest = mask & (0U - mask);
    uint64_t count = 0;
    for (unsigned subset = mask; subset; subset = (subset - 1) & mask)
        if ((subset & lowest) && prime_count[subset])
            count += prime_count[subset] * partitions(mask ^ subset);
    known[mask] = true; memo[mask] = count;
    return count;
}
int main(void) {
    enumerate_primes(0, 0, 0);
    printf("%" PRIu64 "\n", partitions(511));
    return 0;
}
