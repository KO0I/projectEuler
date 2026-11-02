#ifndef EULER_COMMON_H
#define EULER_COMMON_H
#include <gmp.h>
#include <stdint.h>

/* Reusable mathematics only; no problem dispatch. */
int prime(uint64_t n);
unsigned gcd(unsigned a, unsigned b);
unsigned count_divisors(uint64_t n);
unsigned divisor_sum(unsigned n);
int palindrome(unsigned n);
unsigned char *sieve(unsigned limit);
long long digit_sum(const mpz_t z);
int namecmp(const void *a, const void *b);
unsigned factorial(unsigned n);
unsigned digit_power_sum(unsigned n, const unsigned table[10]);
int pentagonal(uint64_t n);
unsigned distinct_factors(unsigned n);
#endif
