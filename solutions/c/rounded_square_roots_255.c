#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 255: Rounded Square Roots
 *
 * Iterating separately over 90 trillion inputs is unnecessary.  For a fixed
 * current approximation x,
 *
 *   next = floor((x + ceil(n/x)) / 2)
 *
 * is constant on long intervals of n.  If q=ceil(n/x), then q is constant for
 * (q-1)x < n <= qx.  Moreover floor((x+q)/2)=y for at most two consecutive q:
 *
 *   q = 2y-x or q = 2y-x+1.
 *
 * The recursive routine below partitions an inclusive input interval [lo,hi]
 * into maximal children sharing the same next approximation y.  A child with
 * y==x has converged; its length is multiplied by the number of iterations
 * taken.  Otherwise only that aggregate interval is recursively refined.
 *
 * This is exact integer arithmetic throughout.  The largest endpoint product
 * is below 10^14, and the final weighted iteration count is below 10^15, both
 * safely inside uint64_t.  Floating point appears only in the final division
 * and formatting to ten decimal places.
 */

static uint64_t ceiling_division(uint64_t numerator, uint64_t denominator) {
    return numerator / denominator + (numerator % denominator != 0);
}

static uint64_t iteration_total(uint64_t lo, uint64_t hi, uint64_t x,
                                unsigned completed_iterations) {
    uint64_t first_q = ceiling_division(lo, x);
    uint64_t last_q = ceiling_division(hi, x);
    uint64_t first_y = (x + first_q) / 2;
    uint64_t last_y = (x + last_q) / 2;
    uint64_t total = 0;

    for (uint64_t y = first_y; y <= last_y; ++y) {
        /* floor((x+q)/2)=y exactly for the following one or two q values. */
        uint64_t q_lo = 2 * y - x;
        uint64_t q_hi = q_lo + 1;
        if (q_lo < first_q) {
            q_lo = first_q;
        }
        if (q_hi > last_q) {
            q_hi = last_q;
        }

        uint64_t child_lo = (q_lo - 1) * x + 1;
        uint64_t child_hi = q_hi * x;
        if (child_lo < lo) {
            child_lo = lo;
        }
        if (child_hi > hi) {
            child_hi = hi;
        }

        unsigned iterations = completed_iterations + 1;
        if (y == x) {
            total += (child_hi - child_lo + 1) * iterations;
        } else {
            total += iteration_total(child_lo, child_hi, y, iterations);
        }
    }
    return total;
}

static uint64_t power_of_ten(unsigned exponent) {
    uint64_t value = 1;
    while (exponent-- != 0) {
        value *= 10;
    }
    return value;
}

static uint64_t initial_approximation(unsigned digits) {
    if (digits % 2 != 0) {
        return 2 * power_of_ten((digits - 1) / 2);
    }
    return 7 * power_of_ten((digits - 2) / 2);
}

int main(void) {
    /* The statement's five-digit example is an inexpensive executable check
     * of all interval endpoints and of whether the final equality is counted. */
    uint64_t five_digit_total =
        iteration_total(UINT64_C(10000), UINT64_C(99999),
                        initial_approximation(5), 0);
    if (five_digit_total != UINT64_C(288926)) {
        return 1;
    }

    uint64_t lo = UINT64_C(10000000000000);
    uint64_t hi = UINT64_C(99999999999999);
    uint64_t count = hi - lo + 1;
    uint64_t total = iteration_total(lo, hi, initial_approximation(14), 0);
    printf("%.10f\n", (double)total / (double)count);
    return 0;
}
