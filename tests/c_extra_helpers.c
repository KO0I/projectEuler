/* Arithmetic boundaries, exhaustive small primality, and a GMP modular oracle. */
#include "euler_extra.h"
#include <assert.h>

int main(void) {
    assert(euler_gcd(0, 0) == 0);
    assert(euler_gcd(54, 24) == 6);
    assert(euler_isqrt(0) == 0);
    assert(euler_isqrt(1) == 1);
    assert(euler_isqrt(15) == 3);
    assert(euler_isqrt(16) == 4);
    assert(euler_isqrt(17) == 4);
    assert(euler_isqrt(UINT64_MAX) == UINT64_C(4294967295));
    unsigned char *sieve = euler_sieve(100000);
    for (unsigned n = 0; n <= 100000; ++n) assert(euler_is_prime(n) == (sieve[n] != 0));
    free(sieve);
    assert(!euler_is_prime(UINT64_C(341550071728321)));
    assert(!euler_is_prime(UINT64_C(3825123056546413051)));
    assert(!euler_is_prime(UINT64_MAX));
    assert(euler_is_prime(UINT64_C(18446744073709551557)));
    mpz_t a, b, modulus, result, expected;
    mpz_inits(a, b, modulus, result, expected, NULL);
    uint64_t state = UINT64_C(88272645463325252);
    for (unsigned i = 0; i < 200; ++i) {
        state ^= state << 13; state ^= state >> 7; state ^= state << 17;
        uint64_t x = state;
        state ^= state << 13; state ^= state >> 7; state ^= state << 17;
        uint64_t y = state;
        uint64_t m = (UINT64_MAX - 2 * i) | 1;
        uint64_t got = euler_mulmod(x, y, m);
        mpz_import(a, 1, 1, sizeof x, 0, 0, &x);
        mpz_import(b, 1, 1, sizeof y, 0, 0, &y);
        mpz_import(modulus, 1, 1, sizeof m, 0, 0, &m);
        mpz_import(result, 1, 1, sizeof got, 0, 0, &got);
        mpz_mul(expected, a, b); mpz_mod(expected, expected, modulus);
        assert(mpz_cmp(result, expected) == 0);
    }
    mpz_clears(a, b, modulus, result, expected, NULL);
    puts("Extra C helpers: all checks passed");
    return 0;
}
