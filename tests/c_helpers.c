/* Exercise internal C helpers without rerunning complete Euler solutions. */
#include <assert.h>
#include <stdio.h>
#include "../support/c/euler_common.c"
int main(void) {
    assert(!prime(0) && !prime(1) && prime(2) && prime(97) && !prime(49));
    assert(gcd(54, 24) == 6 && gcd(0, 7) == 7);
    assert(count_divisors(1) == 1 && count_divisors(28) == 6);
    assert(divisor_sum(1) == 0 && divisor_sum(220) == 284 && divisor_sum(284) == 220);
    assert(number_letters(342) == 23 && number_letters(115) == 20 && number_letters(1000) == 11);
    assert(!leap(1900) && leap(2000) && leap(2004) && !leap(2001));
    assert(factorial(0) == 1 && factorial(9) == 362880);
    assert(!pentagonal(0) && pentagonal(1) && pentagonal(5) && pentagonal(12) && !pentagonal(2));
    assert(distinct_factors(60) == 3 && distinct_factors(64) == 1);
    unsigned char *p = sieve(100);
    assert(p && !p[0] && !p[1] && p[2] && p[97] && !p[99]);
    free(p);
    mpz_t n;
    mpz_init(n);
    mpz_ui_pow_ui(n, 2, 1000);
    assert(digit_sum(n) == 1366);
    mpz_fac_ui(n, 100);
    assert(digit_sum(n) == 648);
    mpz_clear(n);
    puts("C helper regression tests passed.");
    return 0;
}
