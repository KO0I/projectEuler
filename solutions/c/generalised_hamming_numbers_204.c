/* Project Euler 204: https://projecteuler.net/problem=204

   Every type-100 Hamming number has a unique nondecreasing prime
   factorization using primes <=100.  The DFS chooses an exponent for one
   prime, then moves to the next, so each number is counted exactly once. */
#include "euler_extra.h"
static const unsigned primes204[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
static uint64_t count204(unsigned position, uint64_t value) {
    if (position == sizeof(primes204) / sizeof(primes204[0])) return 1;
    uint64_t result = 0;
    do {
        result += count204(position + 1, value);
        if (value > 1000000000ULL / primes204[position]) break;
        value *= primes204[position];
    } while (true);
    return result;
}
int main(void) { printf("%" PRIu64 "\n", count204(0, 1)); return 0; }
