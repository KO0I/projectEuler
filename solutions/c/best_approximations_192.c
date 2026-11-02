#include <gmp.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 192: best approximations.
 *
 * Continued fractions identify the only candidates that can be closest to an
 * irrational with a bounded denominator.  Let p/q be the final convergent
 * with q <= Q and let p0/q0 be its predecessor.  If the next convergent
 * exceeds Q, the sole additional candidate is the furthest admissible
 * semiconvergent
 *
 *       (p0 + k p) / (q0 + k q),  k = floor((Q - q0) / q).
 *
 * All earlier fractions are dominated by one of these two.  Numerators and
 * denominators fit in uint64_t, but the two errors can differ far below the
 * precision of a C floating type.  GMP floats at 256-bit precision are used
 * only for that final comparison; the continued-fraction recurrence remains
 * exact integer arithmetic.  Perfect squares are excluded by the problem.
 */

enum { LIMIT_N = 100000 };
static const uint64_t DENOMINATOR_LIMIT = UINT64_C(1000000000000);

static int integer_sqrt(int n) {
    int low = 0;
    int high = 317;
    while (low + 1 < high) {
        const int middle = (low + high) / 2;
        if (middle * middle <= n) {
            low = middle;
        } else {
            high = middle;
        }
    }
    return low;
}

static int first_is_closer(int n, uint64_t p1, uint64_t q1,
                           uint64_t p2, uint64_t q2) {
    mpf_t root, value1, value2, error1, error2;
    mpf_inits(root, value1, value2, error1, error2, NULL);
    mpf_sqrt_ui(root, (unsigned long)n);

    mpf_set_ui(value1, (unsigned long)p1);
    mpf_div_ui(value1, value1, (unsigned long)q1);
    mpf_sub(error1, value1, root);
    mpf_abs(error1, error1);

    mpf_set_ui(value2, (unsigned long)p2);
    mpf_div_ui(value2, value2, (unsigned long)q2);
    mpf_sub(error2, value2, root);
    mpf_abs(error2, error2);

    const int result = mpf_cmp(error1, error2) < 0;
    mpf_clears(root, value1, value2, error1, error2, NULL);
    return result;
}

static uint64_t best_denominator(int n) {
    const int a0 = integer_sqrt(n);
    int m = 0;
    int d = 1;
    int a = a0;

    /* Standard convergent seeds: p[-2]/q[-2] and p[-1]/q[-1]. */
    uint64_t p0 = 0;
    uint64_t q0 = 1;
    uint64_t p1 = 1;
    uint64_t q1 = 0;

    for (;;) {
        const uint64_t p2 = (uint64_t)a * p1 + p0;
        const uint64_t q2 = (uint64_t)a * q1 + q0;
        if (q2 > DENOMINATOR_LIMIT) {
            const uint64_t k = (DENOMINATOR_LIMIT - q0) / q1;
            const uint64_t semi_p = p0 + k * p1;
            const uint64_t semi_q = q0 + k * q1;
            return first_is_closer(n, p1, q1, semi_p, semi_q) ? q1 : semi_q;
        }
        p0 = p1;
        q0 = q1;
        p1 = p2;
        q1 = q2;

        m = d * a - m;
        d = (n - m * m) / d;
        a = (a0 + m) / d;
    }
}

int main(void) {
    mpf_set_default_prec(256);
    uint64_t sum = 0;
    for (int n = 2; n <= LIMIT_N; ++n) {
        const int root = integer_sqrt(n);
        if (root * root != n) {
            sum += best_denominator(n);
        }
    }
    printf("%llu\n", (unsigned long long)sum);
    return 0;
}
