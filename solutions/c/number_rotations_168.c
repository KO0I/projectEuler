/* Project Euler 168: Number Rotations
 *
 * Write N=10q+d.  Moving the final digit d to the front of an L-digit number
 * gives d*10^(L-1)+q=mN.  Rearranging yields
 *
 *     q = d(10^L-m)/(10m-1).
 *
 * Thus each length, multiplier, and last digit has at most one candidate.
 * GMP performs the exact division for lengths up to 100; a zero remainder and
 * the bounds 10^(L-2)<=q<10^(L-1) enforce integrality and full length.  Only
 * N mod 100000 is retained in the requested modular sum. */
#include "euler_extra.h"

int main(void) {
    mpz_t power, numerator, quotient, remainder, lower, number;
    mpz_inits(power, numerator, quotient, remainder, lower, number, NULL);
    mpz_set_ui(power, 10);
    unsigned answer = 0;
    for (unsigned digits = 2; digits <= 100; ++digits) {
        mpz_fdiv_q_ui(lower, power, 10);
        for (unsigned multiplier = 1; multiplier <= 9; ++multiplier) {
            unsigned denominator = 10 * multiplier - 1;
            for (unsigned last = 1; last <= 9; ++last) {
                mpz_mul_ui(numerator, power, last);
                mpz_sub_ui(numerator, numerator, multiplier * last);
                mpz_fdiv_qr_ui(quotient, remainder, numerator, denominator);
                if (mpz_sgn(remainder) != 0 || mpz_cmp(quotient, lower) < 0 ||
                    mpz_cmp(quotient, power) >= 0) continue;
                mpz_mul_ui(number, quotient, 10);
                mpz_add_ui(number, number, last);
                answer = (answer + mpz_fdiv_ui(number, 100000)) % 100000;
            }
        }
        mpz_mul_ui(power, power, 10);
    }
    printf("%05u\n", answer);
    mpz_clears(power, numerator, quotient, remainder, lower, number, NULL);
    return 0;
}
