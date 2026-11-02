/* Project Euler 56: https://projecteuler.net/problem=56
 * Compute every a^b with 1 <= a,b < 100 exactly with GMP. At most 198
 * decimal digits are needed, since 99^99 < 100^99. */
#include "euler_extra.h"

int main(void) {
    mpz_t value;
    mpz_init(value);
    unsigned best = 0;
    for (unsigned a = 1; a < 100; ++a)
        for (unsigned b = 1; b < 100; ++b) {
            mpz_ui_pow_ui(value, a, b);
            char decimal[200];
            mpz_get_str(decimal, 10, value);
            unsigned sum = 0;
            for (const char *p = decimal; *p; ++p) sum += (unsigned)(*p - '0');
            if (sum > best) best = sum;
        }
    mpz_clear(value);
    printf("%u\n", best);
    return 0;
}
