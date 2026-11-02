/* Project Euler 134: https://projecteuler.net/problem=134 */
#include "euler_extra.h"

static uint64_t inverse_mod(uint64_t a, uint64_t modulus) {
    int64_t old_r = (int64_t)a, r = (int64_t)modulus;
    int64_t old_s = 1, s = 0;
    while (r) {
        int64_t q = old_r / r;
        int64_t next = old_r - q * r; old_r = r; r = next;
        next = old_s - q * s; old_s = s; s = next;
    }
    if (old_s < 0) old_s += (int64_t)modulus;
    return (uint64_t)old_s;
}

int main(void) {
    uint64_t total = 0;
    unsigned p1 = 5;
    for (unsigned candidate = 7; p1 <= 1000000; candidate += 2) {
        if (!euler_is_prime(candidate)) continue;
        unsigned p2 = candidate;
        uint64_t scale = 10;
        while (scale <= p1) scale *= 10;
        uint64_t multiplier = ((p2 - p1 % p2) * inverse_mod(scale % p2, p2)) % p2;
        total += multiplier * scale + p1;
        p1 = p2;
    }
    printf("%" PRIu64 "\n", total);
    return 0;
}
