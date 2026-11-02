/* Shared exact arithmetic and input helpers for Euler problems added through 120. */
#ifndef EULER_EXTRA_H
#define EULER_EXTRA_H

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

static inline void euler_fail(const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}
static inline void *euler_malloc(size_t size) {
    void *p = malloc(size);
    if (!p) euler_fail("Allocation failed");
    return p;
}
static inline void *euler_calloc(size_t count, size_t size) {
    if (size && count > SIZE_MAX / size) euler_fail("Allocation size overflow");
    void *p = calloc(count, size);
    if (!p) euler_fail("Allocation failed");
    return p;
}
static inline FILE *euler_open(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) { perror(path); exit(EXIT_FAILURE); }
    return file;
}
static inline uint64_t euler_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}
static inline uint64_t euler_isqrt(uint64_t n) {
    if (!n) return 0;
    uint64_t root = (uint64_t)sqrtl((long double)n);
    while (root > n / root) --root;
    while (root + 1 <= n / (root + 1)) ++root;
    return root;
}
static inline unsigned char *euler_sieve(size_t limit) {
    if (limit == SIZE_MAX) euler_fail("Sieve size overflow");
    unsigned char *prime = euler_malloc(limit + 1);
    memset(prime, 1, limit + 1);
    prime[0] = 0;
    if (limit >= 1) prime[1] = 0;
    for (size_t p = 2; p <= limit / p; ++p) if (prime[p]) {
        for (size_t m = p * p; m <= limit; m += p) prime[m] = 0;
    }
    return prime;
}
static inline uint64_t euler_mulmod(uint64_t a, uint64_t b, uint64_t m) {
#if defined(__SIZEOF_INT128__)
    return (uint64_t)((__uint128_t)a * b % m);
#else
    uint64_t result = 0;
    a %= m;
    while (b) {
        if (b & 1) result = result >= m - a ? result - (m - a) : result + a;
        b >>= 1;
        if (b) a = a >= m - a ? a - (m - a) : a + a;
    }
    return result;
#endif
}
static inline uint64_t euler_powmod(uint64_t a, uint64_t exponent, uint64_t m) {
    uint64_t result = 1 % m;
    a %= m;
    while (exponent) {
        if (exponent & 1) result = euler_mulmod(result, a, m);
        exponent >>= 1;
        if (exponent) a = euler_mulmod(a, a, m);
    }
    return result;
}
/* Jim Sinclair's seven bases determine primality for every uint64_t.
 * Source: https://miller-rabin.appspot.com/ (2011 seven-base result).
 * A zero reduced witness is skipped, and every other base is tested. */
static inline bool euler_is_prime(uint64_t n) {
    static const unsigned small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    if (n < 2) return false;
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i)
        if (n % small[i] == 0) return n == small[i];
    uint64_t d = n - 1;
    unsigned s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        if (bases[i] % n == 0) continue;
        uint64_t x = euler_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool passed = false;
        for (unsigned r = 1; r < s; ++r) {
            x = euler_mulmod(x, x, n);
            if (x == n - 1) { passed = true; break; }
        }
        if (!passed) return false;
    }
    return true;
}
#endif
