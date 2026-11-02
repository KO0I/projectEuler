#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 241: Perfection Quotients
 *
 * For n=product p^e, sigma(n)/n is the product of sigma(p^e)/p^e.
 * For each possible half-integer target A/2 (A=3,5,...,13), the search keeps
 * the reduced residual Q=(A/2)*n/sigma(n)=u/v.  A solution is reached at Q=1.
 * If an unfinished cofactor m exists, u*m=v*sigma(m), so v divides m.  The
 * smallest prime p in v is therefore forced into m, with exponent at least its
 * valuation in v.  Branching only on that forced prime makes the otherwise
 * enormous factorization search tiny and canonical.
 *
 * A branch stops when Q<1 (future factors only decrease it), or when n*v is
 * already above 10^18 (every completion contains at least v).  Denominators
 * are 64-bit and occasionally have large factors, so deterministic Miller-
 * Rabin plus Pollard rho supplies their smallest prime factor.  All modular
 * products and rational cross-products use unsigned __int128.
 */

typedef uint64_t U64;
typedef unsigned __int128 U128;
static const U64 LIMIT = UINT64_C(1000000000000000000);
static U128 total;

static U64 gcd64(U64 a, U64 b) {
    while (b) { U64 t = a % b; a = b; b = t; }
    return a;
}

static U128 gcd128(U128 a, U128 b) {
    while (b) { U128 t = a % b; a = b; b = t; }
    return a;
}

static U64 mul_mod(U64 a, U64 b, U64 mod) {
    return (U64)((U128)a * b % mod);
}

static U64 pow_mod(U64 a, U64 exponent, U64 mod) {
    U64 result = 1;
    while (exponent) {
        if (exponent & 1U) result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        exponent >>= 1;
    }
    return result;
}

static int is_prime(U64 n) {
    static const U64 small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return 0;
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return 1;
        if (n % small[i] == 0) return 0;
    }
    U64 d = n - 1, s = 0;
    while (!(d & 1U)) { d >>= 1; ++s; }
    static const U64 bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        U64 a = bases[i] % n;
        if (a == 0) continue;
        U64 x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int passed = 0;
        for (U64 r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { passed = 1; break; }
        }
        if (!passed) return 0;
    }
    return 1;
}

static U64 rho(U64 n) {
    if (!(n & 1U)) return 2;
    if (n % 3 == 0) return 3;
    for (U64 c = 1; ; ++c) {
        U64 x = 2, y = 2, divisor = 1;
        while (divisor == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            U64 difference = x > y ? x - y : y - x;
            divisor = gcd64(difference, n);
        }
        if (divisor != n) return divisor;
    }
}

static U64 smallest_prime_factor(U64 n) {
    if (is_prime(n)) return n;
    U64 factor = rho(n);
    U64 left = smallest_prime_factor(factor);
    U64 right = smallest_prime_factor(n / factor);
    return left < right ? left : right;
}

static int already_used(U64 p, const U64 *used, int depth) {
    for (int i = 0; i < depth; ++i)
        if (used[i] == p) return 1;
    return 0;
}

static void search(U64 n, U64 u, U64 v, U64 *used, int depth) {
    if (u == v) { total += n; return; }
    if (u < v || v == 1 || (U128)n * v > LIMIT) return;

    const U64 p = smallest_prime_factor(v);
    if (already_used(p, used, depth)) return;
    int minimum_exponent = 0;
    for (U64 copy = v; copy % p == 0; copy /= p)
        ++minimum_exponent;
    used[depth] = p;

    U64 power = 1;
    U64 sigma = 1;
    for (int exponent = 1; power <= LIMIT / p / n; ++exponent) {
        power *= p;
        if (sigma > UINT64_MAX - power) break;
        sigma += power;
        if (exponent < minimum_exponent) continue;

        U128 new_u = (U128)u * power;
        U128 new_v = (U128)v * sigma;
        U128 reduction = gcd128(new_u, new_v);
        new_u /= reduction;
        new_v /= reduction;
        if (new_u > UINT64_MAX || new_v > UINT64_MAX) continue;
        if (new_u < new_v) break; /* larger e only decreases the residual */
        search(n * power, (U64)new_u, (U64)new_v, used, depth + 1);
    }
}

static void print_wide(U128 value) {
    char digits[64]; int length = 0;
    do { digits[length++] = (char)('0' + value % 10); value /= 10; }
    while (value);
    while (length) putchar(digits[--length]);
    putchar('\n');
}

int main(void) {
    U64 used[64];
    for (U64 numerator = 3; numerator <= 13; numerator += 2)
        search(1, numerator, 2, used, 0);
    print_wide(total);
    return 0;
}
