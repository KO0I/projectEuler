/* Project Euler 157: https://projecteuler.net/problem=157 */
#include "euler_extra.h"

static unsigned divisors157(uint64_t value) {
    unsigned result = 1;
    for (uint64_t prime = 2; prime * prime <= value; ++prime) {
        if (value % prime) continue;
        unsigned exponent = 0;
        do { value /= prime; ++exponent; } while (value % prime == 0);
        result *= exponent + 1;
    }
    return value > 1 ? result * 2 : result;
}

int main(void) {
    uint64_t answer = 0, ten = 1;
    for (unsigned n = 1; n <= 9; ++n) {
        ten *= 10;
        uint64_t two = 1;
        for (unsigned a = 0; a <= n; ++a, two *= 2) {
            uint64_t five = 1;
            for (unsigned b = 0; b <= n; ++b, five *= 5) {
                uint64_t y = two * five;
                answer += divisors157((ten / y) * (y + 1));
                if (a && b)
                    answer += divisors157((ten / y) * (two + five));
            }
        }
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
