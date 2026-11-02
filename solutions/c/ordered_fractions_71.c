/* https://projecteuler.net/problem=71
 * For each denominator, floor((3*d-1)/7) is the largest numerator strictly
 * left of 3/7. Keep reduced fractions and compare by exact cross products. */
#include "euler_extra.h"

static uint64_t p71_numerator(unsigned limit) {
    uint64_t best_n = 0, best_d = 1;
    for (uint64_t denominator = 2; denominator <= limit; ++denominator) {
        uint64_t numerator = (3 * denominator - 1) / 7;
        if (euler_gcd(numerator, denominator) == 1 && numerator * best_d > best_n * denominator) {
            best_n = numerator; best_d = denominator;
        }
    }
    return best_n;
}

int main(void) {
    printf("%" PRIu64 "\n", p71_numerator(1000000));
    return 0;
}
