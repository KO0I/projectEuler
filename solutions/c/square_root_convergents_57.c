/* Project Euler 57: https://projecteuler.net/problem=57
 * Starting at 3/2, advance p/q to (p+2q)/(p+q). Both integers remain
 * exact. Decimal strings avoid GMP sizeinbase's possible one-digit
 * overestimate. Values are below 3^1001, hence below 10^478. */
#include "euler_extra.h"

int main(void) {
    mpz_t numerator, denominator, next;
    mpz_inits(numerator, denominator, next, NULL);
    mpz_set_ui(numerator, 3);
    mpz_set_ui(denominator, 2);
    unsigned count = 0;
    for (unsigned expansion = 1; expansion <= 1000; ++expansion) {
        char n[480], d[480];
        mpz_get_str(n, 10, numerator);
        mpz_get_str(d, 10, denominator);
        count += strlen(n) > strlen(d);
        mpz_mul_ui(next, denominator, 2);
        mpz_add(next, next, numerator);
        mpz_add(denominator, denominator, numerator);
        mpz_set(numerator, next);
    }
    mpz_clears(numerator, denominator, next, NULL);
    printf("%u\n", count);
    return 0;
}
