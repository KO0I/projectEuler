/* https://projecteuler.net/problem=66
 * The fundamental positive solution of x^2-D*y^2=1 occurs among the continued
 * fraction convergents of sqrt(D). Test that identity with arbitrary integers;
 * both x and y can exceed machine integer range long before D reaches 1000. */
#include "euler_extra.h"

static void p66_minimum_x(unsigned n, mpz_t answer) {
    unsigned root = (unsigned)euler_isqrt(n), m = 0, d = 1, a = root;
    mpz_t p0, p1, p, q0, q1, q, difference, temporary;
    mpz_inits(p0, p1, p, q0, q1, q, difference, temporary, NULL);
    mpz_set_ui(p1, 1); mpz_set_ui(q0, 1);
    for (;;) {
        mpz_mul_ui(p, p1, a); mpz_add(p, p, p0);
        mpz_mul_ui(q, q1, a); mpz_add(q, q, q0);
        mpz_mul(difference, p, p);
        mpz_mul(temporary, q, q); mpz_mul_ui(temporary, temporary, n);
        mpz_sub(difference, difference, temporary);
        if (mpz_cmp_ui(difference, 1) == 0) { mpz_set(answer, p); break; }
        mpz_set(p0, p1); mpz_set(p1, p); mpz_set(q0, q1); mpz_set(q1, q);
        m = d * a - m; d = (n - m * m) / d; a = (root + m) / d;
    }
    mpz_clears(p0, p1, p, q0, q1, q, difference, temporary, NULL);
}

int main(void) {
    mpz_t x, largest;
    mpz_inits(x, largest, NULL);
    unsigned answer = 0;
    for (unsigned d = 2; d <= 1000; ++d) {
        unsigned root = (unsigned)euler_isqrt(d);
        if (root * root == d) continue;
        p66_minimum_x(d, x);
        if (mpz_cmp(x, largest) > 0) { mpz_set(largest, x); answer = d; }
    }
    printf("%u\n", answer);
    mpz_clears(x, largest, NULL);
    return 0;
}
