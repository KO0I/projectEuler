#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project Euler 200: prime-proof squbes.
 *
 * A sqube has the unique form p^2 q^3 for distinct primes p and q.  We sieve
 * all primes needed below a deliberately generous 10^12 search ceiling,
 * generate every such product below it, sort the products, and inspect them
 * in numeric order.  The requested answer is reached well before the ceiling.
 *
 * “Prime-proof” is checked literally: replace each decimal position by each
 * of the other nine digits and reject the number if any replacement is prime.
 * Deterministic Miller-Rabin bases for 64-bit integers make this much faster
 * than trial division while retaining an exact result.  Modular products use
 * unsigned 128-bit intermediates so squaring cannot overflow uint64_t.
 */

static const uint64_t SEARCH_LIMIT = UINT64_C(1000000000000);

static uint64_t modular_power(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    while (exponent != 0) {
        if ((exponent & 1U) != 0U) {
            result = (uint64_t)((__uint128_t)result * base % modulus);
        }
        base = (uint64_t)((__uint128_t)base * base % modulus);
        exponent >>= 1U;
    }
    return result;
}

static int is_prime(uint64_t n) {
    static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(*small_primes); ++i) {
        if (n % small_primes[i] == 0) {
            return n == small_primes[i];
        }
    }
    if (n < 2) {
        return 0;
    }

    uint64_t odd_part = n - 1;
    unsigned twos = 0;
    while ((odd_part & 1U) == 0U) {
        odd_part >>= 1U;
        ++twos;
    }
    static const uint64_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(*bases); ++i) {
        const uint64_t base = bases[i] % n;
        if (base == 0) {
            continue;
        }
        uint64_t value = modular_power(base, odd_part, n);
        if (value == 1 || value == n - 1) {
            continue;
        }
        int witness = 1;
        for (unsigned r = 1; r < twos; ++r) {
            value = (uint64_t)((__uint128_t)value * value % n);
            if (value == n - 1) {
                witness = 0;
                break;
            }
        }
        if (witness) {
            return 0;
        }
    }
    return 1;
}

static int is_prime_proof(uint64_t value) {
    char digits[32];
    (void)snprintf(digits, sizeof(digits), "%llu", (unsigned long long)value);
    const size_t length = strlen(digits);
    for (size_t position = 0; position < length; ++position) {
        const char original = digits[position];
        for (char replacement = '0'; replacement <= '9'; ++replacement) {
            if (replacement == original) {
                continue;
            }
            digits[position] = replacement;
            const uint64_t changed = strtoull(digits, NULL, 10);
            if (is_prime(changed)) {
                digits[position] = original;
                return 0;
            }
        }
        digits[position] = original;
    }
    return 1;
}

static int uint64_compare(const void *left, const void *right) {
    const uint64_t a = *(const uint64_t *)left;
    const uint64_t b = *(const uint64_t *)right;
    return (a > b) - (a < b);
}

int main(void) {
    enum { PRIME_LIMIT = 1000000 };
    unsigned char *composite = calloc(PRIME_LIMIT + 1, 1);
    int *primes = malloc((PRIME_LIMIT + 1) * sizeof(*primes));
    if (composite == NULL || primes == NULL) {
        free(primes);
        free(composite);
        return 1;
    }

    int prime_count = 0;
    for (int n = 2; n <= PRIME_LIMIT; ++n) {
        if (!composite[n]) {
            primes[prime_count++] = n;
            if ((int64_t)n * n <= PRIME_LIMIT) {
                for (int multiple = n * n; multiple <= PRIME_LIMIT; multiple += n) {
                    composite[multiple] = 1;
                }
            }
        }
    }
    free(composite);

    size_t capacity = 131072;
    size_t count = 0;
    uint64_t *squbes = malloc(capacity * sizeof(*squbes));
    if (squbes == NULL) {
        free(primes);
        return 1;
    }

    for (int qi = 0; qi < prime_count; ++qi) {
        const uint64_t q = (uint64_t)primes[qi];
        const uint64_t q_cubed = q * q * q;
        if (q_cubed > SEARCH_LIMIT) {
            break;
        }
        const uint64_t maximum_p_squared = SEARCH_LIMIT / q_cubed;
        for (int pi = 0; pi < prime_count; ++pi) {
            const uint64_t p = (uint64_t)primes[pi];
            if (p * p > maximum_p_squared) {
                break;
            }
            if (p == q) {
                continue;
            }
            if (count == capacity) {
                capacity *= 2;
                uint64_t *grown = realloc(squbes, capacity * sizeof(*grown));
                if (grown == NULL) {
                    free(squbes);
                    free(primes);
                    return 1;
                }
                squbes = grown;
            }
            squbes[count++] = p * p * q_cubed;
        }
    }
    free(primes);

    qsort(squbes, count, sizeof(*squbes), uint64_compare);
    int qualifying = 0;
    for (size_t i = 0; i < count; ++i) {
        char text[32];
        (void)snprintf(text, sizeof(text), "%llu", (unsigned long long)squbes[i]);
        if (strstr(text, "200") != NULL && is_prime_proof(squbes[i])) {
            ++qualifying;
            if (qualifying == 200) {
                printf("%llu\n", (unsigned long long)squbes[i]);
                free(squbes);
                return 0;
            }
        }
    }

    /* Reaching the ceiling without 200 values would mean the chosen finite
       generation bound was insufficient, so fail instead of printing a lie. */
    free(squbes);
    return 1;
}
