/* Project Euler 26: https://projecteuler.net/problem=26
 * Long division repeats exactly when a remainder repeats. Remainder zero
 * means a terminating decimal, whose recurring cycle has length zero. */
#include "euler_extra.h"

static unsigned cycle_length(unsigned denominator) {
    int first[1000];
    for (unsigned i = 0; i < denominator; ++i) first[i] = -1;
    unsigned remainder = 1 % denominator, position = 0;
    while (remainder && first[remainder] < 0) {
        first[remainder] = (int)position++;
        remainder = 10 * remainder % denominator;
    }
    return remainder ? position - (unsigned)first[remainder] : 0;
}

int main(void) {
    unsigned best = 0, answer = 0;
    for (unsigned d = 2; d < 1000; ++d) {
        unsigned length = cycle_length(d);
        if (length > best) { best = length; answer = d; }
    }
    printf("%u\n", answer);
    return 0;
}
