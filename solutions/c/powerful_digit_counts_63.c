/* https://projecteuler.net/problem=63
 * Only bases 1..9 can have a^n < 10^n. Compare exact powers with 10^(n-1).
 * Once a^n falls below that threshold, multiplying by a<10 cannot recover. */
#include "euler_extra.h"

int main(void) {
    mpz_t power, lower;
    mpz_inits(power, lower, NULL);
    unsigned count = 0;
    for (unsigned base = 1; base <= 9; ++base) {
        mpz_set_ui(power, base); mpz_set_ui(lower, 1);
        while (mpz_cmp(power, lower) >= 0) {
            ++count;
            mpz_mul_ui(power, power, base); mpz_mul_ui(lower, lower, 10);
        }
    }
    printf("%u\n", count);
    mpz_clears(power, lower, NULL);
    return 0;
}
