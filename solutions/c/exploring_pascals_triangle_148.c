/* Project Euler 148: https://projecteuler.net/problem=148 */
#include "euler_extra.h"

static uint64_t nondivisible_rows(uint64_t rows) {
    unsigned digits[32], length = 0;
    for (uint64_t value = rows; value; value /= 7) digits[length++] = value % 7;
    uint64_t powers[32] = {1};
    for (unsigned i = 1; i <= length; ++i) powers[i] = powers[i - 1] * 28;
    uint64_t total = 0, prefix = 1;
    for (unsigned pos = length; pos-- > 0;) {
        uint64_t d = digits[pos];
        total += prefix * d * (d + 1) / 2 * powers[pos];
        prefix *= d + 1;
    }
    return total;
}

int main(void) {
    printf("%" PRIu64 "\n", nondivisible_rows(1000000000ULL));
    return 0;
}
