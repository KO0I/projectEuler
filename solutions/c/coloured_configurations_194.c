#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 194: coloured configurations.
 *
 * Adjacent units meet along an ordered pair of differently coloured boundary
 * vertices.  Colour names are symmetric, so after that pair is fixed, every
 * type-A unit has the same number A(c) of extensions and every type-B unit
 * has B(c).  Directly enumerating the five new vertices of either small graph
 * yields their chromatic polynomials (with the incoming edge fixed):
 *
 * A(c) = c^5 - 9c^4 + 34c^3 - 69c^2 + 77c - 38
 * B(c) = c^5 - 8c^4 + 27c^3 - 50c^2 + 52c - 24.
 *
 * The first boundary contributes c(c-1), and choosing which 25 of the 100
 * unit positions are type A contributes C(100,25).  Everything is evaluated
 * modulo 10^8.  Pascal's recurrence computes the binomial modulo a composite
 * modulus without an invalid modular division.
 */

static const uint64_t MODULUS = UINT64_C(100000000);

static uint64_t multiply_mod(uint64_t a, uint64_t b) {
    return (a * b) % MODULUS;
}

static uint64_t power_mod(uint64_t base, unsigned exponent) {
    uint64_t result = 1;
    while (exponent != 0) {
        if ((exponent & 1U) != 0U) {
            result = multiply_mod(result, base);
        }
        base = multiply_mod(base, base);
        exponent >>= 1U;
    }
    return result;
}

static uint64_t unit_a(uint64_t c) {
    /* Horner form keeps intermediate values below c^5 and documents the
       signed polynomial without relying on unsigned underflow. */
    int64_t value = (int64_t)c - 9;
    value = value * (int64_t)c + 34;
    value = value * (int64_t)c - 69;
    value = value * (int64_t)c + 77;
    value = value * (int64_t)c - 38;
    return (uint64_t)value;
}

static uint64_t unit_b(uint64_t c) {
    int64_t value = (int64_t)c - 8;
    value = value * (int64_t)c + 27;
    value = value * (int64_t)c - 50;
    value = value * (int64_t)c + 52;
    value = value * (int64_t)c - 24;
    return (uint64_t)value;
}

static uint64_t binomial_mod(int n, int k) {
    uint64_t row[26] = {0};
    row[0] = 1;
    for (int i = 1; i <= n; ++i) {
        const int upper = i < k ? i : k;
        for (int j = upper; j >= 1; --j) {
            row[j] = (row[j] + row[j - 1]) % MODULUS;
        }
    }
    return row[k];
}

int main(void) {
    const uint64_t colours = 1984;
    uint64_t answer = binomial_mod(100, 25);
    answer = multiply_mod(answer, colours * (colours - 1));
    answer = multiply_mod(answer, power_mod(unit_a(colours) % MODULUS, 25));
    answer = multiply_mod(answer, power_mod(unit_b(colours) % MODULUS, 75));
    printf("%08llu\n", (unsigned long long)answer);
    return 0;
}
