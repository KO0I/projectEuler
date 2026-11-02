/* Project Euler 145: https://projecteuler.net/problem=145
 * Carry analysis by decimal length gives 20*30^(k-1) for 2k digits,
 * 100*500^k for 4k+3 digits, and zero for 4k+1 digits. */
#include "euler_extra.h"

int main(void) {
    uint64_t total = 0, even = 20;
    for (unsigned digits = 2; digits <= 8; digits += 2) {
        total += even;
        even *= 30;
    }
    uint64_t odd = 100;
    for (unsigned digits = 3; digits <= 7; digits += 4) {
        total += odd;
        odd *= 500;
    }
    printf("%" PRIu64 "\n", total);
    return 0;
}
