#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project Euler 229: four representations using squares.
 *
 * Temporarily allowing zero coefficients, simultaneous representability by
 * x^2+d*y^2 for d=1,2,3,7 depends only on the squarefree kernel q of n.  Every
 * prime of q must be congruent to 1, 25, or 121 modulo 168.  Each admissible
 * kernel contributes floor(sqrt(L/q)) numbers q*m^2.  A bit-packed odd sieve
 * finds the good primes through L=2e9, and a DFS enumerates products of two or
 * more distinct good primes; only primes <=L/193 can enter such products.
 *
 * The relaxation incorrectly accepts every perfect square via y=0.  We remove
 * all squares, then add back squares m^2 having genuinely positive forms for
 * every d.  Factoring d*b^2 and its divisor pairs u*v=d*b^2 reconstructs
 * m=(u+v)/2 and a=(v-u)/2, making that correction tiny (m<=44721).
 */

enum { LIMIT = 2000000000, ROOT_LIMIT = 44721, MAX_FACTORS = 12, MAX_DIVISORS = 4096 };

typedef struct { uint32_t prime; int exponent; } Factor;

static unsigned char *odd_composite;
static uint32_t *good_primes;
static int good_count;

static int uint32_compare(const void *left, const void *right) {
    const uint32_t a = *(const uint32_t *)left;
    const uint32_t b = *(const uint32_t *)right;
    return (a > b) - (a < b);
}

static uint64_t integer_sqrt(uint64_t value) {
    uint64_t root = (uint64_t)sqrtl((long double)value);
    while ((root + 1) * (root + 1) <= value) ++root;
    while (root * root > value) --root;
    return root;
}

static int is_composite_odd(uint64_t value) {
    const uint64_t index = value >> 1;
    return (odd_composite[index >> 3] >> (index & 7)) & 1U;
}

static void mark_composite_odd(uint64_t value) {
    const uint64_t index = value >> 1;
    odd_composite[index >> 3] |= (unsigned char)(1U << (index & 7));
}

static uint64_t multi_kernel_sum(int start, uint64_t product, int depth) {
    uint64_t sum = 0;
    for (int i = start; i < good_count; ++i) {
        const uint64_t prime = good_primes[i];
        if (prime > LIMIT / product) break;
        const uint64_t next = product * prime;
        if (depth + 1 >= 2) sum += integer_sqrt(LIMIT / next);
        sum += multi_kernel_sum(i + 1, next, depth + 1);
    }
    return sum;
}

static void add_factor(Factor *factors, int *count, uint32_t prime, int exponent) {
    for (int i = 0; i < *count; ++i) {
        if (factors[i].prime == prime) {
            factors[i].exponent += exponent;
            return;
        }
    }
    factors[(*count)++] = (Factor){prime, exponent};
}

static void factor_small(uint32_t value, const uint32_t *spf, Factor *factors,
                         int *factor_count, int multiplier) {
    while (value > 1) {
        const uint32_t prime = spf[value] == 0 ? value : spf[value];
        int exponent = 0;
        do { value /= prime; ++exponent; } while (value % prime == 0);
        add_factor(factors, factor_count, prime, multiplier * exponent);
    }
}

int main(void) {
    const uint64_t odd_count = (uint64_t)LIMIT / 2 + 1;
    odd_composite = calloc((size_t)((odd_count + 7) / 8), 1);
    if (odd_composite == NULL) return 1;
    for (uint64_t p = 3; p * p <= LIMIT; p += 2) {
        if (!is_composite_odd(p)) {
            for (uint64_t multiple = p * p; multiple <= LIMIT; multiple += 2 * p) {
                mark_composite_odd(multiple);
            }
        }
    }

    size_t capacity = 65536;
    good_primes = malloc(capacity * sizeof(*good_primes));
    if (good_primes == NULL) { free(odd_composite); return 1; }
    uint64_t relaxed = integer_sqrt(LIMIT); /* Empty kernel q=1. */
    static const int residues[3] = {1, 25, 121};
    for (int ri = 0; ri < 3; ++ri) {
        uint64_t value = (uint64_t)residues[ri];
        if (value < 193) value += ((193 - value + 167) / 168) * 168;
        for (; value <= LIMIT; value += 168) {
            if (!is_composite_odd(value)) {
                relaxed += integer_sqrt(LIMIT / value);
                if (value <= LIMIT / 193) {
                    if ((size_t)good_count == capacity) {
                        capacity *= 2;
                        uint32_t *grown = realloc(good_primes, capacity * sizeof(*grown));
                        if (grown == NULL) return 1;
                        good_primes = grown;
                    }
                    good_primes[good_count++] = (uint32_t)value;
                }
            }
        }
    }
    free(odd_composite);
    qsort(good_primes, (size_t)good_count, sizeof(*good_primes), uint32_compare);
    relaxed += multi_kernel_sum(0, 1, 0);
    free(good_primes);

    uint32_t *spf = calloc(ROOT_LIMIT + 1, sizeof(*spf));
    if (spf == NULL) return 1;
    for (uint32_t p = 2; (uint64_t)p * p <= ROOT_LIMIT; ++p) {
        if (spf[p] == 0) {
            for (uint32_t multiple = p * p; multiple <= ROOT_LIMIT; multiple += p) {
                if (spf[multiple] == 0) spf[multiple] = p;
            }
        }
    }

    unsigned char square_forms[4][ROOT_LIMIT + 1];
    memset(square_forms, 0, sizeof(square_forms));
    static const int coefficients[4] = {1, 2, 3, 7};
    for (int di = 0; di < 4; ++di) {
        const uint64_t d = (uint64_t)coefficients[di];
        const uint32_t maximum_b = (uint32_t)integer_sqrt(LIMIT / d);
        for (uint32_t b = 1; b <= maximum_b; ++b) {
            Factor factors[MAX_FACTORS];
            int factor_count = 0;
            factor_small((uint32_t)d, spf, factors, &factor_count, 1);
            factor_small(b, spf, factors, &factor_count, 2);
            uint64_t divisors[MAX_DIVISORS];
            int divisor_count = 1;
            divisors[0] = 1;
            for (int fi = 0; fi < factor_count; ++fi) {
                const int old_count = divisor_count;
                uint64_t power = 1;
                for (int e = 1; e <= factors[fi].exponent; ++e) {
                    power *= factors[fi].prime;
                    for (int i = 0; i < old_count; ++i) {
                        if (divisor_count >= MAX_DIVISORS) return 1;
                        divisors[divisor_count++] = divisors[i] * power;
                    }
                }
            }
            const uint64_t product = d * b * b;
            for (int i = 0; i < divisor_count; ++i) {
                const uint64_t u = divisors[i];
                const uint64_t v = product / u;
                if (u >= v || ((u ^ v) & 1U) != 0U) continue;
                const uint64_t m = (u + v) / 2;
                if (m <= ROOT_LIMIT) square_forms[di][m] = 1;
            }
        }
    }
    free(spf);

    uint64_t valid_squares = 0;
    for (int m = 1; m <= ROOT_LIMIT; ++m) {
        valid_squares += square_forms[0][m] && square_forms[1][m] &&
                         square_forms[2][m] && square_forms[3][m];
    }
    printf("%llu\n", (unsigned long long)(relaxed - ROOT_LIMIT + valid_squares));
    return 0;
}
