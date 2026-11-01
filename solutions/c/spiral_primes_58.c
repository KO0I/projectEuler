/* Project Euler 58: https://projecteuler.net/problem=58
 * A side-s layer has corners s^2-k(s-1). Its square corner is composite;
 * test the other three and compare the ratio using integer arithmetic. */
#include "euler_extra.h"

int main(void) {
    uint64_t prime_count = 0;
    for (uint64_t side = 3; ; side += 2) {
        if (side > UINT64_MAX / side) euler_fail("Spiral search overflow");
        uint64_t square = side * side, step = side - 1;
        for (unsigned corner = 1; corner <= 3; ++corner)
            prime_count += euler_is_prime(square - corner * step);
        if (10 * prime_count < 2 * side - 1) {
            printf("%" PRIu64 "\n", side);
            return 0;
        }
    }
}
