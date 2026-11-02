/* Project Euler 128: https://projecteuler.net/problem=128 */
#include "euler_extra.h"

int main(void) {
    unsigned found = 1;
    uint64_t answer = 1;
    for (uint64_t n = 1; found < 2000; ++n) {
        if (euler_is_prime(6 * n - 1) && euler_is_prime(6 * n + 1) &&
            euler_is_prime(12 * n + 5)) {
            answer = 3 * n * (n - 1) + 2;
            ++found;
            if (found == 2000) break;
        }
        if (n > 1 && euler_is_prime(6 * n - 1) && euler_is_prime(6 * n + 5) &&
            euler_is_prime(12 * n - 7)) {
            answer = 3 * n * (n + 1) + 1;
            ++found;
        }
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
