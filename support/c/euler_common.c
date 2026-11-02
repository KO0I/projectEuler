/* Reusable mathematics for the individual C programs. */
#include "euler_common.h"
#include <ctype.h>
#include <gmp.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int prime(uint64_t n) {
  if (n < 2)
    return 0;
  if (n % 2 == 0)
    return n == 2;
  for (uint64_t d = 3; d <= n / d; d += 2)
    if (n % d == 0)
      return 0;
  return 1;
}

unsigned gcd(unsigned a, unsigned b) {
  while (b) {
    unsigned t = a % b;
    a = b;
    b = t;
  }
  return a;
}

unsigned count_divisors(uint64_t n) {
  unsigned prod = 1;
  for (uint64_t p = 2; p <= n / p; p += (p == 2 ? 1 : 2)) {
    if (n % p)
      continue;
    unsigned exponent = 1;
    do {
      n /= p;
      ++exponent;
    } while (n % p == 0);
    prod *= exponent;
  }
  if (n > 1)
    prod *= 2;
  return prod;
}

unsigned divisor_sum(unsigned n) {
  if (n < 2)
    return 0;
  unsigned sum = 1;
  for (unsigned d = 2; d <= n / d; d++)
    if (n % d == 0) {
      sum += d;
      if (d != n / d)
        sum += n / d;
    }
  return sum;
}

int palindrome(unsigned n) {
  unsigned old = n, rev = 0;
  do {
    rev = rev * 10 + n % 10;
    n /= 10;
  } while (n);
  return rev == old;
}

unsigned char *sieve(unsigned limit) {
  unsigned char *p = malloc(limit + 1);
  if (!p)
    return NULL;
  memset(p, 1, limit + 1);
  p[0] = p[1] = 0;
  for (unsigned i = 2; i <= limit / i; i++)
    if (p[i])
      for (unsigned j = i * i; j <= limit; j += i)
        p[j] = 0;
  return p;
}

long long digit_sum(const mpz_t z) {
  char *s = mpz_get_str(NULL, 10, z);
  if (!s)
    return -1;
  long long total = 0;
  for (char *p = s; *p; p++)
    if (isdigit((unsigned char)*p))
      total += *p - '0';
  free(s);
  return total;
}

int namecmp(const void *a, const void *b) {
  return strcmp(*(const char *const *)a, *(const char *const *)b);
}

unsigned factorial(unsigned n) {
  unsigned f = 1;
  for (unsigned k = 2; k <= n; k++)
    f *= k;
  return f;
}

unsigned digit_power_sum(unsigned n, const unsigned table[10]) {
  unsigned total = 0;
  do {
    total += table[n % 10];
    n /= 10;
  } while (n);
  return total;
}

int pentagonal(uint64_t n) {
  if (!n)
    return 0;
  uint64_t m = 24 * n + 1;
  uint64_t root = (uint64_t)sqrtl((long double)m);
  while ((root + 1) <= m / (root + 1))
    root++;
  while (root > m / root)
    root--;
  return root * root == m && (root + 1) % 6 == 0;
}

unsigned distinct_factors(unsigned n) {
  unsigned count = 0;
  for (unsigned p = 2; p <= n / p; p++) {
    if (n % p)
      continue;
    count++;
    do {
      n /= p;
    } while (n % p == 0);
  }
  if (n > 1)
    count++;
  return count;
}
