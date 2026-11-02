#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * Project Euler 217: balanced numbers.
 *
 * For h digits, keep by digit sum both the number of strings and the sum of
 * their numeric values.  One DP allows leading zeroes (the right half), while
 * the other starts with 1..9 (the left half of a full-length integer).  When a
 * digit d is appended, value becomes 10*value+d, giving an exact recurrence
 * for the aggregate sums as well as the counts.
 *
 * Equal-sum left/right buckets are combined algebraically.  Even lengths use
 * left*10^h+right.  Odd lengths insert an arbitrary middle digit; its ten
 * choices multiply the outer contributions, and their digit sum 45 supplies
 * the middle-place contribution.  Every operation is reduced modulo 3^15.
 */

enum { MAX_HALF = 23, MAX_SUM = 9 * MAX_HALF, MODULUS = 14348907 };

static void append_digit_layer(const int64_t *old_count, const int64_t *old_sum,
                               int old_max_sum, int64_t *new_count, int64_t *new_sum) {
    memset(new_count, 0, (MAX_SUM + 1) * sizeof(*new_count));
    memset(new_sum, 0, (MAX_SUM + 1) * sizeof(*new_sum));
    for (int sum = 0; sum <= old_max_sum; ++sum) {
        for (int digit = 0; digit <= 9; ++digit) {
            const int next_sum = sum + digit;
            new_count[next_sum] = (new_count[next_sum] + old_count[sum]) % MODULUS;
            new_sum[next_sum] = (new_sum[next_sum] +
                                 10 * old_sum[sum] + digit * old_count[sum]) % MODULUS;
        }
    }
}

int main(void) {
    int64_t right_count[MAX_SUM + 1] = {0};
    int64_t right_sum[MAX_SUM + 1] = {0};
    int64_t left_count[MAX_SUM + 1] = {0};
    int64_t left_sum[MAX_SUM + 1] = {0};
    int64_t next_count[MAX_SUM + 1];
    int64_t next_sum[MAX_SUM + 1];
    right_count[0] = 1; /* The unique empty string for h=0. */

    int64_t powers10[2 * MAX_HALF + 2];
    powers10[0] = 1;
    for (int i = 1; i <= 2 * MAX_HALF + 1; ++i) {
        powers10[i] = powers10[i - 1] * 10 % MODULUS;
    }

    int64_t answer = 45; /* All one-digit positive integers are balanced. */
    for (int h = 1; h <= MAX_HALF; ++h) {
        append_digit_layer(right_count, right_sum, 9 * (h - 1), next_count, next_sum);
        memcpy(right_count, next_count, sizeof(right_count));
        memcpy(right_sum, next_sum, sizeof(right_sum));

        if (h == 1) {
            for (int digit = 1; digit <= 9; ++digit) {
                left_count[digit] = 1;
                left_sum[digit] = digit;
            }
        } else {
            append_digit_layer(left_count, left_sum, 9 * (h - 1), next_count, next_sum);
            memcpy(left_count, next_count, sizeof(left_count));
            memcpy(left_sum, next_sum, sizeof(left_sum));
        }

        int64_t even_total = 0;
        int64_t odd_total = 0;
        for (int sum = 0; sum <= 9 * h; ++sum) {
            const int64_t pair_count = left_count[sum] * right_count[sum] % MODULUS;
            const int64_t even = (left_sum[sum] * right_count[sum] % MODULUS) *
                                 powers10[h] % MODULUS +
                                 right_sum[sum] * left_count[sum] % MODULUS;
            even_total = (even_total + even) % MODULUS;

            const int64_t outer = (10 * left_sum[sum] % MODULUS) *
                                  right_count[sum] % MODULUS * powers10[h + 1] % MODULUS +
                                  10 * right_sum[sum] % MODULUS * left_count[sum] % MODULUS;
            const int64_t middle = 45 * powers10[h] % MODULUS * pair_count % MODULUS;
            odd_total = (odd_total + outer + middle) % MODULUS;
        }
        answer = (answer + even_total + odd_total) % MODULUS;
    }

    printf("%lld\n", (long long)answer);
    return 0;
}
