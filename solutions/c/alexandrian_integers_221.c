#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 221: Alexandrian integers.
 *
 * The defining equation is equivalent, after choosing the negative member of
 * the triple as -p, to
 *
 *       (q-p)(r-p)=p^2+1.
 *
 * Thus every divisor pair d*e=p^2+1 produces A=p(p+d)(p+e), and taking only
 * d<=e removes the mirror duplicate.  Candidates do not arrive sorted and
 * equal A values can arise from different p, so they are sorted and uniqued.
 *
 * To factor p^2+1 for all p<=500,000 efficiently, sieve the polynomial.  A
 * prime l divides it only on roots of x^2=-1 mod l (l=2 or l==1 mod 4).
 * Divide each root progression and record its exponent; the final residual
 * is prime.  This avoids trial-dividing every p by every smaller prime.
 * Finally, (P+1)(P+2)^2 is a lower bound for all candidates from future p;
 * the program checks this against the selected answer before printing it.
 */

enum { P_LIMIT = 500000, FACTOR_SLOTS = 12, DIVISOR_SLOTS = 8192, TARGET = 150000 };

typedef struct {
    uint32_t prime;
    unsigned char exponent;
} Factor;

static uint64_t power_mod(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    while (exponent != 0) {
        if ((exponent & 1U) != 0U) result = result * base % modulus;
        base = base * base % modulus;
        exponent >>= 1U;
    }
    return result;
}

static uint64_t sqrt_minus_one(uint64_t prime) {
    /* Any quadratic non-residue g raised to (p-1)/4 squares to -1. */
    uint64_t g = 2;
    while (power_mod(g, (prime - 1) / 2, prime) != prime - 1) ++g;
    return power_mod(g, (prime - 1) / 4, prime);
}

static int uint64_compare(const void *left, const void *right) {
    const uint64_t a = *(const uint64_t *)left;
    const uint64_t b = *(const uint64_t *)right;
    return (a > b) - (a < b);
}

int main(void) {
    unsigned char *composite = calloc(P_LIMIT + 1, 1);
    uint64_t *residual = malloc((P_LIMIT + 1) * sizeof(*residual));
    Factor (*factors)[FACTOR_SLOTS] = calloc(P_LIMIT + 1, sizeof(*factors));
    unsigned char *factor_count = calloc(P_LIMIT + 1, 1);
    if (composite == NULL || residual == NULL || factors == NULL || factor_count == NULL) {
        free(factor_count); free(factors); free(residual); free(composite);
        return 1;
    }
    for (int p = 1; p <= P_LIMIT; ++p) {
        residual[p] = (uint64_t)p * p + 1;
    }
    for (int p = 2; p * p <= P_LIMIT; ++p) {
        if (!composite[p]) {
            for (int multiple = p * p; multiple <= P_LIMIT; multiple += p) composite[multiple] = 1;
        }
    }

    for (int prime = 2; prime <= P_LIMIT; ++prime) {
        if (composite[prime] || (prime != 2 && prime % 4 != 1)) continue;
        const uint64_t root = prime == 2 ? 1 : sqrt_minus_one((uint64_t)prime);
        const uint64_t roots[2] = {root, (uint64_t)prime - root};
        const int root_count = prime == 2 ? 1 : 2;
        for (int ri = 0; ri < root_count; ++ri) {
            uint64_t start = roots[ri];
            if (start == 0) start = (uint64_t)prime;
            for (uint64_t p = start; p <= P_LIMIT; p += (uint64_t)prime) {
                if (residual[p] % (uint64_t)prime != 0) continue;
                unsigned char exponent = 0;
                do {
                    residual[p] /= (uint64_t)prime;
                    ++exponent;
                } while (residual[p] % (uint64_t)prime == 0);
                const int slot = factor_count[p]++;
                if (slot >= FACTOR_SLOTS) return 1;
                factors[p][slot] = (Factor){(uint32_t)prime, exponent};
            }
        }
    }
    free(composite);
    for (int p = 1; p <= P_LIMIT; ++p) {
        if (residual[p] > 1) {
            const int slot = factor_count[p]++;
            if (slot >= FACTOR_SLOTS) return 1;
            factors[p][slot] = (Factor){(uint32_t)residual[p], 1};
        }
    }
    free(residual);

    size_t capacity = 2000000;
    size_t candidate_count = 0;
    uint64_t *candidates = malloc(capacity * sizeof(*candidates));
    if (candidates == NULL) {
        free(factor_count); free(factors);
        return 1;
    }

    uint64_t divisors[DIVISOR_SLOTS];
    for (uint64_t p = 1; p <= P_LIMIT; ++p) {
        int divisor_count = 1;
        divisors[0] = 1;
        for (int fi = 0; fi < factor_count[p]; ++fi) {
            const int old_count = divisor_count;
            uint64_t power = 1;
            for (int exponent = 1; exponent <= factors[p][fi].exponent; ++exponent) {
                power *= factors[p][fi].prime;
                for (int i = 0; i < old_count; ++i) {
                    if (divisor_count >= DIVISOR_SLOTS) return 1;
                    divisors[divisor_count++] = divisors[i] * power;
                }
            }
        }
        const uint64_t n = p * p + 1;
        for (int i = 0; i < divisor_count; ++i) {
            const uint64_t d = divisors[i];
            if (d > n / d) continue;
            const uint64_t e = n / d;
            const __uint128_t wide = (__uint128_t)p * (p + d) * (p + e);
            if (wide > UINT64_MAX) continue;
            if (candidate_count == capacity) {
                capacity *= 2;
                uint64_t *grown = realloc(candidates, capacity * sizeof(*grown));
                if (grown == NULL) return 1;
                candidates = grown;
            }
            candidates[candidate_count++] = (uint64_t)wide;
        }
    }
    free(factor_count);
    free(factors);

    qsort(candidates, candidate_count, sizeof(*candidates), uint64_compare);
    size_t unique = 0;
    uint64_t answer = 0;
    for (size_t i = 0; i < candidate_count; ++i) {
        if (i == 0 || candidates[i] != candidates[i - 1]) {
            ++unique;
            if (unique == TARGET) {
                answer = candidates[i];
                break;
            }
        }
    }
    const __uint128_t future_lower = (__uint128_t)(P_LIMIT + 1) *
                                     (P_LIMIT + 2) * (P_LIMIT + 2);
    if (answer == 0 || future_lower <= answer) {
        free(candidates);
        return 1;
    }
    printf("%llu\n", (unsigned long long)answer);
    free(candidates);
    return 0;
}
