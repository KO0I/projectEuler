/* Project Euler 160: Factorial Trailing Digits
 *
 * Every trailing zero is a paired factor 2*5.  The range reduction uses the
 * periodicity of the non-five residues modulo powers of ten, shrinking 10^12
 * to a tractable block.  During multiplication, each factor of five is removed
 * and matched by dividing an already accumulated factor of two.
 *
 * Keeping nine low digits provides guard digits while cancellations occur;
 * only the final five nonzero digits are printed.  The multiplication reduces
 * every step, so uint64_t safely contains its intermediate values. */
#include "euler_extra.h"

int main(void) {
    uint64_t limit = 1000000000000ULL;
    while (limit % 100000 == 0) limit /= 5;
    uint64_t result = 1;
    for (uint64_t value = 1; value <= limit; ++value) {
        uint64_t current = value;
        while (current % 5 == 0) {
            current /= 5;
            result /= 2;
        }
        result *= current % 100000;
        result %= 1000000000ULL;
    }
    printf("%05" PRIu64 "\n", result % 100000);
    return 0;
}
