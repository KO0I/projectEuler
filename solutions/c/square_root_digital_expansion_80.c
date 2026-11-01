/* https://projecteuler.net/problem=80
 * For nonsquare n<=100, floor(sqrt(n*10^198)) contains exactly the first
 * 100 decimal digits of sqrt(n), including the integer digit. Integer square
 * root in GMP avoids roundoff at the last requested digit. */
#include "euler_extra.h"

int main(void) {
    mpz_t scale, value, root;
    mpz_inits(scale, value, root, NULL);
    mpz_ui_pow_ui(scale, 10, 198);
    unsigned sum = 0;
    for (unsigned n = 1; n <= 100; ++n) {
        unsigned integer = (unsigned)euler_isqrt(n);
        if (integer * integer == n) continue;
        mpz_mul_ui(value, scale, n); mpz_sqrt(root, value);
        char *digits = mpz_get_str(NULL, 10, root);
        if (strlen(digits) != 100) euler_fail("Unexpected square-root digit count");
        for (unsigned i = 0; i < 100; ++i) sum += (unsigned)(digits[i] - '0');
        free(digits);
    }
    printf("%u\n", sum);
    mpz_clears(scale, value, root, NULL);
    return 0;
}
