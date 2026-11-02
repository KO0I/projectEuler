#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 223: almost right-angled triangles I.
 *
 * Fix the smallest side a and factor the difference of squares
 *
 *   (c-b)(c+b)=a^2-1.
 *
 * Put v=c-b and u=c+b.  Then b=(u-v)/2, c=(u+v)/2 and the perimeter is
 * simply a+u.  The perimeter and a<=b conditions bound the small divisor v:
 *
 *   ceil((a^2-1)/(P-a)) <= v <= floor(sqrt(2a^2-1))-a.
 *
 * We factor (a-1)(a+1) using a smallest-prime-factor table, recursively form
 * only divisors up to the upper bound, and enforce equal parity of u and v.
 * The exceptional a=1 family is (1,b,b) and is counted in closed form.
 */

enum { PERIMETER = 25000000, MAX_A = PERIMETER / 3 };

typedef struct {
    uint32_t prime;
    int exponent;
} Factor;

static Factor factors[20];
static int factor_count;
static uint64_t product_n;
static uint64_t minimum_v;
static uint64_t maximum_v;

static uint64_t integer_sqrt(uint64_t value) {
    uint64_t root = (uint64_t)sqrtl((long double)value);
    while ((root + 1) * (root + 1) <= value) ++root;
    while (root * root > value) --root;
    return root;
}

static void add_factor(uint32_t prime, int exponent) {
    for (int i = 0; i < factor_count; ++i) {
        if (factors[i].prime == prime) {
            factors[i].exponent += exponent;
            return;
        }
    }
    factors[factor_count++] = (Factor){prime, exponent};
}

static void factor_value(uint32_t value, const uint32_t *spf) {
    while (value > 1) {
        const uint32_t prime = spf[value] == 0 ? value : spf[value];
        int exponent = 0;
        do {
            value /= prime;
            ++exponent;
        } while (value % prime == 0);
        add_factor(prime, exponent);
    }
}

static uint64_t count_divisors(int index, uint64_t divisor) {
    if (index == factor_count) {
        if (divisor < minimum_v || divisor > maximum_v) return 0;
        const uint64_t partner = product_n / divisor;
        return ((partner ^ divisor) & 1U) == 0U;
    }
    uint64_t count = 0;
    uint64_t power = 1;
    for (int exponent = 0; exponent <= factors[index].exponent; ++exponent) {
        if (divisor * power > maximum_v) break;
        count += count_divisors(index + 1, divisor * power);
        power *= factors[index].prime;
    }
    return count;
}

int main(void) {
    uint32_t *spf = calloc((size_t)MAX_A + 2, sizeof(*spf));
    if (spf == NULL) return 1;
    for (uint32_t p = 2; (uint64_t)p * p <= MAX_A + 1; ++p) {
        if (spf[p] == 0) {
            for (uint64_t multiple = (uint64_t)p * p; multiple <= MAX_A + 1; multiple += p) {
                if (spf[multiple] == 0) spf[multiple] = p;
            }
        }
    }

    uint64_t total = (PERIMETER - 1) / 2; /* (1,b,b). */
    for (uint64_t a = 2; a <= MAX_A; ++a) {
        product_n = a * a - 1;
        minimum_v = (product_n + (PERIMETER - a) - 1) / (PERIMETER - a);
        maximum_v = integer_sqrt(2 * a * a - 1) - a;
        if (minimum_v > maximum_v) continue;

        factor_count = 0;
        factor_value((uint32_t)(a - 1), spf);
        factor_value((uint32_t)(a + 1), spf);
        total += count_divisors(0, 1);
    }
    printf("%llu\n", (unsigned long long)total);
    free(spf);
    return 0;
}
