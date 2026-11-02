/* Project Euler 97: https://projecteuler.net/problem=97
 * Modular repeated squaring retains exactly the last ten decimal digits. */
#include "euler_extra.h"
int main(void) {
    const uint64_t modulus = UINT64_C(10000000000);
    uint64_t power = euler_powmod(2, 7830457, modulus);
    uint64_t result = (euler_mulmod(28433, power, modulus) + 1) % modulus;
    printf("%010" PRIu64 "\n", result);
    return 0;
}
