/* https://projecteuler.net/problem=65
 * Numerators obey p[k]=a[k]*p[k-1]+p[k-2], starting with 1 and 0.
 * e has coefficients 2; 1,2,1; 1,4,1; 1,6,1; ... . GMP keeps them exact. */
#include "euler_extra.h"

int main(void) {
    mpz_t previous, before, next;
    mpz_inits(previous, before, next, NULL);
    mpz_set_ui(previous, 1);
    for (unsigned k = 0; k < 100; ++k) {
        unsigned coefficient = k == 0 ? 2 : (k % 3 == 2 ? 2 * ((k + 1) / 3) : 1);
        mpz_mul_ui(next, previous, coefficient);
        mpz_add(next, next, before);
        mpz_set(before, previous); mpz_set(previous, next);
    }
    char *decimal = mpz_get_str(NULL, 10, previous);
    unsigned sum = 0;
    for (const char *s = decimal; *s; ++s) sum += (unsigned)(*s - '0');
    printf("%u\n", sum);
    free(decimal); mpz_clears(previous, before, next, NULL);
    return 0;
}
